#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

static ssize_t read_all(int fd, void *buf, size_t count) {
    uint8_t *p = buf;
    size_t left = count;
    while (left > 0) {
        ssize_t r = read(fd, p, left);
        if (r == 0) return (ssize_t)(count - left); // EOF
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        left -= r;
        p += r;
    }
    return count;
}

static ssize_t write_all(int fd, const void *buf, size_t count) {
    const uint8_t *p = buf;
    size_t left = count;
    while (left > 0) {
        ssize_t w = write(fd, p, left);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        left -= w;
        p += w;
    }
    return count;
}

int main() {

    while (1) {
        uint32_t netlen;
        ssize_t r = read(STDIN_FILENO, &netlen, sizeof(netlen));
        if (r == 0) break;   // EOF: padre cerró
        if (r < 0) break;

        uint32_t len = ntohl(netlen);
        char buf[4096];

        if (len > sizeof(buf)-1) len = sizeof(buf)-1;

        ssize_t got = read(STDIN_FILENO, buf, len);
        if (got <= 0) break;

        buf[len] = '\0';

        // Mensaje que recibió el hijo
        fprintf(stderr, "[Hijo] Recibí: %s\n", buf);

        // Respuesta automática
        char respuesta[500];
        int rl = snprintf(respuesta, sizeof(respuesta),
                          "ACK: %s", buf);

        uint32_t rlen = htonl(rl);
        write(STDOUT_FILENO, &rlen, sizeof(rlen));
        write(STDOUT_FILENO, respuesta, rl);
    }

    return 0;
}
