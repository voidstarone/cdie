#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "numutils.h"

#include "DieFactory.h"
#include "DiceCollection.h"
#include "DiceCollectionResults.h"

DiceCollection * dice_collection_create(size_t count, size_t faces) {
    DiceCollection *dc = malloc(sizeof(DiceCollection));
    dc->_die_array = diefactory_make_die_array(faces, count);
    dc->_size = count;
    dc->num_faces = faces;
    dc->explosion_lower_bound = 0;
    dc->last_results = NULL;
    dc->do_explosions_stack = true;
    return dc;
}

size_t dice_collection_count(DiceCollection *dc) {
    return dc->_size;
}

size_t dice_collection_faces(DiceCollection *dc) {
    return dc->num_faces;
}

inline Die * dice_collection_die_at(DiceCollection *dc, size_t index) {
    return dc->_die_array[index];
}

void dice_collection_do_explodes(DiceCollection *dc, size_t start_index) {
    DiceCollectionResults *dcr = dc->last_results;
    size_t count = dice_collection_results_count(dcr);
    size_t i, num_explosions = 0;
    for (i=start_index; i < count; ++i) {
        size_t die_result = dice_collection_results_result_at(dcr, i);
        if (die_result >= dc->explosion_lower_bound) {
            num_explosions++;
        }
    }
    if (num_explosions == 0) {
        return;
    }
    Die *d = die_create(dc->num_faces);
    for (i = 0; i < num_explosions; ++i) {
        dice_collection_results_add(dcr, die_roll(d));
    }
    die_free(d);
    if (dc->do_explosions_stack) {
        dice_collection_do_explodes(dc, count);
    }
}

void dice_collection_set_results(DiceCollection *dc, size_t *results) {
    if (dc == NULL) {
        return;
    }
    if (dc->last_results != NULL) {
        dice_collection_results_free(dc->last_results);
    }
    DiceCollectionResults *dcr = dice_collection_results_create_for_dice_collection(dc);
    for (size_t i = 0, count = dice_collection_count(dc); i < count; i++) {
        size_t result_value = results[i];
        dice_collection_results_add(dcr, result_value);
    }
    dc->last_results = dcr;
}

void dice_collection_roll_silent(DiceCollection *dc) {
    DiceCollectionResults *dcr = dice_collection_results_create_for_dice_collection(dc);
    if (dc->last_results) {
        dice_collection_results_free(dc->last_results);
    }
    dc->last_results = dcr;

    size_t count = dice_collection_count(dc);
    Die *d;
    size_t die_result, i;
    for (i = 0; i < count; ++i) {
        d = dice_collection_die_at(dc, i);
        die_result = die_roll(d);
        dice_collection_results_add(dcr, die_result);
    }
    
    if (dc->explosion_lower_bound) {
        dice_collection_do_explodes(dc, 0);
    }
}

void dice_collection_roll(DiceCollection *dc, DiceCollectionResults *dcr) {
    size_t count = dice_collection_count(dc);
    Die *d;
    for(size_t i = 0; i < count; ++i) {
        d = dice_collection_die_at(dc, i);
        dice_collection_results_add(dcr, die_roll(d));
    }
}

DiceCollectionResults * dice_collection_last_results(DiceCollection *dc) {
    if (dc->last_results == NULL) {
        dice_collection_roll_silent(dc);
    }
    return dc->last_results;
}

size_t dice_collection_total(DiceCollection *dc) {
    DiceCollectionResults *last_results = dice_collection_last_results(dc);
    size_t total = 0, count = dice_collection_results_count(last_results);
    for (size_t i = 0; i < count; i++) {
        total += dice_collection_results_result_at(last_results, i);
    }
    return total;
}

size_t dice_collection_get_explosion_lower_bound(DiceCollection *dc) {
    return dc->explosion_lower_bound;
}

void dice_collection_set_explosion_lower_bound(DiceCollection *dc, size_t lower_bound) {
    dc->explosion_lower_bound = lower_bound;
}

bool dice_collection_get_stacking_explosions(DiceCollection *dc) {
    return dc->do_explosions_stack;
}

void dice_collection_set_stacking_explosions(DiceCollection *dc, bool do_explosions_stack) {
    dc->do_explosions_stack = do_explosions_stack;
}


void dice_collection_free(DiceCollection *dc) {
    if (dc == NULL) { return; }
    for(size_t i = 0; i < dc->_size; ++i) {
        die_free(dc->_die_array[i]);
    }
    
    free(dc->_die_array);
    dc->_size = 0;
    dc->num_faces = 0;
    dc->explosion_lower_bound = 0;
    
    if (dc->last_results != NULL) {
        dice_collection_results_free(dc->last_results);
    }
    
    free(dc);
    dc = NULL;
}

char * dice_collection_desc(DiceCollection *dc, char *final_str) {
    size_t die_count = dc->_size;
    size_t die_faces = dc->num_faces;
    snprintf(final_str, 30, "DiceCollection(%zu, %zu){ ", die_faces, die_count);
    size_t die_index;
    Die *current_die;
    size_t last_result;
    
    // string length
    size_t result_str_len_max = (die_count * 2) + 1;
    size_t result_str_index = 0;
    for(die_index = 0; die_index < die_count; ++die_index) {
        current_die = dice_collection_die_at(dc, die_index);
        last_result = die_last_result(current_die);
        result_str_len_max += num_digits(last_result);
    }
    // Write string
    for(die_index = 0; die_index < die_count; ++die_index) {
        current_die = dice_collection_die_at(dc, die_index);
        last_result = die_last_result(current_die);
        
        size_t size = sizeof(final_str) + sizeof(result_str_index);
        if (die_index == (size_t) die_count-1) {
            snprintf(final_str+result_str_index, size, "%zu }", last_result);
        } else {
            snprintf(final_str+result_str_index, size, "%zu, ", last_result);
        }
        
        result_str_index += (num_digits(last_result) + 2) * sizeof(char);
    }
    
    return final_str;
}

DiceCollectionResults * dice_collection_results_create_for_dice_collection(DiceCollection *dc) {
    size_t num_faces = dice_collection_faces(dc);
    size_t explosion_lower_bound = dice_collection_get_explosion_lower_bound(dc);
    size_t results_array_size = dice_collection_count(dc);
    if (explosion_lower_bound) {
        size_t num_exploding_results = num_faces - explosion_lower_bound;
        size_t probable_num_results = dice_collection_count(dc) * num_faces / (num_faces - num_exploding_results);
        
        // Bad maths; just to be safe
        size_t safety_net = probable_num_results * 0.25; 
        results_array_size = probable_num_results + safety_net;
    }
    
    return dice_collection_results_create(results_array_size);
}
