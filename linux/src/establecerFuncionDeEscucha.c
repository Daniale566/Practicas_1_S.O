#include "internal_proceso.h"

Estado_t establecerFuncionDeEscucha(ProcesoPar_t *p, FuncionEscucha_t f)
{
    if (!p) return E_PAR_INC;
    p->callback = f;
    return E_OK;
}
