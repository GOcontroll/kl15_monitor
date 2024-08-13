_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CFLAGS += -DVERSION="1.1.1"

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kl15_monitor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~