CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -I../include
LDFLAGS = -lws2_32

OBJS = proceso_par_win.o

libproceso_par.a: $(OBJS)
	ar rcs ../lib/libproceso_par.a $(OBJS)

proceso_par_win.o: proceso_par_win.c ../include/ProcesoPar.h

clean:
	rm -f *.o ../lib/libproceso_par.a
