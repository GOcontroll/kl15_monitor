IDIR =$(PWD)/lib
CC=aarch64-none-linux-gnu-gcc
CFLAGS=-I$(IDIR)

ODIR=$(PWD)
LDIR =$(PWD)/lib

LIBS=$(PWD)/lib/libiio.a

_DEPS = iio.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kl15_monitor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
