CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall -Werror

all : aesdsocket

default : aesdsocket


aesdsocket : aesdsocket.o
	$(CC) -o $@ $^
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean :
	rm -f aesdsocket.o
