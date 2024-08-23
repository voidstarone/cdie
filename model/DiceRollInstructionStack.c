#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "numutils.h"

#include "DynArray.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"
#include "DiceRollInstructionResultStack.h"

DiceRollInstructionStack *dice_roll_instruction_stack_create(size_t size) {
	DiceRollInstructionStack *dris = malloc(sizeof(DiceRollInstructionStack));
	dris->instructions = dyn_array_create(size == 0 ? 1 : size);
	return dris;
}

void dice_roll_instruction_stack_free(DiceRollInstructionStack *dris) {
	size_t count = dyn_array_count(dris->instructions);
	if (count > 0) {
		for (size_t i = count - 1; i > 0; --i) {
			DiceRollInstruction *instruction = dyn_array_element_at_index(dris->instructions, i);
			dice_roll_instruction_free(instruction);
		}
	}
	dyn_array_free(dris->instructions);
	free(dris);
	dris = NULL;
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

DiceRollInstruction * dice_roll_instruction_stack_instruction_at(DiceRollInstructionStack *dris, size_t index) {
	return dyn_array_element_at_index(dris->instructions, index);
}

DynArray *dice_roll_instruction_stack_get_dice_collections(DiceRollInstructionStack *dris) {
	size_t instruction_count = dyn_array_count(dris->instructions);
	DynArray *dice_collections = dyn_array_create(instruction_count);
	for (size_t i = 0; i < instruction_count; i++) {
		DiceRollInstruction *instrution = dice_roll_instruction_stack_instruction_at(dris, i);

		DiceCollection *potential_dc = dice_roll_instruction_get_dice_collection(instrution);
		if (potential_dc != NULL) {
			dyn_array_push(dice_collections, potential_dc);
		}
	}
	return dice_collections;
}

DiceRollInstructionResult *result_from_instruction(DiceRollInstruction *dri) {
	OperationType op_type = dice_roll_instruction_get_operation_type(dri);
	double d = -1;
	DiceCollection *dc = NULL;
	switch (op_type) {
		case op_type_number:
			d = dice_roll_instruction_get_number(dri);
			return dice_roll_instruction_result_with_double(d);
		case op_type_dice_collection:
			dc = dice_roll_instruction_get_dice_collection(dri);
			return dice_roll_instruction_result_with_dice_collection(dc);
		default:
            return NULL;
	}
}

DiceRollInstructionResult *dice_roll_instruction_stack_evaluate(DiceRollInstructionStack *dris) {
	DiceRollInstruction *dri = NULL;
	DiceRollInstructionResult *drir = NULL;
	if (dyn_array_count(dris->instructions) == 1) {
		dri = dice_roll_instruction_stack_pop(dris);
		drir = result_from_instruction(dri);
		return drir;
	}
	DynArray *working_stack = dyn_array_create(16);
	DiceRollInstructionResultStack *drirs = dice_roll_instruction_result_stack_create(8);

    printf("Start evaluating:\n");
	while (dice_roll_instruction_stack_peek(dris)) {
		dri = dice_roll_instruction_stack_pop(dris);
        dice_roll_instruction_print(dri);
		OperationType op_type = dice_roll_instruction_get_operation_type(dri);
		if (op_type == op_type_unknown) {
			dice_roll_instruction_result_stack_free(drirs);
			dyn_array_free(working_stack);
			return NULL;
		}
		// Operands
		if (op_type <= op_type_number) {
			drir = result_from_instruction(dri);
			dyn_array_push(working_stack, drir);
			continue;
		}
		// Operations
		drir = dice_roll_instruction_do_op(dri, working_stack);
		dice_roll_instruction_result_stack_push(drirs, drir);
	}
    printf("End evaluating\n");

	drir = dice_roll_instruction_result_stack_pop(drirs);
	dice_roll_instruction_result_stack_free(drirs);
	return drir;
}
