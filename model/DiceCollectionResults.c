#import <stdlib.h>
#import <stdio.h> // Debugging; shouldn't be here

#include <stdarg.h>
#import "DiceCollectionResults.h"
#import "DiceCollection.h"

DiceCollectionResults * dice_collection_results_init(size_t capacity) {
	DiceCollectionResults *dcr = malloc(sizeof(DiceCollectionResults));
	
	dcr->results_array = (int *) malloc(sizeof(int) * capacity);
	dcr->size = capacity;
	dcr->count = 0;
	dcr->results_array[0] = 0;
	
	return dcr;
}

void dice_collection_results_add(DiceCollectionResults *dcr, int result) {
	if (dcr->count+2 >= dcr->size) {
		// extend
		printf("ERROR: dice_collection_results needs extending");
	}
	dcr->results_array[dcr->count++] = result;
	dcr->results_array[dcr->count] = 0;
}

void dice_collection_results_update_count(DiceCollectionResults *dcr) {
	size_t new_count = 0;
	int *results_array = dcr->results_array;
	
	while(results_array[new_count] != 0) {
		++new_count;
	}
	
	dcr->count = new_count;
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

int * dice_collection_results_as_array(DiceCollectionResults *dcr) {
	return dcr->results_array; // BAD needs trimming
}

char * dice_collection_results_desc(DiceCollectionResults *dcr) {
	
	char *desc_string = "NOT IMPLEMENTED";

	
	dcr->results_array[0] = 0;
	return desc_string;
}