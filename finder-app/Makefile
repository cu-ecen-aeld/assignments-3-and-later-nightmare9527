#CROSS_COMPILE=

all:    writer
        
writer:writer.o
	$(CROSS_COMPILE)gcc -g -Wall -I/ writer.o -o writer

        
clean:
	rm -f *.o writer *.elf *.map

