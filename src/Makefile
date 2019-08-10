IDIR=../include
CC=gcc
CFLAGS=-W -Wall -g -fsanitize=address -I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS =

_DEPS = nes.h util.h cpu.h ops.h dasm.h ppu.h apu.h cart.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ  = nes.o util.o cpu.o ops.o dasm.o ppu.o apu.o cart.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

emu: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~