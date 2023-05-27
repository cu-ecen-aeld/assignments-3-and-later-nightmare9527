CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall -Werror
LDFLAGS= -pthread -lrt

all : aesdsocket

default : aesdsocket


aesdsocket : aesdsocket.o $(LDFLAGS)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean :
	rm -f aesdsocket.o
