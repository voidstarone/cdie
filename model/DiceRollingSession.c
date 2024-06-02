// Take string input and return formatted roll results

#include <stdio.h>
#include <string.h>
#include "DiceRollingSession.h"
#include "DiceDoodads.h"

DiceRollingSession * dice_rolling_session_create(void) {
	DiceRollingSession *drs = malloc(sizeof(DiceRollingSession));
	drs->dice_collections = NULL;
	// drs->single_digit_d10s = false;
	drs->botch_upper_bound = 0;
	drs->success_lower_bound = __LONG_LONG_MAX__;
	return drs;
}

void dice_rolling_session_free(DiceRollingSession *drs) {
	if (drs->dice_collections != NULL) {
		free(drs->dice_collections);
	}
	free(drs);
}

char *dice_rolling_session_resolve_notation(DiceRollingSession *drs, char *expression) {
	char *retStr = malloc(sizeof(char) * 1000);
	for (size_t i = 0; i < 1000; i++) {
		retStr[i] = '\0';
	}
	
	DiceRollInstructionStack *instructions = dice_roll_instruction_stack_from_expression(expression);
	
	drs->dice_collections = dice_roll_instruction_stack_get_dice_collections(instructions);
	for (size_t i = 0; i < dyn_array_count(drs->dice_collections); i++) {
		DiceCollection *dc = dyn_array_element_at_index(drs->dice_collections, i);
		DiceCollectionResults *dcr = dice_collection_last_results(dc);
		char *results_str = dice_collection_create_results_string(dcr);
		strcat(retStr, results_str);
		free(results_str);
		strcat(retStr, "\n");
        if (drs->success_lower_bound < __LONG_LONG_MAX__) {
            char success_buffer[32];
            long long int botches = dice_collection_count_results_below_or_matching_bound(
                dc,
                drs->botch_upper_bound
            );
            long long int successes = dice_collection_count_results_above_or_matching_bound(
                dc,
                drs->success_lower_bound
            );
            snprintf(success_buffer, 32, "successes: %lld\n", successes - botches);
            strcat(retStr, success_buffer);
        }
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

	if (drs->success_lower_bound < __LONG_LONG_MAX__) {
		snprintf(final_result_str_buffer, 128, "%.0lf", final_result_num);
	}																																																																																																																																											
	return retStr;
}
