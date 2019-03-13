MDIR=model
BDIR=bin
ODIR=obj
MTDIR=mtests

CC=clang
CFLAGS=-Wall -I$(MDIR)/
CCF=$(CC) $(CFLAGS)

OFILES= $(ODIR)/Die.o \
	$(ODIR)/DieFactory.o \
	$(ODIR)/DiceCollection.o \
	$(ODIR)/DiceNotation.o
	
TFILES=$(ODIR)/TestSuiteDie.o \
	$(ODIR)/TestSuiteDieFactory.o \
	$(ODIR)/TestSuiteDiceCollection.o \
	$(ODIR)/TestSuiteDiceNotation.o 

test: $(BDIR)/testdie
	$(BDIR)/testdie
	
$(BDIR)/testdie: $(OFILES) $(TFILES)
	$(CCF) -o $(BDIR)/testdie -lcunit $(MTDIR)/TestAll.c $(OFILES) $(TFILES)
				

$(ODIR)/TestSuiteDiceNotation.o: $(ODIR)/TestSuiteDiceCollection.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceNotation.o $(MTDIR)/TestSuiteDiceNotation.c;

$(ODIR)/TestSuiteDiceCollection.o: $(ODIR)/DiceCollection.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceCollection.o $(MTDIR)/TestSuiteDiceCollection.c;

$(ODIR)/TestSuiteDieFactory.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/TestSuiteDieFactory.o $(MTDIR)/TestSuiteDieFactory.c;

$(ODIR)/TestSuiteDie.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/TestSuiteDie.o $(MTDIR)/TestSuiteDie.c;


# Models
$(ODIR)/DiceNotation.o: $(ODIR)/DiceCollection.o
	$(CCF) -c -o $(ODIR)/DiceNotation.o $(MDIR)/DiceNotation.c;

$(ODIR)/DiceCollection.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/DiceCollection.o $(MDIR)/DiceCollection.c;

$(ODIR)/DieFactory.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/DieFactory.o $(MDIR)/DieFactory.c;

$(ODIR)/Die.o: $(MDIR)/Die.c
	$(CCF) -c -o $(ODIR)/Die.o $(MDIR)/Die.c;


clean:
	rm -rf $(ODIR)/*;
	rm -rf **/*.gch;
	rm -rf bin/*;

.PHONY: clean $(BDIR)/testdie
