#CROSS_COMPILE=

all:    writer
        

writer.o: writer.c
	$(CROSS_COMPILE)gcc -g -Wall   -c -o writer.o writer.c
        

clean:
	rm -f *.o writer *.elf *.map

