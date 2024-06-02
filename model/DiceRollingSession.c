// Take string input and return formatted roll results

#include <stdio.h>
#include <string.h>
#include "DiceRollingSession.h"
#include "DiceDoodads.h"

DiceRollingSession * dice_rolling_session_init(void) {
	DiceRollingSession *drs = malloc(sizeof(DiceRollingSession));
	// drs->single_digit_d10s = false;
	return drs;
}

void dice_rolling_session_free(DiceRollingSession *drs) {
	free(drs);
}

char *dice_rolling_session_resolve_notation(DiceRollingSession *drs, char *expression) {
	char *retStr = malloc(sizeof(char) * 1000);
	for (size_t i = 0; i < 1000; i++) {
		retStr[i] = '\0';
	}
	
	DiceRollInstructionStack *instructions = dice_roll_instruction_stack_from_expression(expression);
	
	DynArray *dice_collections = dice_roll_instruction_stack_get_dice_collections(instructions);
	for (size_t i = 0; i < dyn_array_count(dice_collections); i++) {
		DiceCollection *dc = dyn_array_element_at_index(dice_collections, i);
		DiceCollectionResults *dcr = dice_collection_last_results(dc);
		char *results_str = dice_collection_create_results_string(dcr);
		strcat(retStr, results_str);
		free(results_str);
		strcat(retStr, "\n");
	}
	DiceRollInstructionResult *final_result = dice_roll_instruction_stack_evaluate(instructions);
	char final_result_str_buffer[128];
	double final_result_num = dice_roll_instruction_result_get_number(final_result);
	if (count_decimals(final_result_num) == 0) {
		snprintf(final_result_str_buffer, 128, "%.0lf", final_result_num);
	} else {
		snprintf(final_result_str_buffer, 128, "%.2lf", final_result_num);
	}
	strcat(retStr, final_result_str_buffer);																																																																																																																																												
	return retStr;
}
