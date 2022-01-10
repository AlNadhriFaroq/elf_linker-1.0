CC=gcc -m32
CFLAGS= -Wall -Werror -g

all: renumSect correctSymboles reimpTypeR_ARM_ABS

%.o: %.c
	$(CC) $(CFLAGS) -c $<

renumSect.o: ../Phase1/readelfOptions.h ../Phase1/readelfHeader.h ../Phase1/readelfSectTable.h ../Phase1/readelfSectLect.h 

renumSect: renumSect.o ../Phase1/readelfOptions.o ../Phase1/readelfHeader.o ../Phase1/readelfSectTable.o ../Phase1/readelfSectLect.o
	$(CC) -o $@ $^

correctSymboles: correctSymboles.o
	$(CC) -o $@ $^

reimpTypeR_ARM_ABS: reimpTypeR_ARM_ABS.o
	$(CC) -o $@ $^

clean:
	rm *.o renumSect correctSymboles reimpTypeR_ARM_ABS