CC = gcc
CFLAGS = -Iinclude -Wall -g

SRC = lanzarProcesoPar.c \
      enviarMensajeProcesoPar.c \
      establecerFuncionDeEscucha.c \
      destruirProcesoPar.c



OBJ = $(SRC:.c=.o)

all: lib/libproceso_par.a

lib/libproceso_par.a: $(OBJ)
	ar rcs libproceso_par.a $(OBJ)

clean:
	rm -f src/*.o lib/*.a
