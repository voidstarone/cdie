#include <stdlib.h>
#include <stdio.h>

#include "DiceRollInstructionResult.h"
#include "DiceRollInstructionResultStack.h"

DiceRollInstructionResultStack *dice_roll_instruction_result_stack_create() {
	DiceRollInstructionResultStack *drirs = malloc(sizeof(DiceRollInstructionResultStack));
	drirs->size = 5;
	drirs->count = 0;
	drirs->results = malloc(sizeof(DiceRollInstructionResult *) * drirs->size);
	return drirs;
}

void dice_roll_instruction_result_stack_free(DiceRollInstructionResultStack *dris) {
	for (int i = dris->count-1; i > 0; --i) {
		dice_roll_instruction_result_free(dris->results[i]);
	}
	free(dris->results);
	free(dris);
}

void dice_roll_instruction_result_stack_push(DiceRollInstructionResultStack *drirs, DiceRollInstructionResult *drir) {
	if (drirs->count+1 >= drirs->size) {
		size_t new_size = drirs->size * 1.25;
		drirs->results = realloc(drirs->results, new_size * sizeof(DiceRollInstruction *));
		drirs->size = new_size;
	}
	drirs->results[drirs->count] = drir;
	drirs->count = drirs->count + 1;
}

DiceRollInstructionResult * dice_roll_instruction_result_stack_peek(DiceRollInstructionResultStack *drirs) {
	if (drirs->count == 0) {
		return NULL;
	}
	return drirs->results[drirs->count-1];
}


DiceRollInstructionResult * dice_roll_instruction_result_stack_pop(DiceRollInstructionResultStack *drirs) {
	if (drirs->count == 0) {
		return NULL;
	}

	DiceRollInstructionResult *drir = drirs->results[drirs->count-1];
	drirs->count = drirs->count - 1;
	return drir;
}

