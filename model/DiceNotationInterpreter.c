#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"
 

bool is_digit(char c) {
    return c > 47 && c < 58;
}


int extract_num_dice(int *num_dice, char *notation) {
    if (notation[0] == 'd') {
        *num_dice = 1;
        return 0;
    }
    if (notation[0] == '%') {
        *num_dice = 100;
        return 0;
    }
    int num_digits = 0;
    while (is_digit(notation[num_digits]) && notation[num_digits] != 0) {
        ++num_digits;
    }
    *num_dice = atoi(notation);
    
    return num_digits;
}

int is_dc_percentile(bool *is_percentile, char *notation, int start_index) {
    *is_percentile = notation[start_index] == '%';
    return start_index+1;
}

int extract_num_sides(int *num_sides, char *notation, int start_index) {
    int num_digits = 0;
    while (is_digit(notation[num_digits+start_index]) && notation[num_digits+start_index] != 0) {
        ++num_digits;
    }
    *num_sides = atoi(&notation[start_index]);
    
    return start_index+num_digits;
}

int does_dc_explode(bool *does_explode, char *notation, int start_index) {
    *does_explode = notation[start_index] == '!';
    return start_index+1;
}

int extract_explodes_at(int *explodes_at, char *notation, int start_index) {
    int num_digits = 0;
    while (is_digit(notation[num_digits]) && notation[num_digits] != 0) {
        ++num_digits;
    }
    *explodes_at = atoi(&notation[start_index]);
    
    return start_index+num_digits;
}

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
	
	return dice_collection_init(atoi(str_count), atoi(str_faces));
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
	
	return dice_collection_init(atoi(str_count), 100);
}

DiceCollection * dice_collection_from_notation(char *notation) {
	
	int num_dice = 0, 
    num_sides = 0,
    explodes_at = 0,
    len_notation = strlen(notation);
    
    bool is_percentile, does_explode;
    int next_index = extract_num_dice(&num_dice, notation);
    if (num_dice == 0) return NULL;
    next_index = is_dc_percentile(&is_percentile, notation, next_index);
    if (is_percentile) {
        num_sides = 100;
    } else {
        next_index = extract_num_sides(&num_sides, notation, next_index);
    }
    if (num_sides == 0) return NULL;
    
    next_index = does_dc_explode(&does_explode, notation, next_index);
    
    if (next_index >= len_notation) {
        return dice_collection_init(num_dice, num_sides);
    }
    next_index = extract_num_sides(&explodes_at, notation, next_index);
    if (!explodes_at) {
        explodes_at = num_sides;
    }
    if (next_index < len_notation) {
        return NULL;
    }
	DiceCollection *dc = dice_collection_init(num_dice, num_sides);
	dice_collection_set_explosion_lower_bound(dc, explodes_at);
	return dc;
}