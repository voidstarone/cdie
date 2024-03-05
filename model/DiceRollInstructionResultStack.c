#include <stdlib.h>
#include <stdio.h>

#include "DynArray.h"
#include "DiceRollInstructionResult.h"
#include "DiceRollInstructionResultStack.h"

DiceRollInstructionResultStack *dice_roll_instruction_result_stack_create() {
	DiceRollInstructionResultStack *drirs = malloc(sizeof(DiceRollInstructionResultStack));
	if (drirs == NULL) {
		return NULL;
	}
	drirs->array = dyn_array_create(8);
	return drirs;
}

void dice_roll_instruction_result_stack_free(DiceRollInstructionResultStack *dris) {
	for (int i = dyn_array_count(dris->array) - 1; i > 0; --i) {
		DiceRollInstructionResult *result = dyn_array_element_at_index(dris->array, i);
		dice_roll_instruction_result_free(result);
	}
	dyn_array_free(dris->array);
	free(dris);
}

void dice_roll_instruction_result_stack_push(DiceRollInstructionResultStack *drirs, DiceRollInstructionResult *drir) {
	dyn_array_push(drirs->array, drir);
}

DiceRollInstructionResult * dice_roll_instruction_result_stack_peek(DiceRollInstructionResultStack *drirs) {
	return dyn_array_peek(drirs->array);
}


DiceRollInstructionResult * dice_roll_instruction_result_stack_pop(DiceRollInstructionResultStack *drirs) {
	return dyn_array_pop(drirs->array);
}

