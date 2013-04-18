CC = gcc
CFLAGS = -D_XOPEN_SOURCE=700 -Wall -pedantic -std=c99
LDFLAGS = -shared
FILES = libSocketUDP.so SocketUDP.h.gch .*~ *~ main 
LIBS = -L. -lSocketUDP 

all : libSocketUDP.so main

SocketUDP.o : SocketUDP.c SocketUDP.h
	$(CC) -c $? $(CFLAGS) -fPIC
libSocketUDP.so : SocketUDP.o
	$(CC) -o $@ $(LDFLAGS) $?
main.o : main.c SocketUDP.h
	$(CC) -c $(CFLAGS) $^
main : main.o
	$(CC) $< $(LIBS) -o $@
clean :
	rm -f *.o $(FILES)