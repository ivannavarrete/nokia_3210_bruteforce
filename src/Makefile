
CC = gcc
CFLAGS = -Wall -O2
MAKE = make

OBJFILES = serial.o sfile.o install.o main.o

all: $(OBJFILES)
	gcc $(CFLAGS) $(OBJFILES) -o nokia
	@cd ./hc11; $(MAKE) all;

clean:
	rm -f *.o nokia;
	@cd ./hc11; $(MAKE) clean;
