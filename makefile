CC = gcc
CFLAGS = -D_XOPEN_SOURCE=700 -Wall -pedantic -std=c99
LIBS = -L./bib/ -lSocketUDP
FILES = *.o *.*~ serveur_tftp client_tftp *.h.gch

all : client_tftp serveur_tftp 

tftp.o : tftp.c tftp.h
	$(CC) -c $(CFLAGS) $^
utils.o : utils.c utils.h tftp.h
	$(CC) -c -fPIC $(CFLAGS) $(LIBS) $^
client_tftp.o : client_tftp.c client_tftp.h tftp.h utils.h
	$(CC) -c $(CFLAGS) $^
serveur_tftp.o : serveur_tftp.c serveur_tftp.h tftp.h utils.h
	$(CC) -c $(CFLAGS) $^
client_tftp : client_tftp.o tftp.o utils.o
	$(CC) $^ $(LIBS) -o $@  
serveur_tftp : serveur_tftp.o tftp.o utils.o
	$(CC) $^ $(LIBS) -o $@
clean : 
	rm -f $(FILES)