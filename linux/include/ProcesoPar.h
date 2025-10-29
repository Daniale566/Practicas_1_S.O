#ifndef PROCESO_PAR_H
#define PROCESO_PAR_H

/**
 * @file ProcesoPar.h
 * @brief Biblioteca para la creación y manejo de procesos pares con comunicación bidireccional.
 *
 * Permite que un proceso A cree un proceso B, estableciendo comunicación
 * full-duplex (bidireccional) mediante tuberías (pipes).
 *
 * Implementaciones esperadas:
 *   - Linux (usando fork/exec/pipe)
 *   - Windows (usando CreateProcess y CreatePipe)
 */

#include <stdio.h>
#include <stdlib.h>

/* ---------------------------------------------
 * Tipos de datos básicos
 * --------------------------------------------- */

/**
 * @typedef Estado_t
 * @brief Tipo genérico para códigos de estado o error.
 */
typedef unsigned int Estado_t;

/**
 * @brief Códigos de retorno estándar.
 */
#define E_OK            0   ///< Operación exitosa
#define E_PAR_INC       1   ///< Parámetros incorrectos
#define E_NO_MEM        2   ///< Fallo de asignación de memoria
#define E_PIPE_FAIL     3   ///< Fallo al crear tubería
#define E_FORK_FAIL     4   ///< Fallo al crear proceso (fork)
#define E_EXEC_FAIL     5   ///< Fallo en exec() / CreateProcess
#define E_WRITE_FAIL    6   ///< Fallo al enviar datos
#define E_READ_FAIL     7   ///< Fallo al leer datos
#define E_THREAD_FAIL   8   ///< Fallo al crear hilo de escucha
#define E_CALLBACK_NULL 9   ///< No se ha establecido función de escucha

/* ---------------------------------------------
 * Estructura principal
 * --------------------------------------------- */

/**
 * @brief Representa un proceso par y sus recursos asociados.
 *
 * Los campos reales son privados y dependen de la implementación
 * (Linux o Windows).
 */
typedef struct ProcesoPar_t ProcesoPar_t;

/**
 * @typedef FuncionEscucha_t
 * @brief Tipo de función callback para manejar mensajes recibidos.
 *
 * @param mensaje  Puntero al buffer del mensaje recibido.
 * @param longitud Longitud del mensaje (en bytes).
 * @return Código de estado (E_OK si se procesó correctamente).
 */
typedef Estado_t (*FuncionEscucha_t)(const char *mensaje, int longitud);

/* ---------------------------------------------
 * API pública
 * --------------------------------------------- */

/**
 * @brief Lanza un proceso par y establece comunicación bidireccional.
 *
 * @param nombreArchivo     Ruta del ejecutable del proceso hijo.
 * @param listaLineaComando Lista de argumentos (NULL-terminada).
 * @param procesoPar        Puntero donde se almacenará la instancia creada.
 * @return Estado_t         Código de resultado (E_OK si todo fue correcto).
 */
Estado_t lanzarProcesoPar(const char *nombreArchivo, const char **listaLineaComando, ProcesoPar_t **procesoPar);

/**
 * @brief Envía un mensaje al proceso par.
 *
 * @param procesoPar Puntero al proceso par.
 * @param mensaje    Buffer con el mensaje.
 * @param longitud   Longitud en bytes del mensaje.
 * @return Estado_t  E_OK si se envió correctamente.
 */
Estado_t enviarMensajeProcesoPar(ProcesoPar_t *procesoPar, const char *mensaje, int longitud);

/**
 * @brief Establece la función de escucha para mensajes recibidos.
 *
 * @param procesoPar Puntero al proceso par.
 * @param f          Puntero a la función callback que procesará los mensajes.
 * @return Estado_t  E_OK si se registró correctamente.
 */
Estado_t establecerFuncionDeEscucha(ProcesoPar_t *procesoPar, FuncionEscucha_t f);

/**
 * @brief Destruye un proceso par, cerrando tuberías y liberando recursos.
 *
 * @param procesoPar Puntero al proceso par.
 * @return Estado_t  E_OK si se liberó correctamente.
 */
Estado_t destruirProcesoPar(ProcesoPar_t *procesoPar);

#endif /* PROCESO_PAR_H */
