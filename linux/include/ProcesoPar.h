#ifndef PROCESO_PAR_H
#define PROCESO_PAR_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int Estado_t;

#define E_OK            0
#define E_PAR_INC       1
#define E_NO_MEM        2
#define E_PIPE_FAIL     3
#define E_FORK_FAIL     4
#define E_EXEC_FAIL     5
#define E_WRITE_FAIL    6
#define E_READ_FAIL     7
#define E_THREAD_FAIL   8
#define E_CALLBACK_NULL 9

typedef struct ProcesoPar_t ProcesoPar_t;

typedef Estado_t (*FuncionEscucha_t)(const char *mensaje, int longitud);

Estado_t lanzarProcesoPar(const char *nombreArchivo, const char **listaLineaComando, ProcesoPar_t **procesoPar);
Estado_t enviarMensajeProcesoPar(ProcesoPar_t *procesoPar, const char *mensaje, int longitud);
Estado_t establecerFuncionDeEscucha(ProcesoPar_t *procesoPar, FuncionEscucha_t f);
Estado_t destruirProcesoPar(ProcesoPar_t *procesoPar);

#endif
