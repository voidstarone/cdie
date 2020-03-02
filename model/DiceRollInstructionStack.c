#include <stdlib.h>
#include <stdio.h>

#include "DiceRollInstructionStack.h"

DiceRollInstructionStack *dice_roll_instruction_stack_init() {
	DiceRollInstructionStack *dris = malloc(sizeof(DiceRollInstructionStack));
	dris->first_instruction = NULL;
	return dris;
}

void dice_roll_instruction_stack_free(DiceRollInstructionStack *dris) {
	free(dris);
}