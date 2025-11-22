// windows/src/eco_win.c
#define _WIN32_WINNT 0x0601
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <winsock2.h>   // htonl, ntohl
#include <io.h>
#include <fcntl.h>

static int read_exact(FILE *f, void *buf, size_t n) {
    unsigned char *p = (unsigned char*)buf;
    while (n > 0) {
        size_t r = fread(p, 1, n, f);
        if (r == 0) return 0; // EOF o error
        p += r;
        n -= r;
    }
    return 1;
}

static int write_all(FILE *f, const void *buf, size_t n) {
    const unsigned char *p = (const unsigned char*)buf;
    while (n > 0) {
        size_t w = fwrite(p, 1, n, f);
        if (w == 0) return 0;
        p += w;
        n -= w;
    }
    fflush(f);
    return 1;
}

int main(void) {
    // stdin/stdout en binario para pipes
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);

    while (1) {
        uint32_t len_net;
        if (!read_exact(stdin, &len_net, sizeof(len_net))) break;

        uint32_t len = ntohl(len_net);
        if (len == 0) continue;

        char *msg = (char*)malloc(len + 1);
        if (!msg) break;

        if (!read_exact(stdin, msg, len)) {
            free(msg);
            break;
        }
        msg[len] = '\0';

        for (uint32_t i = 0; i < len; ++i) {
            msg[i] = (char)toupper((unsigned char)msg[i]);
        }

        uint32_t out_len_net = htonl(len);
        if (!write_all(stdout, &out_len_net, sizeof(out_len_net))) {
            free(msg);
            break;
        }
        if (!write_all(stdout, msg, len)) {
            free(msg);
            break;
        }

        free(msg);
    }
    return 0;
}
