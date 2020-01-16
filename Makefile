CC=gcc
CFLAGS=-o2
LFLAGS=-lm

all:prog nn filtre

prog: main.o nn.o
	$(CC) $(CFLAGS) -o $@ $^	$(LFLAGS)	-lcmocka

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<	-lcmocka

nn.o : protos.h
main.o : protos.h

filtre: filtre.o nn.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)	-lcmocka

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

filtre.o: protos.h

clean:
	rm -f *.o nn filtre

.PHONY: clean
