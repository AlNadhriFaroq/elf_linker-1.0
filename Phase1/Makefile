CC=gcc -m32
CFLAGS= -Wall -Werror -g

all: readelf writeelf

%.o: %.c
	$(CC) $(CFLAGS) -c $<

readelfOptions.o: readelfOptions.h
readelfHeader.o: readelfHeader.h 
readelfSectTable.o: readelfHeader.h readelfSectTable.h
readelfSectLect.o: readelfOptions.h readelfHeader.h readelfSectTable.h readelfSectLect.h 
readelfSymbTable.o: readelfHeader.h readelfSectTable.h readelfSymbTable.h
readelfReimpTable.o: readelfHeader.h readelfSectTable.h readelfReimpTable.h

readelf: readelf.o readelfOptions.o readelfHeader.o readelfSectTable.o readelfSectLect.o readelfSymbTable.o readelfReimpTable.o
	$(CC) -o $@ $^

writeelf: writeelf.o readelfHeader.o readelfSectTable.o readelfSectLect.o readelfSymbTable.o readelfReimpTable.o
	$(CC) -o $@ $^

clean:
	rm *.o readelf writeelf > /dev/null
