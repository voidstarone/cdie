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
	$(ODIR)/DiceNotationInterpreter.o \
	$(ODIR)/DiceRollInstruction.o \
	$(ODIR)/DiceRollInstructionStack.o
	
TOFILES=$(ODIR)/TestSuiteDie.o \
	$(ODIR)/TestSuiteDieFactory.o \
	$(ODIR)/TestSuiteDiceCollectionResults.o \
	$(ODIR)/TestSuiteDiceCollection.o \
	$(ODIR)/TestSuiteDiceNotationInterpreter.o \
	$(ODIR)/TestSuiteDiceRollInstruction.o \
	$(ODIR)/TestSuiteDiceRollInstructionStack.o 
	

test: $(BDIR)/testdie $(MOFILES) $(TOFILES)
	bin/testdie

# Model Tests
	
$(BDIR)/testdie: $(MOFILES) $(TOFILES)
	$(CCF) -o $(BDIR)/testdie -lcunit -I$(MDIR)/DiceDoodads.h $(MTDIR)/TestAll.c $(MOFILES) $(TOFILES)
	
#----

$(ODIR)/TestSuiteDiceRollInstructionStack.o: $(ODIR)/DiceRollInstructionStack.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceRollInstructionStack.o $(MTDIR)/TestSuiteDiceRollInstructionStack.c;

$(ODIR)/TestSuiteDiceRollInstruction.o: $(ODIR)/DiceRollInstruction.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceRollInstruction.o $(MTDIR)/TestSuiteDiceRollInstruction.c;

$(ODIR)/TestSuiteDiceNotationInterpreter.o: $(ODIR)/DiceNotationInterpreter.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceNotationInterpreter.o $(MTDIR)/TestSuiteDiceNotationInterpreter.c;

$(ODIR)/TestSuiteDiceCollection.o: $(ODIR)/DiceCollection.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceCollection.o $(MTDIR)/TestSuiteDiceCollection.c;
	
$(ODIR)/TestSuiteDiceCollectionResults.o: $(ODIR)/DiceCollectionResults.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceCollectionResults.o $(MTDIR)/TestSuiteDiceCollectionResults.c;

$(ODIR)/TestSuiteDieFactory.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/TestSuiteDieFactory.o $(MTDIR)/TestSuiteDieFactory.c;

$(ODIR)/TestSuiteDie.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/TestSuiteDie.o $(MTDIR)/TestSuiteDie.c;

# CLI
build: $(MOFILES) $(CLIFILES) 
	$(CCF) -o $(BDIR)/roll $(UDIR)/main.c $(MOFILES) $(CLIFILES) 
	
# will need  -largp 

$(ODIR)/DiceRollingSession.o:
	$(CCF) -c -o $(ODIR)/DiceRollingSession.o $(MDIR)/DiceRollingSession.c;

# Models
$(ODIR)/DiceRollInstructionStack.o: 
	$(CCF) -c -o $(ODIR)/DiceRollInstructionStack.o $(MDIR)/DiceRollInstructionStack.c;

$(ODIR)/DiceRollInstruction.o: 
	$(CCF) -c -o $(ODIR)/DiceRollInstruction.o $(MDIR)/DiceRollInstruction.c;

$(ODIR)/DiceNotationInterpreter.o: 
	$(CCF) -c -o $(ODIR)/DiceNotationInterpreter.o $(MDIR)/DiceNotationInterpreter.c;

$(ODIR)/DiceCollection.o: $(ODIR)/DieFactory.o
	$(CCF) -c -o $(ODIR)/DiceCollection.o $(MDIR)/DiceCollection.c;

$(ODIR)/DiceCollectionResults.o:
	$(CCF) -c -o $(ODIR)/DiceCollectionResults.o $(MDIR)/DiceCollectionResults.c;

$(ODIR)/DieFactory.o: $(ODIR)/Die.o
	$(CCF) -c -o $(ODIR)/DieFactory.o $(MDIR)/DieFactory.c;

$(ODIR)/Die.o: $(MDIR)/Die.c
	$(CCF) -c -o $(ODIR)/Die.o $(MDIR)/Die.c;

$(ODIR)/numutils.o: $(MDIR)/numutils.c
	$(CCF) -c -o $(ODIR)/numutils.o $(MDIR)/numutils.c;


clean:
	rm -rf $(ODIR)/*;
	rm -rf **/*.gch;
	rm -rf bin/*;

.PHONY: clean $(BDIR)/testdie
