#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "numutils.h"

#include "DynArray.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"

DiceRollInstructionStack *dice_roll_instruction_stack_create() {
	DiceRollInstructionStack *dris = malloc(sizeof(DiceRollInstructionStack));
	dris->instructions = dyn_array_create(8);
	return dris;
}

void dice_roll_instruction_stack_free(DiceRollInstructionStack *dris) {
	for (int i = dyn_array_count(dris->instructions) - 1; i > 0; --i) {
		DiceRollInstruction *instruction = dyn_array_element_at_index(dris->instructions, i);
		dice_roll_instruction_free(instruction);
	}
	dyn_array_free(dris->instructions);
	free(dris);
}

void dice_roll_instruction_stack_push(DiceRollInstructionStack *dris, DiceRollInstruction *dri) {
	dyn_array_push(dris->instructions, dri);
}

DiceRollInstruction * dice_roll_instruction_stack_peek(DiceRollInstructionStack *dris) {
	return dyn_array_peek(dris->instructions);
}


DiceRollInstruction * dice_roll_instruction_stack_pop(DiceRollInstructionStack *dris) {
	return dyn_array_pop(dris->instructions);
}

DiceRollInstruction * dice_roll_instruction_stack_instruction_at(DiceRollInstructionStack *dris, int index) {
	return dyn_array_element_at_index(dris->instructions, index);
}


int dice_roll_instruction_stack_evaluate(DiceRollInstructionStack *dris, DiceRollInstructionResultStack *drirs) {
	DiceRollInstruction *dri = NULL;
	DiceRollInstruction **args = NULL;
	DiceRollInstructionResult *drir = NULL;
	int instruction_count = dyn_array_count(dris->instructions);
	int num_args = 0, arg_index = 0;
	for (int instruction_index = 0; instruction_index < instruction_count; ) {
		dri = dice_roll_instruction_stack_instruction_at(dris, instruction_index);
		num_args = dice_roll_instruction_get_num_args(dri);
		if (num_args == -1) {
			exit(-1);
		}
		if (instruction_index + num_args > instruction_count) {
			return -1;
		}
		args = malloc(sizeof(DiceRollInstruction *) * num_args);
		for (arg_index = 0; arg_index < num_args; arg_index++) {
			args[arg_index] = dice_roll_instruction_stack_instruction_at(dris, instruction_index + arg_index+1);
		}
		drir = dice_roll_instruction_do_op(dri, num_args, args);
		free(args);
		dice_roll_instruction_result_stack_push(drirs, drir);
		instruction_index += num_args + 1;
	}
	
	return 0;
}