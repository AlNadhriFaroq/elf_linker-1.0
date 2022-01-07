CC=gcc
CFLAGS= -Wall -Werror -g

all: readelf

%.o: %.c
	$(CC) $(CFLAGS) -c $<

readElfHeader.o: readElfHeader.h 
readElfSecTable.o: readElfSecTable.h 
readElfLecSect.o: readElfHeader.h readElfSecTable.h
readElfSym.o: readElfSym.h readElfSecTable.h readElfHeader.h
readElfRel.o: readElfRel.h readElfSecTable.h 
options.o: options.h

readelf: readelf.o readElfHeader.o readElfSecTable.o readElfLecSect.o readElfSym.o readElfRel.o options.o
	$(CC) -o $@ $^

clean:
	rm *.o readelf
