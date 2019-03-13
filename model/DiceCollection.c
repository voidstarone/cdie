#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DieFactory.h"
#include "DiceCollection.h"

int num_digits(int start) {
	int count = 0;
	int n = start;
	while(n != 0) {
		n /= 10;
		++count;
	}
	return count + (start < 0 ? 1 : 0);
}


DiceCollection * dice_collection_init(int faces, size_t count) {
	DiceCollection *dc = malloc(sizeof(DiceCollection));
	dc->_die_array = diefactory_make_die_array(faces, count);
	dc->_size = count;
	dc->num_faces = faces;
	return dc;
}

size_t dice_collection_count(DiceCollection *dc) {
	return dc->_size;
}

int dice_collection_faces(DiceCollection *dc) {
	return dc->num_faces;
}


inline Die * dice_collection_die_at(DiceCollection *dc, size_t index) {
	return &(dc->_die_array[index]);
}

void dice_collection_roll_silent(DiceCollection *dc) {
	size_t count = dice_collection_count(dc);
	Die *d;
	for(size_t i = 0; i < count; ++i) {
		d = dice_collection_die_at(dc, i);
		die_roll(d);
	}
}

int dice_collection_roll(DiceCollection *dc, int *results) {
	size_t count = dice_collection_count(dc);
	Die *d;
	for(size_t i = 0; i < count; ++i) {
		d = dice_collection_die_at(dc, i);
		results[i] = die_roll(d);
	}
	
	return 1;
}

void dice_collection_clean(DiceCollection *dc) {
	free(dc->_die_array);
	dc->_size = 0;
	dc->num_faces = 0;
}

char * dice_collection_desc(DiceCollection *dc) {
	int die_count = dc->_size;
	int die_faces = dc->num_faces;
	char *out_str;
	char out_str_start[30];
	sprintf(out_str_start, "DiceCollection(%d, %d){ ", die_faces, die_count);
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
		
		if (die_index == die_count-1) {
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

