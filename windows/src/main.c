
#include "ProcesoPar.h"
#include <stdio.h>
#include <string.h>

static Estado_t callback(const char *msg, int len) {
    printf("[A] Recibido desde B (%d bytes): '%.*s'\n", len, len, msg);
    return E_OK;
}

int main(void) {
    ProcesoPar_t *pp = NULL;

#ifdef _WIN32
    const char *exe = "eco.exe";
#else
    const char *exe = "./eco";  //  asegúrate de compilarlo
#endif

    const char *args[] = { NULL }; // sin argumentos adicionales

    Estado_t st = lanzarProcesoPar(exe, args, &pp);
    if (st != E_OK) {
        printf("Error lanzarProcesoPar: %u\n", st);
        return 1;
    }

    st = establecerFuncionDeEscucha(pp, callback);
    if (st != E_OK) {
        printf("Error establecerFuncionDeEscucha: %u\n", st);
        destruirProcesoPar(pp);
        return 1;
    }

    const char *m1 = "hola proceso B";
    enviarMensajeProcesoPar(pp, m1, (int)strlen(m1));

    const char *m2 = "otro mensaje";
    enviarMensajeProcesoPar(pp, m2, (int)strlen(m2));

    printf("Presiona ENTER para terminar...\n");
    getchar();

    destruirProcesoPar(pp);
    return 0;
}
