CC=clang
CFLAGS= -Wall -Werror
all:readelf

%.o:%.c
	$(CC) $(CFLAGS) -c $<
readElfHeader.o:readElfHeader.h 
readElfSecTable.o:readElfSecTable.h 
readElfRel.o:readElfRel.h readElfSecTable.h 
readElfLecSect.o:readElfHeader.h readElfSecTable.h

readelf: readelf.o readElfHeader.o readElfSecTable.o readElfRel.o readElfLecSect.o
	$(CC) -o $@ $^

clean:
	rm *.o readelf