#ifndef INTERNAL_PROCESO_H
#define INTERNAL_PROCESO_H

#include "../include/ProcesoPar.h"

#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

/* Estructura interna real */
struct ProcesoPar_t {
    int pipePadreAHijo[2];
    int pipeHijoAPadre[2];
    pid_t pid;
    FuncionEscucha_t callback;
    pthread_t hiloEscucha;
    pthread_mutex_t write_mutex;
    int hiloCreado;
    int fd_write;
    int fd_read;
    int terminando;
};

/* --- utilidades I/O robustas --- */
static ssize_t write_all(int fd, const void *buf, size_t count);
static ssize_t read_all(int fd, void *buf, size_t count);

/* --- hilo lector interno --- */
static void *hilo_lector(void *arg);

/* IMPLEMENTACIONES */
static ssize_t write_all(int fd, const void *buf, size_t count) {
    const uint8_t *p = buf;
    size_t left = count;
    while (left > 0) {
        ssize_t w = write(fd, p, left);
        if (w < 0) { if (errno == EINTR) continue; return -1; }
        left -= w;
        p += w;
    }
    return count;
}

static ssize_t read_all(int fd, void *buf, size_t count) {
    uint8_t *p = buf;
    size_t left = count;
    while (left > 0) {
        ssize_t r = read(fd, p, left);
        if (r == 0) return count - left;
        if (r < 0) { if (errno == EINTR) continue; return -1; }
        left -= r;
        p += r;
    }
    return count;
}

static void *hilo_lector(void *arg) {
    ProcesoPar_t *p = arg;
    int fd = p->fd_read;

    while (!p->terminando) {
        uint32_t netlen;
        ssize_t r = read_all(fd, &netlen, sizeof(netlen));
        if (r <= 0) break;

        uint32_t len = ntohl(netlen);
        char *buf = malloc(len + 1);
        if (!buf) break;

        r = read_all(fd, buf, len);
        if (r <= 0) { free(buf); break; }

        buf[len] = '\0';
        if (p->callback) p->callback(buf, len);
        free(buf);
    }

    return NULL;
}

#endif
