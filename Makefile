ICC = icc

CFLAGS=-std=c99 -Wall
DFLAGS=-g3
LFLAGS=-lm
IFLAGS = -mkl

NN_FLIST=nn.c 
all: nn filtre O2 mkl

nn: main.o nn.o
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $^ $(LFLAGS) -g

mkl: main.o nn.o
	$(ICC) $(CFLAGS) main.c $(NN_FLIST) -o nn_bench_$@ $(IFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(DFLAGS) -c -o $@ $< $(LFLAGS) 

nn.o : protos.h
main.o : protos.h

filtre: filtre.o nn.o
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(DFLAGS) -c -o $@ $< $(LFLAGS)

filtre.o: protos.h

O2:
	$(CC) $(CFLAGS) $(DFLAGS) -$@ $(NN_FLIST) main.c -o nn_$@ $(LFLAGS)

clean:
	rm -f *.o nn filtre nn_O2 nn_bench_*

.PHONY: clean
