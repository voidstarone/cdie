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


DiceRollInstructionResult *dice_roll_instruction_stack_evaluate(DiceRollInstructionStack *dris) {
	DiceRollInstruction *dri = NULL;
	DiceCollection *dc = NULL;
	double d = -1;
	DiceRollInstructionResult *drir = NULL;
	DiceRollInstructionResultStack *drirs = dice_roll_instruction_result_stack_create();
	// todo: return single result from this func

	while (dice_roll_instruction_stack_peek(dris)) {
		dri = dice_roll_instruction_stack_pop(dris);
		OperationType op_type = dice_roll_instruction_get_operation_type(dri);
		if (op_type == op_type_unknown) {
			dice_roll_instruction_result_stack_free(drirs);
			return NULL;
		}
		// Operands
		if (op_type <= op_type_number) {
			switch (op_type) {
				case op_type_number:
					d = dice_roll_instruction_get_number(dri);
					drir = dice_roll_instruction_result_with_double(d);
					break;
				case op_type_dice_collection:
					dc = dice_roll_instruction_get_dice_collection(dri);
					drir = dice_roll_instruction_result_with_dice_collection(dc);
					break;
				default:
					break;
			}
			dice_roll_instruction_result_stack_push(drirs, drir);
			continue;
		}

		// Operations
		drir = dice_roll_instruction_do_op(dri, drirs);
		dice_roll_instruction_result_stack_push(drirs, drir);
	}
	drir = dice_roll_instruction_result_stack_pop(drirs);
	dice_roll_instruction_result_stack_free(drirs);
	return drir;
}