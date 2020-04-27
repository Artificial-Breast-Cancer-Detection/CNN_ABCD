CC = gcc
CFLAGS = -o2
LDFLAGS = -lm

all :prog

prog:	nn.o	main.o
	$(CC)	$(CFLAGS)	-o $@ $^	-lcmocka	$(LDFLAGS)

nn.o:	nn.c	protos.h
	$(CC)	$(CFLAGS)	-c	-o	$@	$<	$(LDFLAGS)

main.o:	main.c
	$(CC)	$(CFLAGS)	-c	-o	$@	$<

clean:
	rm	-f	*.o

.PHONY:	clean
