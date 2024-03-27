#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "numutils.h"

#include "DieFactory.h"
#include "DiceCollection.h"
#include "DiceCollectionResults.h"


DiceCollection * dice_collection_create(size_t count, int faces) {
	DiceCollection *dc = malloc(sizeof(DiceCollection));
	dc->_die_array = diefactory_make_die_array(faces, count);
	dc->_size = count;
	dc->num_faces = faces;
	dc->explosion_lower_bound = 0;
	dc->last_results = NULL;
	dc->do_explosions_stack = true;
	return dc;
}

size_t dice_collection_count(DiceCollection *dc) {
	return dc->_size;
}

int dice_collection_faces(DiceCollection *dc) {
	return dc->num_faces;
}

inline Die * dice_collection_die_at(DiceCollection *dc, size_t index) {
	return dc->_die_array[index];
}

void dice_collection_do_explodes(DiceCollection *dc, int start_index) {
	DiceCollectionResults *dcr = dc->last_results;
	int count = dice_collection_results_count(dcr);
	int i, num_explosions = 0;
	for (i=start_index; i < count; ++i) {
		int die_result = dice_collection_results_result_at(dcr, i);
		if (die_result >= dc->explosion_lower_bound) {
			num_explosions++;
		}
	}
	if (num_explosions == 0) {
		return;
	}
	Die *d = die_create(dc->num_faces);
	for (i = 0; i < num_explosions; ++i) {
		dice_collection_results_add(dcr, die_roll(d));
	}
	die_free(d);
	if (dc->do_explosions_stack) {
		dice_collection_do_explodes(dc, count);
	}
}

void dice_collection_set_results(DiceCollection *dc, int *results) {
	if (dc == NULL) {
		return;
	}
	if (dc->last_results != NULL) {
		dice_collection_results_free(dc->last_results);
	}
	DiceCollectionResults *dcr = dice_collection_results_create_for_dice_collection(dc);
	for (size_t i = 0, count = dice_collection_count(dc); i < count; i++) {
		int result_value = results[i];
		dice_collection_results_add(dcr, result_value);
	}
	dc->last_results = dcr;
}

void dice_collection_roll_silent(DiceCollection *dc) {
	DiceCollectionResults *dcr = dice_collection_results_create_for_dice_collection(dc);
	if (dc->last_results) {
		dice_collection_results_free(dc->last_results);
	}
	dc->last_results = dcr;

	int count = dice_collection_count(dc);
	Die *d;
	int die_result, i;
	for (i = 0; i < count; ++i) {
		d = dice_collection_die_at(dc, i);
		die_result = die_roll(d);
		dice_collection_results_add(dcr, die_result);
	}
	
	if (dc->explosion_lower_bound) {
		dice_collection_do_explodes(dc, 0);
	}
}

void dice_collection_roll(DiceCollection *dc, DiceCollectionResults *dcr) {
	size_t count = dice_collection_count(dc);
	Die *d;
	for(size_t i = 0; i < count; ++i) {
		d = dice_collection_die_at(dc, i);
		dice_collection_results_add(dcr, die_roll(d));
	}
}

DiceCollectionResults * dice_collection_last_results(DiceCollection *dc) {
	if (dc->last_results == NULL) {
		dice_collection_roll_silent(dc);
	}
	return dc->last_results;
}

int dice_collection_total(DiceCollection *dc) {
	if (!dc->last_results) {
		return 0;
	}
	DiceCollectionResults *last_results = dice_collection_last_results(dc);
	int total = 0, count = dice_collection_results_count(last_results);
	for (int i = 0; i < count; i++) {
		total += dice_collection_results_result_at(last_results, i);
	}
	return total;
}

int dice_collection_get_explosion_lower_bound(DiceCollection *dc) {
	return dc->explosion_lower_bound;
}

void dice_collection_set_explosion_lower_bound(DiceCollection *dc, int lower_bound) {
	dc->explosion_lower_bound = lower_bound;
}

bool dice_collection_get_stacking_explosions(DiceCollection *dc) {
	return dc->do_explosions_stack;
}

void dice_collection_set_stacking_explosions(DiceCollection *dc, bool do_explosions_stack) {
	dc->do_explosions_stack = do_explosions_stack;
}


void dice_collection_free(DiceCollection *dc) {
	if (dc == NULL) { return; }
	for(size_t i = 0; i < dc->_size; ++i) {
		die_free(dc->_die_array[i]);
	}
	
	free(dc->_die_array);
	dc->_size = 0;
	dc->num_faces = 0;
	dc->explosion_lower_bound = 0;
	
	if (dc->last_results != NULL) {
		dice_collection_results_free(dc->last_results);
	}
	
	free(dc);
}

char * dice_collection_desc(DiceCollection *dc) {
	size_t die_count = dc->_size;
	int die_faces = dc->num_faces;
	char *out_str;
	char out_str_start[30];
	snprintf(out_str_start, 30, "DiceCollection(%d, %d){ ", die_faces, (int) die_count);
	size_t die_index;
	Die *current_die;
	int last_result;
	
	// string length
	size_t result_str_len_max = (die_count * 2) + 1;
	size_t result_str_index = 0;
	for(die_index = 0; die_index < die_count; ++die_index) {
		current_die = dice_collection_die_at(dc, die_index);
		last_result = die_last_result(current_die);
		result_str_len_max += num_digits(last_result);
	}
	// Write string
	out_str = malloc(sizeof(char) * result_str_len_max);
	for(die_index = 0; die_index < die_count; ++die_index) {
		current_die = dice_collection_die_at(dc, die_index);
		last_result = die_last_result(current_die);
		
		size_t size = sizeof(out_str) + sizeof(result_str_index);
		if (die_index == (size_t) die_count-1) {
			snprintf(out_str+result_str_index, size, "%d }", last_result);
		} else {
			snprintf(out_str+result_str_index, size, "%d, ", last_result);
		}
		
		result_str_index += (num_digits(last_result) + 2) * sizeof(char);
	}
	
	size_t total_str_len = strlen(out_str_start) + result_str_len_max;
	char *final_str = malloc(total_str_len * sizeof(char));
	strcat(final_str, out_str_start);
	strcat(final_str, out_str);
	return final_str;
}

DiceCollectionResults * dice_collection_results_create_for_dice_collection(DiceCollection *dc) {
	int num_faces = dice_collection_faces(dc);
	int explosion_lower_bound = dice_collection_get_explosion_lower_bound(dc);
	size_t results_array_size = dice_collection_count(dc);
	if (explosion_lower_bound) {
		size_t num_exploding_results = num_faces - explosion_lower_bound;
		size_t probable_num_results = dice_collection_count(dc) * num_faces/(num_faces-num_exploding_results);
		
		// Bad maths; just to be safe
		size_t safety_net = probable_num_results * 0.25; 
		results_array_size = probable_num_results + safety_net;
	}
	
	return dice_collection_results_create(results_array_size);
}
