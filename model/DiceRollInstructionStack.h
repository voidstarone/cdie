#include "DynArray.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionResultStack.h"

#ifndef DICE_ROLL_INSTRUCTION_STACK_H_FA033981
#define DICE_ROLL_INSTRUCTION_STACK_H_FA033981

typedef struct {
	DynArray *instructions;
} DiceRollInstructionStack;

DiceRollInstructionStack *dice_roll_instruction_stack_create();
void dice_roll_instruction_stack_free(DiceRollInstructionStack *dris);

void dice_roll_instruction_stack_push(DiceRollInstructionStack *dris, DiceRollInstruction *dri);
DiceRollInstruction * dice_roll_instruction_stack_peek(DiceRollInstructionStack *dris);
DiceRollInstruction * dice_roll_instruction_stack_pop(DiceRollInstructionStack *dris);

int dice_roll_instruction_stack_evaluate(DiceRollInstructionStack *dris, DiceRollInstructionResultStack *drirs);
#endif /*DICE_ROLL_INSTRUCTION_STACK_H_FA033981*/
