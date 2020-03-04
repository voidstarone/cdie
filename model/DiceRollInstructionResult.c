#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DiceCollection.h"
#include "DiceRollInstructionResult.h"

DiceRollInstructionResult *dice_roll_instruction_result_init() {
    DiceRollInstructionResult *drir = malloc(sizeof(DiceRollInstructionResult));
    drir->result_value = NULL;
    drir->type = result_type_void;
    return drir;
}

// TODO: Actually assign the value
DiceRollInstructionResult *dice_roll_instruction_result_with_double(double value) {
    DiceRollInstructionResult *drir = dice_roll_instruction_result_init();
    drir->type = result_type_double;
    drir->result_value = malloc(sizeof(double));
    memcpy(drir->result_value, &value, sizeof(double));
    return drir;
}

DiceRollInstructionResult *dice_roll_instruction_result_with_dice_collection(DiceCollection *dc) {
    DiceRollInstructionResult *drir = dice_roll_instruction_result_init();
    drir->type = result_type_dice_collection;
    drir->result_value = dc;
    return drir;
}

double dice_roll_instruction_result_get_number(DiceRollInstructionResult *drir) {
    if (drir->type == result_type_double) {
        double *result = drir->result_value;
        return *result;
    }
    if (drir->type == result_type_dice_collection) {
        return dice_collection_total(drir->result_value);
    }
}

void dice_roll_instruction_result_free(DiceRollInstructionResult *drir) {
    if (drir->result_value != NULL) {
        free(drir->result_value);
    }
    free(drir);
}

