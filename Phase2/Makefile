CC=gcc -m32
CFLAGS= -Wall -Werror -g

all: reimplantation

%.o: %.c
	$(CC) $(CFLAGS) -c $<

reimpRenumSect.o: reimpRenumSect.h ../Phase1/readelfHeader.o ../Phase1/readelfSectTable.o ../Phase1/readelfSectLect.o
reimpCorrectSymb.o: reimpCorrectSymb.h ../Phase1/readelfHeader.o ../Phase1/readelfSectTable.o ../Phase1/readelfSectLect.o ../Phase1/readelfSymbTable.o
reimpTypeAbs.o: reimpTypeAbs.h ../Phase1/readelfHeader.o ../Phase1/readelfSectTable.o ../Phase1/readelfSectLect.o ../Phase1/readelfSymbTable.o ../Phase1/readelfReimpTable.o
reimpTypeJumpCall.o: reimpTypeJumpCall.h ../Phase1/readelfHeader.o ../Phase1/readelfSectTable.o ../Phase1/readelfSectLect.o ../Phase1/readelfSymbTable.o ../Phase1/readelfReimpTable.o
#interfaceSimul.o: interfaceSimul.h
#productionExec.o: productionExec.h

reimplantation: reimplantation.o ../Phase1/readelfHeader.o ../Phase1/readelfSectTable.o ../Phase1/readelfSectLect.o ../Phase1/readelfSymbTable.o ../Phase1/readelfReimpTable.o reimpRenumSect.o reimpCorrectSymb.o reimpTypeAbs.o reimpTypeJumpCall.o
	$(CC) -o $@ $^

clean:
	rm *.o reimplantation outFile >/dev/null
