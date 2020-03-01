#include <stdbool.h>
#include <stdlib.h>

#ifndef DIE_COLLECTION_RESULTS_H_FA033981
#define DIE_COLLECTION_RESULTS_H_FA033981


typedef struct {
	// int num_faces;
	size_t count;
	size_t size;
	int *results_array;
} DiceCollectionResults;

DiceCollectionResults * dice_collection_results_init(size_t capacity);
void dice_collection_results_free(DiceCollectionResults *dcr);

size_t dice_collection_results_count(DiceCollectionResults *dcr);
int dice_collection_results_result_at(DiceCollectionResults *dcr, int index);
void dice_collection_results_add(DiceCollectionResults *dcr, int result);

void dice_collection_results_as_array(DiceCollectionResults *dcr, int *results);

DiceCollectionResults * dice_collection_results_clone(DiceCollectionResults *dcr);

DiceCollectionResults * dice_collection_results_filter(DiceCollectionResults *dcr, bool (*condition)(int) );

char * dice_collection_results_string(DiceCollectionResults *dcr);

#endif /* end of include guard: DIE_COLLECTION_RESULTS_H_FA033981 */
