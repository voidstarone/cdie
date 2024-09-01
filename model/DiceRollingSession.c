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

size_t expected_output_length_for_dice_collections(DynArray *dcs) {
    size_t length = 64;
    for (size_t i = 0; i < dyn_array_count(dcs); i++) {
        DiceCollection *dc = dyn_array_element_at_index(dcs, i);
        size_t max_digits = num_digits(dice_collection_faces(dc));
        size_t num_dice = dice_collection_count(dc);
        length += max_digits * num_dice + num_dice;
    }
    return length;
}

int compare_dice_collection_found_index(void *arg1, void *arg2) {
    DiceCollection *dc1 = arg1;
    DiceCollection *dc2 = arg2;
    if (dc1->found_index > dc2->found_index) {
        return 1;
    } else if (dc1->found_index < dc2->found_index){
        return -1;
    }
    return 0;
}

char *dice_rolling_session_resolve_notation(DiceRollingSession *drs, char *expression) {
	DiceRollInstructionStack *instructions = dice_roll_instruction_stack_from_expression(expression);
	
	drs->dice_collections = dice_roll_instruction_stack_get_dice_collections(instructions);
    dyn_array_sort_in_place(drs->dice_collections, &compare_dice_collection_found_index);

    size_t expected_length = expected_output_length_for_dice_collections(drs->dice_collections);
    char *retStr = malloc(sizeof(char) * expected_length);
	for (size_t i = 0; i < expected_length; i++) {
		retStr[i] = '\0';
	}

	for (size_t i = 0; i < dyn_array_count(drs->dice_collections); i++) {
		DiceCollection *dc = dyn_array_element_at_index(drs->dice_collections, i);
		DiceCollectionResults *dcr = dice_collection_last_results(dc);
        char *results_str = malloc(sizeof(char) * 3200);
        
        snprintf(results_str, 2800, "%zud%zu: %s", dice_collection_count(dc), dice_collection_faces(dc), dice_collection_create_results_string(dcr));
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
	DiceRollInstruction *final_result = dice_roll_instruction_stack_evaluate(instructions);
	char final_result_str_buffer[128];
	double final_result_num = dice_roll_instruction_get_number(final_result);
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
