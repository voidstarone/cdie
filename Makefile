MDIR=model
UDIR=cli
BDIR=bin
ODIR=obj
MTDIR=mtests

CC=clang
CFLAGS=-W -Wall -I$(MDIR)/
CCF=$(CC) $(CFLAGS)

CLIFILES= $(ODIR)/DiceRollingSession.o 

MOFILES= $(ODIR)/numutils.o \
	$(ODIR)/Die.o \
	$(ODIR)/DieFactory.o \
	$(ODIR)/DiceCollectionResults.o \
	$(ODIR)/DiceCollection.o \
	$(ODIR)/DiceNotationInterpreter.o
	
TOFILES=$(ODIR)/TestSuiteDie.o \
	$(ODIR)/TestSuiteDieFactory.o \
	$(ODIR)/TestSuiteDiceCollectionResults.o \
	$(ODIR)/TestSuiteDiceCollection.o \
	$(ODIR)/TestSuiteDiceNotationInterpreter.o 
	

test: $(BDIR)/testdie $(MOFILES) $(TOFILES)
	bin/testdie

# Model Tests
	
$(BDIR)/testdie: $(MOFILES) $(TOFILES)
	$(CCF) -o $(BDIR)/testdie -lcunit $(MTDIR)/TestAll.c $(MOFILES) $(TOFILES)
	
#----

$(ODIR)/*.o: $(ODIR)/*.o
	$(CCF) -c -o $(ODIR)/*.o $(MTDIR)/*.c;

# CLI
build: $(MOFILES) $(CLIFILES) 
	$(CCF) -o $(BDIR)/roll $(UDIR)/main.c $(MOFILES) $(CLIFILES) 
	
# will need  -largp 

# Models
$(ODIR)/*.o: $(ODIR)/*.o
	$(CCF) -c -o $(ODIR)/*.o $(MDIR)/*.c;


clean:
	rm -rf $(ODIR)/*;
	rm -rf **/*.gch;
	rm -rf bin/*;

.PHONY: clean $(BDIR)/testdie
