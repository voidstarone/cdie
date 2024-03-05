#include "DiceRollInstructionResult.h"

#ifndef DIE_ROLL_INSTRUCTION_H_FA033981
#define DIE_ROLL_INSTRUCTION_H_FA033981

typedef enum _optype {
	op_type_add = 0,
	op_type_subtract = 1,
	op_type_multiply = 2,
	op_type_divide = 3,
	op_type_sum = 4,
	op_type_mean = 5,
	op_type_avg = 5,
	op_type_max = 6,
	op_type_unknown = -1,
	op_type_number = -2,
	op_type_dice_collection = -3
} OperationType;

typedef struct _DieRollInstruction {
	void *value;
	int num_args;
	OperationType operation_type;
	ResultType expected_result_type;
} DiceRollInstruction;

DiceRollInstruction *dice_roll_instruction_create();
void dice_roll_instruction_free(DiceRollInstruction *dri);
DiceRollInstruction *dice_roll_instruction_clone(DiceRollInstruction *original);
bool dice_roll_instruction_is_equal(DiceRollInstruction *dri1, DiceRollInstruction *dri2);
DiceRollInstruction *dice_roll_instruction_from_string(char *stringRepresentation);
void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType opType);
OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri);
int dice_roll_instruction_get_num_args(DiceRollInstruction *dri);
void dice_roll_instruction_set_expected_result_type(DiceRollInstruction *dri, ResultType result_type);
DiceRollInstructionResult * dice_roll_instruction_do_op(DiceRollInstruction *dri, int argc, DiceRollInstruction **argv);

#endif /*DIE_ROLL_INSTRUCTION_H_FA033981*/
