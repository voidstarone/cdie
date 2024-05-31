#include <stdlib.h>
#include <stdio.h>

#include "DynArray.h"
#include "DiceRollInstructionResult.h"
#include "DiceRollInstructionResultStack.h"

DiceRollInstructionResultStack *dice_roll_instruction_result_stack_create(size_t size) {
	DiceRollInstructionResultStack *drirs = malloc(sizeof(DiceRollInstructionResultStack));
	if (drirs == NULL) {
		return NULL;
	}
	drirs->results = dyn_array_create(size);
	return drirs;
}

void dice_roll_instruction_result_stack_free(DiceRollInstructionResultStack *dris) {
	for (size_t i = dyn_array_count(dris->results) - 1; i > 0; --i) {
		DiceRollInstructionResult *result = dyn_array_element_at_index(dris->results, i);
		dice_roll_instruction_result_free(result);
	}
	dyn_array_free(dris->results);
	free(dris);
	dris = NULL;
}

void dice_roll_instruction_result_stack_push(DiceRollInstructionResultStack *drirs, DiceRollInstructionResult *drir) {
	dyn_array_push(drirs->results, drir);
}

DiceRollInstructionResult * dice_roll_instruction_result_stack_peek(DiceRollInstructionResultStack *drirs) {
	return dyn_array_peek(drirs->results);
}

DiceRollInstructionResult * dice_roll_instruction_result_stack_pop(DiceRollInstructionResultStack *drirs) {
	return dyn_array_pop(drirs->results);
}
