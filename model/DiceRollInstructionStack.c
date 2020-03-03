#include <stdlib.h>
#include <stdio.h>

#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"

DiceRollInstructionStack *dice_roll_instruction_stack_init() {
	DiceRollInstructionStack *dris = malloc(sizeof(DiceRollInstructionStack));
	dris->size = 5;
	dris->count = 0;
	dris->instructions = malloc(sizeof(DiceRollInstruction *) * dris->size);
	return dris;
}

void dice_roll_instruction_stack_free(DiceRollInstructionStack *dris) {
	for (int i = dris->count-1; i > 0; --i) {
		dice_roll_instruction_free(dris->instructions[i]);
	}
	free(dris->instructions);
	free(dris);
}

void dice_roll_instruction_stack_push(DiceRollInstructionStack *dris, DiceRollInstruction *dri) {
	if (dris->count+1 >= dris->size) {
		size_t new_size = dris->size * 1.25;
		dris->instructions = realloc(dris->instructions, new_size * sizeof(DiceRollInstruction *));
		dris->size = new_size;
	}
	dris->instructions[dris->count] = dri;
	dris->count = dris->count + 1;
}

DiceRollInstruction * dice_roll_instruction_stack_peek(DiceRollInstructionStack *dris) {
	if (dris->count == 0) {
		return NULL;
	}
	return dris->instructions[dris->count-1];
}


DiceRollInstruction * dice_roll_instruction_stack_pop(DiceRollInstructionStack *dris) {
	if (dris->count == 0) {
		return NULL;
	}

	DiceRollInstruction *dri = dris->instructions[dris->count-1];
	dris->count = dris->count - 1;
	return dri;
}

DiceRollInstruction * dice_roll_instruction_stack_instruction_at(DiceRollInstructionStack *dris, int index) {
	if (index >= dris->count || index < 0) {
		return NULL;
	}
	DiceRollInstruction *dri = dris->instructions[index];
	return dri;
}


int dice_roll_instruction_stack_evaluate(DiceRollInstructionStack *dris, DiceRollInstructionResultStack *drirs) {
	if (drirs == NULL) {
		drirs = dice_roll_instruction_result_stack_init();
	}
	
	DiceRollInstruction *dri = NULL;
	int instruction_count = dris->count;
	int num_args = 0;
	for (int instruction_index = 0; instruction_index < instruction_count; instruction_index++) {
		dri = dice_roll_instruction_stack_instruction_at(dris, instruction_index);
		num_args = dice_roll_instruction_get_num_args(dri);
		if (instruction_index + num_args > instruction_count) {
			return -1;
		}
		
	}
	

	return 0;
}