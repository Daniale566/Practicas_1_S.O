#ifndef PROCESOPAR_H
#define PROCESOPAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef unsigned int Estado_t;

/* Códigos de estado básicos */
#define E_OK            0   /* La función se comportó bien */
#define E_PAR_INC       1   /* Parámetro incorrecto      */
#define E_SYS_ERROR     2   /* Error de llamada al sistema */
#define E_MEM_ERROR     3   /* Error de memoria          */
#define E_IO_ERROR      4   /* Error de E/S              */
#define E_PIPE_CERRADA  5   /* La tubería se cerró       */
#define E_STATE_ERROR   6   /* Estado no válido          */

#ifdef _WIN32
    #include <windows.h>

    typedef struct ProcesoPar {
        HANDLE  hProcess;
        HANDLE  hThread;
        HANDLE  hStdinWrite;   /* A -> B */
        HANDLE  hStdoutRead;   /* B -> A */
        Estado_t (*callback)(const char*, int);
        volatile int corriendo;
    } ProcesoPar_t;

#else  /* Linux / POSIX */
    #include <pthread.h>
    #include <sys/types.h>

    typedef struct ProcesoPar {
        pid_t    pid;
        int      fd_escritura;   /* A -> B */
        int      fd_lectura;     /* B -> A */
        pthread_t hilo_escucha;
        Estado_t (*callback)(const char*, int);
        volatile int corriendo;
    } ProcesoPar_t;

#endif

/* Prototipos de la API */
Estado_t lanzarProcesoPar(const char *nombreArchivoEjecutable,
                          const char **listaLineaComando,
                          ProcesoPar_t **procesoPar);

Estado_t destruirProcesoPar(ProcesoPar_t *procesoPar);

Estado_t enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
                                 const char *mensaje,
                                 int longitud);

Estado_t establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
                                    Estado_t (*f)(const char*, int));

#ifdef __cplusplus
}
#endif

#endif /* PROCESOPAR_H */
