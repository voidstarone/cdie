
#ifndef DIE_ROLL_INSTRUCTION_H_FA033981
#define DIE_ROLL_INSTRUCTION_H_FA033981

typedef enum {
	op_type_add = 0,
	op_type_subtract = 1,
	op_type_sum = 2,
	op_type_mean = 3,
	op_type_avg = 3,
	op_type_max = 4,
	op_type_unknown = -1,
	op_type_number = -2,
	op_type_dice_collection = -3
} OperationType;

typedef enum {
	result_type_double,
	result_type_dice_collection
} ResultType;

typedef struct _DieRollInstruction {
	void *value;
	int numArgs;
	OperationType operation_type;
	ResultType expected_result_type;
} DiceRollInstruction;

typedef struct _DieRollInstructionResult {
	ResultType type;
	void *result_value;
} DiceRollInstructionResult;

DiceRollInstruction *dice_roll_instruction_init();

void dice_roll_instruction_free(DiceRollInstruction *dri);
DiceRollInstruction *dice_roll_instruction_from_string(char *stringRepresentation);
void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType opType);
OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri);
void dice_roll_instruction_set_expected_result_type(DiceRollInstruction *dri, ResultType result_type);
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
--
20 + sum(2d6, 4d20, 1d4)
DRIS[DRI(+), DRI(20), DRI("sum"), DRI(2d6), DRI(4d20), DRI(1d4)] =>
int
*/