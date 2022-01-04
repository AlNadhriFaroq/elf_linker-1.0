CC=gcc
CFLAGS= -Wall -Werror

all: readelf

%.o: %.c
	$(CC) $(CFLAGS) -c $<

readElfHeader.o: readElfHeader.h 
readElfSecTable.o: readElfSecTable.h 

readelf: readelf.o readElfHeader.o readElfSecTable.o
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm *.o readelf