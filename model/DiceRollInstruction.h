
#ifndef DIE_ROLL_INSTRUCTION_H_FA033981
#define DIE_ROLL_INSTRUCTION_H_FA033981

typedef enum {
	add = 0,
	subtract = 1,
	sum = 2,
	mean = 3,
	max = 4,
	unknown = -1,
	number = -2,
	dice_collection = -3
} OperationType;

typedef enum {
	intResult,
	diceCollectionResult
} ResultType;

typedef struct _DieRollInstruction {
	void *value;
	int numArgs;
	OperationType operation_type;
} DiceRollInstruction;

typedef struct _DieRollInstructionResult {
	ResultType type;
	void *resultValue;
} DiceRollInstructionResult;

DiceRollInstruction *dice_roll_instruction_init();

void dice_roll_instruction_free(DiceRollInstruction *dri);
DiceRollInstruction *dice_roll_instruction_from_string(char *stringRepresentation);
void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType opType);
OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri);

#endif /*DIE_ROLL_INSTRUCTION_H_FA033981*/
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