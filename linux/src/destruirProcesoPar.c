#include "internal_proceso.h"

Estado_t destruirProcesoPar(ProcesoPar_t *p)
{
    if (!p) return E_PAR_INC;

    p->terminando = 1;

    if (p->fd_write >= 0) close(p->fd_write);
    if (p->fd_read >= 0) close(p->fd_read);

    if (p->hiloCreado)
        pthread_join(p->hiloEscucha, NULL);

    kill(p->pid, SIGTERM);
    waitpid(p->pid, NULL, 0);

    pthread_mutex_destroy(&p->write_mutex);
    free(p);

    return E_OK;
}
