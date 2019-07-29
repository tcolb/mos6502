IDIR=../include
CC=gcc
CFLAGS=-W -Wall -g -I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS =

_DEPS = instructions.h disassemble.h ines.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mos6502.o instructions.o disassemble.o ines.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mos6502: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~