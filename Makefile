ODIR=$(PWD)

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS = -liio

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

kl15_monitor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

install: kl15_monitor
	install -Dm644 kl15_monitor $(DESTDIR)/usr/bin/kl15_monitor

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core
