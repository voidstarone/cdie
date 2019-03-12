SDIR=src
BDIR=bin
ODIR=obj
TDIR=tests

CC=clang
CFLAGS=-Wall -I$(SDIR)
CCF=$(CC) $(CFLAGS)


DEPS = Die.h

test: $(BDIR)/testdie
	$(BDIR)/testdie
	
$(BDIR)/testdie: $(ODIR)/TestSuiteDie.o \
				 $(ODIR)/TestSuiteDieFactory.o \
				 $(ODIR)/TestSuiteDiceCollection.o
	$(CCF) -o $(BDIR)/testdie -lcunit $(TDIR)/TestAll.c $(ODIR)/TestSuiteDie.o \
	 $(ODIR)/TestSuiteDieFactory.o $(ODIR)/TestSuiteDiceCollection.o \
	 $(ODIR)/Die.o $(ODIR)/DieFactory.o $(ODIR)/DiceCollection.o;

$(ODIR)/TestSuiteDiceCollection.o: $(ODIR)/DiceCollection.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceCollection.o $(TDIR)/TestSuiteDiceCollection.c;

$(ODIR)/TestSuiteDieFactory.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/TestSuiteDieFactory.o $(TDIR)/TestSuiteDieFactory.c;

$(ODIR)/TestSuiteDie.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/TestSuiteDie.o $(TDIR)/TestSuiteDie.c;


# Models

$(ODIR)/DiceCollection.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/DiceCollection.o $(SDIR)/DiceCollection.c;

$(ODIR)/DieFactory.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/DieFactory.o $(SDIR)/DieFactory.c;

$(ODIR)/Die.o: $(SDIR)/Die.c
	$(CCF) -c -o $(ODIR)/Die.o $(SDIR)/Die.c;


clean:
	rm -rf $(ODIR)/*;
	rm -rf **/*.gch;
	rm -rf bin/*;

.PHONY: clean $(BDIR)/testdie
