#include "Die.h"
#include "DiceCollectionResults.h"
#include <stdbool.h>

#ifndef DIE_COLLECTION_H_FA033981
#define DIE_COLLECTION_H_FA033981

typedef struct {
	size_t num_faces;
	size_t _size;
	Die **_die_array;
	DiceCollectionResults *last_results;
	size_t explosion_lower_bound;
	bool do_explosions_stack;
    size_t found_index;
} DiceCollection;

DiceCollectionResults * dice_collection_results_create_for_dice_collection(DiceCollection *dc);

DiceCollection * dice_collection_create(size_t count, size_t faces);

size_t dice_collection_count(DiceCollection *dc);
size_t dice_collection_faces(DiceCollection *dc);

void dice_collection_set_results(DiceCollection *dc, long long int *results);

Die * dice_collection_die_at(DiceCollection *dc, size_t index);

void dice_collection_roll(DiceCollection *dc, DiceCollectionResults *dcr);
void dice_collection_roll_silent(DiceCollection *dc);
long long int dice_collection_total(DiceCollection *dc);

bool dice_collection_get_stacking_explosions(DiceCollection *dc);
void dice_collection_set_stacking_explosions(DiceCollection *dc, bool do_explosions_stack);

long long int dice_collection_count_results_above_or_matching_bound(
    DiceCollection *dc, 
    long long int bound
);

long long int dice_collection_count_results_below_or_matching_bound(
    DiceCollection *dc, 
    long long int bound
);

long long int dice_collection_get_explosion_lower_bound(DiceCollection *dc);
void dice_collection_set_explosion_lower_bound(DiceCollection *dc, long long int lower_bound);

DiceCollectionResults * dice_collection_last_results(DiceCollection *dc);

char * dice_collection_desc(DiceCollection *dc, char *final_str);

void dice_collection_free(DiceCollection *dc);

#endif /* end of include guard: DIE_COLLECTION_H_FA033981 */

