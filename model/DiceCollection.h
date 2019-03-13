#import "Die.h"

#ifndef DIE_COLLECTION_H_FA033981
#define DIE_COLLECTION_H_FA033981

typedef struct {
	int num_faces;
	size_t _size;
	Die *_die_array;
} DiceCollection;

DiceCollection * dice_collection_init(int faces, size_t count);

size_t dice_collection_count(DiceCollection *dc);
int dice_collection_faces(DiceCollection *dc);

Die * dice_collection_die_at(DiceCollection *dc, size_t index);

int dice_collection_roll(DiceCollection *dc, int results[]);
void dice_collection_roll_silent(DiceCollection *dc);

char * dice_collection_desc(DiceCollection *dc);

void dice_collection_clean(DiceCollection *dc);

#endif /* end of include guard: DIE_COLLECTION_H_FA033981 */

