#include "DiceCollection.h"

#ifndef DIE_NOTATION_H_FA033981
#define DIE_NOTATION_H_FA033981

#define DICE_NOTATION_SEPARATOR "d"
#define DICE_NOTATION_PERCENTILE_INDICATOR "%"

typedef struct {
	DiceCollection *_diceCollection;
	char *_str_notation;
} DiceNotation;

// Take something like "2d6" and turn it into a dice collection
DiceCollection * dice_collection_from_core_notation(char *notation);
DiceCollection * dice_collection_from_percentile_notation(char *notation);
DiceCollection * dice_collection_create_from_notation(char *notation);

#endif /* end of include guard: DIE_NOTATION_H_FA033981 */
