#include "DiceRollInstruction.h"

#ifndef DIE_ROLL_INSTRUCTION_RESULT_STACK_H_FA033981
#define DIE_ROLL_INSTRUCTION_RESULT_STACK_H_FA033981


typedef struct {
	DiceRollInstructionResult **results;
	size_t size;
	size_t count;
} DiceRollInstructionResultStack;

DiceRollInstructionResultStack *dice_roll_instruction_result_stack_create();
void dice_roll_instruction_result_stack_free(DiceRollInstructionResultStack *drirs);

void dice_roll_instruction_result_stack_push(DiceRollInstructionResultStack *drirs, DiceRollInstructionResult *drir);
DiceRollInstructionResult * dice_roll_instruction_result_stack_peek(DiceRollInstructionResultStack *drirs);
DiceRollInstructionResult * dice_roll_instruction_result_stack_pop(DiceRollInstructionResultStack *drirs);

#endif /*DIE_ROLL_INSTRUCTION_RESULT_STACK_H_FA033981*/