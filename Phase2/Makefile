CC=gcc -m32
CFLAGS= -Wall -Werror -g

all: renumSect correctSymboles reimpTypeR_ARM_ABS

%.o: %.c
	$(CC) $(CFLAGS) -c $<

renumSect: renumSect.o
	$(CC) -o $@ $^

correctSymboles: correctSymboles.o
	$(CC) -o $@ $^

reimpTypeR_ARM_ABS: reimpTypeR_ARM_ABS.o
	$(CC) -o $@ $^

clean:
	rm *.o renumSect correctSymboles reimpTypeR_ARM_ABS