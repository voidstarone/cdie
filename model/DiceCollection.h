#include "Die.h"
#include "DiceCollectionResults.h"
#include <stdbool.h>

#ifndef DIE_COLLECTION_H_FA033981
#define DIE_COLLECTION_H_FA033981

typedef struct {
	int num_faces;
	size_t _size;
	Die **_die_array;
	DiceCollectionResults *last_results;
	int explosion_lower_bound;
	bool do_explosions_stack;
} DiceCollection;

DiceCollectionResults * dice_collection_results_init_for_dice_collection(DiceCollection *dc);


DiceCollection * dice_collection_init(size_t count, int faces);

size_t dice_collection_count(DiceCollection *dc);
int dice_collection_faces(DiceCollection *dc);

Die * dice_collection_die_at(DiceCollection *dc, size_t index);

void dice_collection_roll(DiceCollection *dc, DiceCollectionResults *dcr);
void dice_collection_roll_silent(DiceCollection *dc);
int dice_collection_total(DiceCollection *dc);

int dice_collection_get_stacking_explosions(DiceCollection *dc);
void dice_collection_set_stacking_explosions(DiceCollection *dc, bool do_explosions_stack);

int dice_collection_get_explosion_lower_bound(DiceCollection *dc);
void dice_collection_set_explosion_lower_bound(DiceCollection *dc, int lower_bound);

DiceCollectionResults * dice_collection_last_results(DiceCollection *dc);


char * dice_collection_desc(DiceCollection *dc);

void dice_collection_free(DiceCollection *dc);

#endif /* end of include guard: DIE_COLLECTION_H_FA033981 */

