#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "numutils.h"

#include <stdarg.h>
#include "DiceCollectionResults.h"
#include "DiceCollection.h"

DiceCollectionResults * dice_collection_results_init(size_t capacity) {
	DiceCollectionResults *dcr = malloc(sizeof(DiceCollectionResults));
	
	dcr->results_array = (int *) malloc(sizeof(int) * capacity);
	dcr->size = capacity;
	dcr->count = 0;
	dcr->results_array[0] = 0;
	
	return dcr;
}

int dice_collection_results_result_at(DiceCollectionResults *dcr, int index) {
	return dcr->results_array[index];
}

void dice_collection_results_add(DiceCollectionResults *dcr, int result) {
	if (dcr->count+1 >= dcr->size) {
		size_t new_size = dcr->size * 1.25;
		dcr->results_array = realloc(dcr->results_array, new_size * sizeof(int));
		dcr->size = new_size;
	}
	dcr->results_array[dcr->count++] = result;
}

void dice_collection_results_update_count(DiceCollectionResults *dcr) {
	size_t new_count = 0;
	int *results_array = dcr->results_array;
	
	while(results_array[new_count] != 0) {
		++new_count;
	}
	
	dcr->count = new_count;
}

DiceCollectionResults * dice_collection_results_clone(DiceCollectionResults *dcr) {
	DiceCollectionResults *clone_dcr = dice_collection_results_init(dcr->count);
	clone_dcr->count = dcr->count;
	for(size_t i = 0; i < dcr->count; ++i) {
		clone_dcr->results_array[i] = dcr->results_array[i];
	}
	return clone_dcr;
}

void dice_collection_results_free(DiceCollectionResults *dcr) {
	free(dcr->results_array);
	dcr->size = 0;
	dcr->count = 0;
	free(dcr);
}

size_t dice_collection_results_count(DiceCollectionResults *dcr) {
	return dcr->count;
}

void dice_collection_results_as_array(DiceCollectionResults *dcr, int *results) {
	for(size_t i = 0; i < dcr->count; ++i) {
		results[i] = dcr->results_array[i];
	}
}

char * dice_collection_results_string(DiceCollectionResults *dcr) {
	int total_characters = dcr->count - (dcr->count == 1 ? 0 : 1);
	
	for(size_t i = 0; i < dcr->count; ++i) {
		total_characters += num_digits(dcr->results_array[i]);
	}
	char *results_string = malloc(sizeof(char) * total_characters);
	size_t char_index = 0;
	for(size_t i = 0; i < dcr->count; ++i) {
		sprintf(results_string+char_index, "%d", dcr->results_array[i]);
		char_index += num_digits(dcr->results_array[i]);
		if (i != dcr->count-1) {
			sprintf(results_string+char_index, " ");
			char_index++;
		}
	}
	return results_string;
}

char * dice_collection_results_desc(DiceCollectionResults *dcr) {
	
	char *desc_string = "NOT IMPLEMENTED";

	
	dcr->results_array[0] = 0;
	return desc_string;
}

