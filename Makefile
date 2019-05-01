MDIR=model
UDIR=cli
BDIR=bin
ODIR=obj
MTDIR=mtests

CC=clang
CFLAGS=-W -Wall -I$(MDIR)/
CCF=$(CC) $(CFLAGS)

MOFILES= $(ODIR)/Die.o \
	$(ODIR)/DieFactory.o \
	$(ODIR)/DiceCollectionResults.o \
	$(ODIR)/DiceCollection.o \
	$(ODIR)/DiceNotation.o
	
TOFILES=$(ODIR)/TestSuiteDie.o \
	$(ODIR)/TestSuiteDieFactory.o \
	$(ODIR)/TestSuiteDiceCollectionResults.o \
	$(ODIR)/TestSuiteDiceCollection.o \
	$(ODIR)/TestSuiteDiceNotation.o 
	

test: $(BDIR)/testdie $(MOFILES) $(TOFILES)
	bin/testdie

# Model Tests
	
$(BDIR)/testdie: $(MOFILES) $(TOFILES)
	$(CCF) -o $(BDIR)/testdie -lcunit $(MTDIR)/TestAll.c $(MOFILES) $(TOFILES)
				

$(ODIR)/TestSuiteDiceNotation.o: $(ODIR)/TestSuiteDiceCollection.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceNotation.o $(MTDIR)/TestSuiteDiceNotation.c;

$(ODIR)/TestSuiteDiceCollection.o: $(ODIR)/DiceCollection.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceCollection.o $(MTDIR)/TestSuiteDiceCollection.c;
	
$(ODIR)/TestSuiteDiceCollectionResults.o: $(ODIR)/DiceCollectionResults.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceCollectionResults.o $(MTDIR)/TestSuiteDiceCollectionResults.c;

$(ODIR)/TestSuiteDieFactory.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/TestSuiteDieFactory.o $(MTDIR)/TestSuiteDieFactory.c;

$(ODIR)/TestSuiteDie.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/TestSuiteDie.o $(MTDIR)/TestSuiteDie.c;

# CLI
build: $(MOFILES)
	$(CCF) -o $(BDIR)/roll -largp $(UDIR)/main.c $(MOFILES)


# Models
$(ODIR)/DiceNotation.o: $(ODIR)/DiceCollection.o
	$(CCF) -c -o $(ODIR)/DiceNotation.o $(MDIR)/DiceNotation.c;

$(ODIR)/DiceCollection.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/DiceCollection.o $(MDIR)/DiceCollection.c;

$(ODIR)/DiceCollectionResults.o:
	$(CCF) -c -o $(ODIR)/DiceCollectionResults.o $(MDIR)/DiceCollectionResults.c;

$(ODIR)/DieFactory.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/DieFactory.o $(MDIR)/DieFactory.c;

$(ODIR)/Die.o: $(MDIR)/Die.c
	$(CCF) -c -o $(ODIR)/Die.o $(MDIR)/Die.c;


clean:
	rm -rf $(ODIR)/*;
	rm -rf **/*.gch;
	rm -rf bin/*;

.PHONY: clean $(BDIR)/testdie
