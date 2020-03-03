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

DiceRollInstructionResult *dice_roll_instruction_result_with_double(double value) {
    DiceRollInstructionResult *drir = dice_roll_instruction_result_init();
    drir->type = result_type_double;
    double *double_store = malloc(sizeof(double));
    *double_store = value;
    drir->result_value = double_store;
    return drir;
}
DiceRollInstructionResult *dice_roll_instruction_result_with_dice_collection(DiceCollection *dc) {
    DiceRollInstructionResult *drir = dice_roll_instruction_result_init();
    drir->type = result_type_dice_collection;
    drir->result_value = dc;
    return drir;
}


void dice_roll_instruction_result_free(DiceRollInstructionResult *drir) {
    if (drir->result_value != NULL) {
        free(drir->result_value);
    }
    free(drir);
}

