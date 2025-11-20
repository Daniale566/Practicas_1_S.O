#include "internal_proceso.h"

Estado_t enviarMensajeProcesoPar(ProcesoPar_t *p,
                                 const char *mensaje,
                                 int longitud)
{
    if (!p || p->fd_write < 0) return E_PAR_INC;

    pthread_mutex_lock(&p->write_mutex);

    uint32_t netlen = htonl(longitud);
    if (write_all(p->fd_write, &netlen, 4) < 0) {
        pthread_mutex_unlock(&p->write_mutex);
        return E_WRITE_FAIL;
    }

    if (longitud > 0) {
        if (write_all(p->fd_write, mensaje, longitud) < 0) {
            pthread_mutex_unlock(&p->write_mutex);
            return E_WRITE_FAIL;
        }
    }

    pthread_mutex_unlock(&p->write_mutex);
    return E_OK;
}
