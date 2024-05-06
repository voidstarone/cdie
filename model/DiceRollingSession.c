// Take string input and return formatted roll results

#include <stdio.h>
#include <string.h>
#include "DiceRollingSession.h"
#include "DiceDoodads.h"

DiceRollingSession * dice_rolling_session_init() {
	DiceRollingSession *drs = malloc(sizeof(DiceRollingSession));
	// drs->single_digit_d10s = false;
	return drs;
}

void dice_rolling_session_free(DiceRollingSession *drs) {
	free(drs);
}


char *dice_rolling_session_resolve_notation(DiceRollingSession *drs, char *expression) {
	char *retStr = "";
	
	printf("making instructions\n");
	DiceRollInstructionStack *instructions = dice_roll_instruction_stack_from_expression(expression);
	printf("getting collections\n");
	DynArray *dice_collections = dice_roll_instruction_stack_get_dice_collections(instructions);
	for (size_t i = 0; i < dyn_array_count(dice_collections); i++) {
		DiceCollection *dc = dyn_array_element_at_index(dice_collections, i);
		printf("count: %zu, faces: %zu\n", dice_collection_count(dc), dice_collection_faces(dc));
		DiceCollectionResults *dcr = dice_collection_last_results(dc);
		
		strcat(retStr, dcr);
		strcat(retStr, "\n");
	}
	DiceRollInstructionResult *final_result = dice_roll_instruction_stack_evaluate(instructions);
	char final_result_str_buffer[128];
	snprintf(final_result_str_buffer, 128, "%lf", dice_roll_instruction_result_get_number(final_result));
	strcat(retStr, final_result_str_buffer);
	return retStr;
}