#include "../include/ProcesoPar.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

Estado_t mi_callback(const char *mensaje, int longitud) {
    printf("[Padre] Callback recibió: %.*s\n", longitud, mensaje);
    return E_OK;
}

int main() {
    ProcesoPar_t *p = NULL;

    // Ruta real al hijo en tu WSL
    const char *hijo_path = "/home/dacruzj/proceso_test/hijo";

    // argv para execvp
    const char *args[] = { hijo_path, NULL };

    // Lanzar el proceso hijo
    if (lanzarProcesoPar(hijo_path, args, &p) != E_OK) {
        printf("Error al lanzar el proceso par\n");
        return 1;
    }

    // Establecer callback
    establecerFuncionDeEscucha(p, mi_callback);

    // Enviar un mensaje simple
    const char *msg = "Hola hijo!";
    enviarMensajeProcesoPar(p, msg, strlen(msg));

    // Opcional: enviar más mensajes seguidos
    for (int i = 0; i < 3; i++) {
        char buf[64];
        sprintf(buf, "Mensaje %d desde padre", i);
        enviarMensajeProcesoPar(p, buf, strlen(buf));
        usleep(200000); // 0.2s
    }

    // Esperar respuestas
    sleep(1);

    // Destruir proceso par y recursos
    destruirProcesoPar(p);
    return 0;
}
