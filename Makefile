MDIR=model
UDIR=cli
BDIR=bin
ODIR=obj
MTDIR=mtests

all: build

CFLAGS=-I$(MDIR)/

CC=clang
CCF=$(CC) $(CFLAGS)

CLIFILES= $(ODIR)/DiceRollingSession.o 

MOFILES= $(ODIR)/DynArray.o \
	$(ODIR)/numutils.o \
	$(ODIR)/Die.o \
	$(ODIR)/DieFactory.o \
	$(ODIR)/DiceCollectionResults.o \
	$(ODIR)/DiceCollection.o \
	$(ODIR)/DiceNotationInterpreter.o \
	$(ODIR)/DiceRollInstructionResult.o \
	$(ODIR)/DiceRollInstruction.o \
	$(ODIR)/DiceRollInstructionResultStack.o \
	$(ODIR)/DiceRollInstructionStack.o \
	$(ODIR)/ExpressionToDiceRollInstructionStack.o
	
TOFILES= $(ODIR)/TestSuiteDynArray.o \
	$(ODIR)/TestSuiteDie.o \
	$(ODIR)/TestSuiteDieFactory.o \
	$(ODIR)/TestSuiteDiceCollectionResults.o \
	$(ODIR)/TestSuiteDiceCollection.o \
	$(ODIR)/TestSuiteDiceNotationInterpreter.o \
	$(ODIR)/TestSuiteDiceRollInstructionResult.o \
	$(ODIR)/TestSuiteDiceRollInstruction.o \
	$(ODIR)/TestSuiteDiceRollInstructionStack.o \
	$(ODIR)/TestSuiteExpressionToDiceRollInstructionStack.o 

# Model Tests
	
$(BDIR)/testdie: $(MOFILES) $(TOFILES)
	$(CCF) -o $(BDIR)/testdie -lcunit -I$(MDIR)/DiceDoodads.h $(MTDIR)/TestAll.c $(MOFILES) $(TOFILES)
	
#----

$(ODIR)/TestSuiteExpressionToDiceRollInstructionStack.o: $(ODIR)/ExpressionToDiceRollInstructionStack.o
	$(CCF) -c -o $(ODIR)/TestSuiteExpressionToDiceRollInstructionStack.o $(MTDIR)/TestSuiteExpressionToDiceRollInstructionStack.c

$(ODIR)/TestSuiteDiceRollInstructionStack.o: $(ODIR)/DiceRollInstructionStack.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceRollInstructionStack.o $(MTDIR)/TestSuiteDiceRollInstructionStack.c;

$(ODIR)/TestSuiteDiceRollInstruction.o: $(ODIR)/DiceRollInstruction.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceRollInstruction.o $(MTDIR)/TestSuiteDiceRollInstruction.c;

$(ODIR)/TestSuiteDiceRollInstructionResult.o: $(ODIR)/DiceRollInstructionResult.o
	$(CCF) -c -o $(ODIR)/TestSuiteDiceRollInstructionResult.o $(MTDIR)/TestSuiteDiceRollInstructionResult.c;

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

$(ODIR)/TestSuiteDynArray.o: $(ODIR)/DynArray.o 
	$(CCF) -c -o $(ODIR)/TestSuiteDynArray.o $(MTDIR)/TestSuiteDynArray.c

$(ODIR)/DiceRollingSession.o:
	$(CCF) -c -o $(ODIR)/DiceRollingSession.o $(MDIR)/DiceRollingSession.c;

# Models

$(ODIR)/ExpressionToDiceRollInstructionStack.o:
	$(CCF) -c -o $(ODIR)/ExpressionToDiceRollInstructionStack.o $(MDIR)/ExpressionToDiceRollInstructionStack.c;
	
$(ODIR)/DiceRollInstructionStack.o: 
	$(CCF) -c -o $(ODIR)/DiceRollInstructionStack.o $(MDIR)/DiceRollInstructionStack.c;

$(ODIR)/DiceRollInstructionResultStack.o: 
	$(CCF) -c -o $(ODIR)/DiceRollInstructionResultStack.o $(MDIR)/DiceRollInstructionResultStack.c;

$(ODIR)/DiceRollInstruction.o: 
	$(CCF) -c -o $(ODIR)/DiceRollInstruction.o $(MDIR)/DiceRollInstruction.c;

$(ODIR)/DiceRollInstructionResult.o: 
	$(CCF) -c -o $(ODIR)/DiceRollInstructionResult.o $(MDIR)/DiceRollInstructionResult.c;

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
	
$(ODIR)/DynArray.o: 
	$(CCF) -c -o $(ODIR)/DynArray.o $(MDIR)/DynArray.c

$(ODIR)/numutils.o: $(MDIR)/numutils.c
	$(CCF) -c -o $(ODIR)/numutils.o $(MDIR)/numutils.c;


# CLI
build: CCF+= -O 
build: $(MOFILES) $(CLIFILES) 
	$(CCF) -largp -o $(BDIR)/roll $(UDIR)/main.c $(MOFILES) $(CLIFILES) 

test: CCF+= -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -W -Wall -g 
test: $(BDIR)/testdie $(MOFILES) $(TOFILES)
	bin/testdie

debugtest: $(BDIR)/testdie $(MOFILES) $(TOFILES)
	lldb bin/testdie

clean:
	rm -rf $(ODIR)/*;
	rm -rf **/*.gch;
	rm -rf bin/*;

.PHONY: clean $(BDIR)/testdie
