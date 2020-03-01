#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "numutils.h"

#include "DieFactory.h"
#include "DiceCollection.h"
#include "DiceCollectionResults.h"


DiceCollection * dice_collection_init(size_t count, int faces) {
	DiceCollection *dc = malloc(sizeof(DiceCollection));
	dc->_die_array = diefactory_make_die_array(faces, count);
	dc->_size = count;
	dc->num_faces = faces;
	dc->explosion_lower_bound = 0;
	dc->last_results = NULL;
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

void dice_collection_roll_silent(DiceCollection *dc) {
	DiceCollectionResults *dcr = dice_collection_results_init_for_dice_collection(dc);
	
	size_t count = dice_collection_count(dc);
	Die *d;
	int die_result, i;
	for (i = 0; i < count; ++i) {
		d = dice_collection_die_at(dc, i);
		die_result = die_roll(d);
		dice_collection_results_add(dcr, die_result);
	}

	if (dc->explosion_lower_bound) {
		int num_explosions = 0;
		for (i = 0; i < count; ++i) {
			d = dice_collection_die_at(dc, i);
			die_result = die_last_result(d);
			if (die_result >= dc->explosion_lower_bound) {
				num_explosions++;
			}
		}
		Die *d = die_init(dc->num_faces);
		for (i = 0; i < num_explosions; ++i) {
			dice_collection_results_add(dcr, die_roll(d));
		}
		die_free(d);
	}
	
	if (dc->last_results) {
		dice_collection_results_free(dc->last_results);
	}
	dc->last_results = dcr;
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
	return dice_collection_results_clone(dc->last_results);
}

int dice_collection_get_explosion_lower_bound(DiceCollection *dc) {
	return dc->explosion_lower_bound;
}

void dice_collection_set_explosion_lower_bound(DiceCollection *dc, int lower_bound) {
	dc->explosion_lower_bound = lower_bound;
}

void dice_collection_free(DiceCollection *dc) {
	
	for(size_t i = 0; i < dc->_size; ++i) {
		die_free(dc->_die_array[i]);
	}
	
	free(dc->_die_array);
	dc->_size = 0;
	dc->num_faces = 0;
	dc->explosion_lower_bound = 0;
	
	if (dc->last_results) {
		dice_collection_results_free(dc->last_results);
	}
	
	free(dc);
}

char * dice_collection_desc(DiceCollection *dc) {
	size_t die_count = dc->_size;
	int die_faces = dc->num_faces;
	char *out_str;
	char out_str_start[30];
	sprintf(out_str_start, "DiceCollection(%d, %d){ ", die_faces, (int) die_count);
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
		
		if (die_index == (size_t) die_count-1) {
			sprintf(out_str+result_str_index, "%d }", last_result);
		} else {
			sprintf(out_str+result_str_index, "%d, ", last_result);
		}
		
		result_str_index += (num_digits(last_result) + 2) * sizeof(char);
	}
	
	size_t total_str_len = strlen(out_str_start) + result_str_len_max;
	char *final_str = malloc(total_str_len * sizeof(char));
	strcat(final_str, out_str_start);
	strcat(final_str, out_str);
	return final_str;
}

DiceCollectionResults * dice_collection_results_init_for_dice_collection(DiceCollection *dc) {
	int num_faces = dice_collection_faces(dc);
	int explosion_lower_bound = dice_collection_get_explosion_lower_bound(dc);
	size_t results_array_size = dice_collection_count(dc);
	if (explosion_lower_bound) {
		size_t num_exploding_results = num_faces - explosion_lower_bound;
		size_t probable_num_results = dice_collection_count(dc) *  num_faces/(num_faces-num_exploding_results);
		
		// Bad maths; just to be safe
		size_t safety_net = probable_num_results * 0.25; 
		results_array_size = probable_num_results + safety_net;
	}
	
	return dice_collection_results_init(results_array_size);
}
