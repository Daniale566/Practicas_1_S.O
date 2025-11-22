// windows/src/proceso_par_win.c
#include <winsock2.h>// htonl, ntohl
#include "ProcesoPar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
   
#include <windows.h>

/* === Helpers internos === */

static Estado_t write_all_win(HANDLE h, const void *buf, DWORD n) {
    const char *p = (const char*)buf;
    while (n > 0) {
        DWORD written = 0;
        if (!WriteFile(h, p, n, &written, NULL)) {
            return E_IO_ERROR;
        }
        if (written == 0) {
            return E_PIPE_CERRADA;
        }
        p += written;
        n -= written;
    }
    return E_OK;
}

static Estado_t read_exact_win(HANDLE h, void *buf, DWORD n) {
    char *p = (char*)buf;
    while (n > 0) {
        DWORD readBytes = 0;
        if (!ReadFile(h, p, n, &readBytes, NULL)) {
            return E_IO_ERROR;
        }
        if (readBytes == 0) {
            return E_PIPE_CERRADA;
        }
        p += readBytes;
        n -= readBytes;
    }
    return E_OK;
}

/* === Hilo de escucha === */
/* === representacion proceso par B === */

static DWORD WINAPI thread_escucha(LPVOID param) {
    ProcesoPar_t *pp = (ProcesoPar_t*)param;

    while (pp->corriendo) {
        uint32_t len_net = 0; //mini protocolo
        Estado_t st = read_exact_win(pp->hStdoutRead,
                                     &len_net, sizeof(len_net));
        if (st != E_OK) {
            break;
        }

        uint32_t len = ntohl(len_net);
        if (len == 0) continue;

        char *buffer = (char*)malloc(len + 1);
        if (!buffer) break;

        st = read_exact_win(pp->hStdoutRead, buffer, len);
        if (st != E_OK) {
            free(buffer);
            break;
        }
        buffer[len] = '\0';

        if (pp->callback) {
            pp->callback(buffer, (int)len);
        }

        free(buffer);
    }

    return 0;
}

/* Construye la línea de comandos: "exe arg1 arg2 ..." */
static char* build_command_line(const char *exe, const char **args) {
    size_t len = strlen(exe) + 1;
    int count = 0;
    if (args) {
        while (args[count] != NULL) {
            len += strlen(args[count]) + 1;
            count++;
        }
    }

    char *cmd = (char*)malloc(len + 1);
    if (!cmd) return NULL;

    strcpy(cmd, exe);
    for (int i = 0; i < count; ++i) {
        strcat(cmd, " ");
        strcat(cmd, args[i]);
    }

    return cmd;
}

/* === Implementación API === */

Estado_t lanzarProcesoPar(const char *nombreArchivoEjecutable,
                          const char **listaLineaComando,
                          ProcesoPar_t **procesoPar)
{
    if (!nombreArchivoEjecutable || !procesoPar) {
        return E_PAR_INC;
    }
    *procesoPar = NULL;

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hChildStdoutRd = NULL, hChildStdoutWr = NULL;
    HANDLE hChildStdinRd  = NULL, hChildStdinWr  = NULL;

    // Pipe para STDOUT del hijo (B -> A)
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
        return E_SYS_ERROR;

    if (!SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0)) {
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        return E_SYS_ERROR;
    }

    // Pipe para STDIN del hijo (A -> B)
    if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        return E_SYS_ERROR;
    }

    if (!SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0)) {
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        return E_SYS_ERROR;
    }

    // Construir la línea de comandos
    char *cmdLine = build_command_line(nombreArchivoEjecutable,
                                       listaLineaComando);
    if (!cmdLine) {
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        return E_MEM_ERROR;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    si.cb = sizeof(STARTUPINFOA);
    si.hStdError  = hChildStdoutWr;
    si.hStdOutput = hChildStdoutWr;
    si.hStdInput  = hChildStdinRd;
    si.dwFlags   |= STARTF_USESTDHANDLES;

    BOOL ok = CreateProcessA(
        NULL,           // lpApplicationName
        cmdLine,        // lpCommandLine
        NULL,           // lpProcessAttributes
        NULL,           // lpThreadAttributes
        TRUE,           // bInheritHandles
        0,              // dwCreationFlags
        NULL,           // lpEnvironment
        NULL,           // lpCurrentDirectory
        &si,
        &pi
    );

    free(cmdLine);
    CloseHandle(hChildStdoutWr); // ya en el hijo
    CloseHandle(hChildStdinRd);  // ya en el hijo

    if (!ok) {
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdinWr);
        return E_SYS_ERROR;
    }

    ProcesoPar_t *pp = (ProcesoPar_t*)malloc(sizeof(ProcesoPar_t));
    if (!pp) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdinWr);
        return E_MEM_ERROR;
    }

    pp->hProcess    = pi.hProcess;
    pp->hThread     = NULL;
    pp->hStdoutRead = hChildStdoutRd;
    pp->hStdinWrite = hChildStdinWr;
    pp->callback    = NULL;
    pp->corriendo   = 0;

    CloseHandle(pi.hThread); // no lo necesitamos

    *procesoPar = pp;
    return E_OK;
}

Estado_t establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
                                    Estado_t (*f)(const char*, int))
{
    if (!procesoPar) return E_PAR_INC;

    procesoPar->callback = f;
    if (procesoPar->corriendo) {
        return E_OK;
    }

    procesoPar->corriendo = 1;
    HANDLE hThread = CreateThread(
        NULL, 0, thread_escucha, procesoPar, 0, NULL
    );
    if (!hThread) {
        procesoPar->corriendo = 0;
        return E_SYS_ERROR;
    }

    procesoPar->hThread = hThread;
    return E_OK;
}

Estado_t enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
                                 const char *mensaje,
                                 int longitud)
{
    if (!procesoPar || !mensaje || longitud < 0) {
        return E_PAR_INC;
    }

    uint32_t len = (uint32_t)longitud;
    uint32_t len_net = htonl(len);

    Estado_t st = write_all_win(procesoPar->hStdinWrite,
                                &len_net, sizeof(len_net));
    if (st != E_OK) return st;

    return write_all_win(procesoPar->hStdinWrite,
                         mensaje, (DWORD)longitud);
}

Estado_t destruirProcesoPar(ProcesoPar_t *procesoPar)
{
    if (!procesoPar) return E_PAR_INC;

    procesoPar->corriendo = 0;

    // Cerrar pipes
    CloseHandle(procesoPar->hStdoutRead);
    CloseHandle(procesoPar->hStdinWrite);

    // Esperar hilo
    if (procesoPar->hThread) {
        WaitForSingleObject(procesoPar->hThread, INFINITE);
        CloseHandle(procesoPar->hThread);
    }

    // Esperar proceso
    WaitForSingleObject(procesoPar->hProcess, INFINITE);
    CloseHandle(procesoPar->hProcess);

    free(procesoPar);
    return E_OK;
}
