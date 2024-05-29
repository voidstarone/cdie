#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "numutils.h"

#include <stdarg.h>
#include "DiceCollectionResults.h"
#include "DiceCollection.h"

DiceCollectionResults * dice_collection_results_create(size_t capacity) {
	DiceCollectionResults *dcr = malloc(sizeof(DiceCollectionResults));
	
	dcr->results_array = malloc(sizeof(size_t) * capacity);
	dcr->size = capacity;
	dcr->count = 0;
	for (size_t i = 0; i < capacity; i++) {
		dcr->results_array[i] = 0;
	}
		
	return dcr;
}

size_t dice_collection_results_result_at(DiceCollectionResults *dcr, size_t index) {
	return dcr->results_array[index];
}

void dice_collection_results_add(DiceCollectionResults *dcr, size_t result) {
	if (dcr->count+1 >= dcr->size) {
		size_t new_size = dcr->size * 1.25;
		size_t *tmp_results_array = realloc(dcr->results_array, new_size * sizeof(size_t));
		if (tmp_results_array == NULL) {
			exit(1);
		}
		dcr->results_array = tmp_results_array;
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
	DiceCollectionResults *clone_dcr = dice_collection_results_create(dcr->count);
	clone_dcr->count = dcr->count;
	for(size_t i = 0; i < dcr->count; ++i) {
		clone_dcr->results_array[i] = dcr->results_array[i];
	}
	return clone_dcr;
}

void dice_collection_results_free(DiceCollectionResults *dcr) {
	if (dcr == NULL) { return; }
	if (dcr->results_array != NULL) {
		free(dcr->results_array);
	}
	dcr->size = 0;
	dcr->count = 0;
	free(dcr);
	dcr = NULL;
}

size_t dice_collection_results_count(DiceCollectionResults *dcr) {
	return dcr->count;
}

void dice_collection_results_as_array(DiceCollectionResults *dcr, size_t *results) {
	for(size_t i = 0; i < dcr->count; ++i) {
		results[i] = dcr->results_array[i];
	}
}

char * dice_collection_create_results_string(DiceCollectionResults *dcr) {
	size_t total_characters = dcr->count;
	
	for(size_t i = 0; i < dcr->count; ++i) {
		total_characters += num_digits(dcr->results_array[i]);
	}
	char *results_string = malloc(sizeof(char) * total_characters);
	size_t char_index = 0;
	for(size_t i = 0; i < dcr->count; ++i) {
		if (i != 0 && char_index != 0) {
			snprintf(results_string + char_index, total_characters, " ");
			char_index++;
		}
		snprintf(results_string+char_index, total_characters, "%d", dcr->results_array[i]);
		char_index += num_digits(dcr->results_array[i]);
	}
	return results_string;
}

char * dice_collection_results_desc(DiceCollectionResults *dcr) {
	
	char *desc_string = "NOT IMPLEMENTED";
	dcr->results_array[0] = 0;
	return desc_string;
}

