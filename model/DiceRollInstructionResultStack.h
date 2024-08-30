#include "DynArray.h"
#include "DiceRollInstruction.h"

#ifndef DIE_ROLL_INSTRUCTION_RESULT_STACK_H_FA033981
#define DIE_ROLL_INSTRUCTION_RESULT_STACK_H_FA033981


typedef struct {
	DynArray *results;
} DiceRollInstructionResultStack;

DiceRollInstructionResultStack *dice_roll_instruction_result_stack_create(size_t size);
void dice_roll_instruction_result_stack_free(DiceRollInstructionResultStack *drirs);

void dice_roll_instruction_result_stack_push(DiceRollInstructionResultStack *drirs, DiceRollInstructionResult *drir);
DiceRollInstructionResult * dice_roll_instruction_result_stack_peek(DiceRollInstructionResultStack *drirs);
DiceRollInstructionResult * dice_roll_instruction_result_stack_pop(DiceRollInstructionResultStack *drirs);

//DiceRollInstructionResult * dice_roll_instruction_do_op(DiceRollInstruction *dri, DynArray *argv);
DiceRollInstruction * dice_roll_instruction_do_op(DiceRollInstruction *dri, DynArray *argv);


#endif /*DIE_ROLL_INSTRUCTION_RESULT_STACK_H_FA033981*/
