

enum InstructionType {
	operation,
	data
}

enum ResultType {
	intResult,
	diceCollectionResult
}

typedef struct _DieRollInstruction {
	void *value;
	int numArgs;
	InstructonType instructionType;
} DieRollInstruction;

typedef struct _DieRollInstructionResult {
	ResultType type;
	void *resultValue;
} DieRollInstructionResult;

/*
2d6 + 4d20 =>
DRIS[DRI(+), DRI(2d6), DRI(4d20)] => 
int
--
2d6 + 4d20 1d100 =>
DRIS[DRI(+), DRI(2d6), DRI(4d20), DRI(1d100)] => 
int, int
--
2d6 + 4d20 2d100 =>
DRIS[DRI(+), DRI(2d6), DRI(4d20), DRI(2d100)] => 
int, int, int
--
1 + 4d20 =>
DRIS[DRI(+), DRI(1), DRI(4d20)] => 
int
--
2d6 - 4d20 =>
DRIS[DRI(-), DRI(2d6), DRI(4d20)] => 
int
--
sum(2d6, 4d20, 1d4)
DRIS[DRI("sum"), DRI(2d6), DRI(4d20), DRI(1d4)] =>
int
--
mean(2d6, 4d20, 1d4)
DRIS[DRI("mean"), DRI(2d6), DRI(4d20), DRI(1d4)] =>
int
*/