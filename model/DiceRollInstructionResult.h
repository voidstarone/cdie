#include "DiceCollection.h"

#ifndef DIE_ROLL_INSTRUCTION_RESULT_H_FA033981
#define DIE_ROLL_INSTRUCTION_RESULT_H_FA033981


typedef enum {
	result_type_void,
	result_type_double,
	result_type_dice_collection
} ResultType;

typedef struct _DieRollInstructionResult {
	ResultType type;
	void *result_value;
} DiceRollInstructionResult;


void dice_roll_instruction_result_free(DiceRollInstructionResult *drir);
DiceRollInstructionResult *dice_roll_instruction_result_create();
DiceRollInstructionResult *dice_roll_instruction_result_with_double(double value);
double dice_roll_instruction_result_value_as_double(DiceRollInstructionResult *drir);
DiceRollInstructionResult *dice_roll_instruction_result_with_dice_collection(DiceCollection *dc);
double dice_roll_instruction_result_get_number(DiceRollInstructionResult *drir);

#endif /*DIE_ROLL_INSTRUCTION_RESULT_H_FA033981*/
