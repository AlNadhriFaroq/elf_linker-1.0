CC=gcc
CFLAGS= -Wall -Werror

all: readelf

%.o: %.c
	$(CC) $(CFLAGS) -c $<

readElfHeader.o: readElfHeader.h 
readElfSecTable.o: readElfSecTable.h 
readElfLecSect.o: readElfHeader.h readElfSecTable.h
readElfRel.o: readElfRel.h readElfSecTable.h 

readelf: readelf.o readElfHeader.o readElfSecTable.o readElfLecSect.o readElfRel.o
	$(CC) -o $@ $^

clean:
	rm *.o readelf