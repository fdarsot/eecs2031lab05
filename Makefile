CC=gcc
CFLAGS=-lWarn -pedantic

irtester: irtester.o libmyifttt.a
	$(CC) irtester.o -L. -lmyifttt -lcurl -lwiringPi -o irtester


blink: blink.o 
	cc blink.o -lwiringPi -o blink

button: button.o 
	cc button.o -lwiringPi -o button

libmyifttt.a: ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

irtester.o:	irtester.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

all: irtester blink button

clean:
	rm  irtester *.o
