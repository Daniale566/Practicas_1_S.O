#include "internal_proceso.h"

Estado_t lanzarProcesoPar(const char *nombreArchivo,
                          const char **listaLineaComando,
                          ProcesoPar_t **procesoPar)
{
    if (!nombreArchivo || !procesoPar) return E_PAR_INC;

    ProcesoPar_t *p = calloc(1, sizeof(ProcesoPar_t));
    if (!p) return E_NO_MEM;

    pthread_mutex_init(&p->write_mutex, NULL);

    if (pipe(p->pipePadreAHijo) < 0) return E_PIPE_FAIL;
    if (pipe(p->pipeHijoAPadre) < 0) return E_PIPE_FAIL;

    pid_t pid = fork();

    if (pid < 0) return E_FORK_FAIL;

    if (pid == 0) {
        dup2(p->pipePadreAHijo[0], STDIN_FILENO);
        dup2(p->pipeHijoAPadre[1], STDOUT_FILENO);

        close(p->pipePadreAHijo[1]);
        close(p->pipeHijoAPadre[0]);

        int argc = 0;
        while (listaLineaComando && listaLineaComando[argc]) argc++;

        char **argv = malloc((argc + 2) * sizeof(char *));
        argv[0] = (char *)nombreArchivo;

        for (int i = 0; i < argc; i++)
            argv[i + 1] = (char *)listaLineaComando[i];

        argv[argc + 1] = NULL;

        execvp(nombreArchivo, argv);
        perror("execvp");
        exit(1);
    }

    p->pid = pid;
    close(p->pipePadreAHijo[0]);
    close(p->pipeHijoAPadre[1]);

    p->fd_write = p->pipePadreAHijo[1];
    p->fd_read  = p->pipeHijoAPadre[0];

    pthread_create(&p->hiloEscucha, NULL, hilo_lector, p);

    p->hiloCreado = 1;
    *procesoPar = p;

    return E_OK;
}
