#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DieFactory.h"
#include "DiceCollection.h"
#include "DiceNotation.h"


// Take something like "2d6" and turn it into a dice collection
DiceCollection * dice_collection_from_core_notation(char *notation) {
	// We assume only one dice notation here; nothing surrounding it
	char *separator = DICE_NOTATION_SEPARATOR;

	char *mutable_notation = malloc(sizeof(char) * strlen(notation));
	strcpy(mutable_notation, notation);

	char *str_faces;
	char *str_count;

	str_count = strtok(mutable_notation, separator);
	str_faces = strtok(NULL, separator);
	
	return dice_collection_init(atoi(str_faces), atoi(str_count));
}

DiceCollection * dice_collection_from_percentile_notation(char *notation) {
	char *percentile_indicator = DICE_NOTATION_PERCENTILE_INDICATOR;
	if (strstr(notation, DICE_NOTATION_PERCENTILE_INDICATOR) == NULL) {
		return NULL; // Not percentile
	}
	
	char *mutable_notation = malloc(sizeof(char) * strlen(notation));
	strcpy(mutable_notation, notation);

	char *str_count;
	str_count = strtok(mutable_notation, percentile_indicator);
	
	return dice_collection_init(100, atoi(str_count));
}