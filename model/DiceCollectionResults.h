#include <stdbool.h>
#include <stdlib.h>

#ifndef DIE_COLLECTION_RESULTS_H_FA033981
#define DIE_COLLECTION_RESULTS_H_FA033981


typedef struct {
	// int num_faces;
	size_t count;
	size_t size;
	size_t *results_array;
} DiceCollectionResults;

DiceCollectionResults * dice_collection_results_create(size_t capacity);
void dice_collection_results_free(DiceCollectionResults *dcr);

size_t dice_collection_results_count(DiceCollectionResults *dcr);
size_t dice_collection_results_result_at(DiceCollectionResults *dcr, size_t index);
void dice_collection_results_add(DiceCollectionResults *dcr, size_t result);

void dice_collection_results_as_array(DiceCollectionResults *dcr, size_t *results);

DiceCollectionResults * dice_collection_results_clone(DiceCollectionResults *dcr);

DiceCollectionResults * dice_collection_results_filter(DiceCollectionResults *dcr, bool (*condition)(int) );

char * dice_collection_create_results_string(DiceCollectionResults *dcr);

double dice_collection_results_sum(DiceCollectionResults *dcr);


#endif /* end of include guard: DIE_COLLECTION_RESULTS_H_FA033981 */
