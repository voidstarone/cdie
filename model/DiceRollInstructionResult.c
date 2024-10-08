#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DiceCollection.h"
#include "DiceRollInstructionResult.h"

DiceRollInstructionResult *dice_roll_instruction_result_create(void) {
    DiceRollInstructionResult *drir = malloc(sizeof(DiceRollInstructionResult));
    if (drir == NULL) {
        return NULL;
    }
    drir->result_value = NULL;
    drir->type = result_type_void;
    return drir;
}

DiceRollInstructionResult *dice_roll_instruction_result_init(void) {
    DiceRollInstructionResult *drir = malloc(sizeof(DiceRollInstructionResult));
    drir->result_value = NULL;
    drir->type = result_type_void;
    return drir;
}

DiceRollInstructionResult *dice_roll_instruction_result_with_double(double value) {
    DiceRollInstructionResult *drir = dice_roll_instruction_result_create();
    drir->type = result_type_double;
    drir->result_value = (double *) malloc(sizeof(double));
    memcpy(drir->result_value, &value, sizeof(double));
    return drir;
}

double dice_roll_instruction_result_value_as_double(DiceRollInstructionResult *drir) {
    return *((double *)drir->result_value);
}

DiceRollInstructionResult *dice_roll_instruction_result_with_dice_collection(DiceCollection *dc) {
    DiceRollInstructionResult *drir = dice_roll_instruction_result_create();
    drir->type = result_type_dice_collection;
    drir->result_value = dc;
    return drir;
}

double dice_roll_instruction_result_get_number(DiceRollInstructionResult *drir) {
    if (drir == NULL) {
        return -1;
    }
    if (drir->type == result_type_double) {
        return dice_roll_instruction_result_value_as_double(drir);
    }
    if (drir->type == result_type_dice_collection) {
        return dice_collection_total(drir->result_value);
    }
    return -1;
}

DiceCollection *dice_roll_instruction_result_get_dice_collection(DiceRollInstructionResult *drir) {
    if (drir->type != result_type_dice_collection) {
        return NULL;
    }
    return drir->result_value;
}

void dice_roll_instruction_result_free(DiceRollInstructionResult *drir) {
    if (drir == NULL) {
        return;
    }
    if (drir->result_value != NULL) {
        free(drir->result_value);
    }
    free(drir);
    drir = NULL;
}

void dice_roll_instruction_result_print(DiceRollInstructionResult *drir) {
    ResultType r_type = drir->type;
    double num;
    DiceCollection *dc;
    switch (r_type) {
        case result_type_double:
            num = dice_roll_instruction_result_get_number(drir);
            printf("%lf,", num);
            break;
        case result_type_dice_collection:
            dc = dice_roll_instruction_result_get_dice_collection(drir);
            printf("%zud%zu,", dc->_size, dc->num_faces);
            break;
        default:
            printf("Bad Op!,");
    }
}
