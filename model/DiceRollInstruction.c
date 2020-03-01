#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"
#include "DiceRollInstruction.h"
#include "Die.h"

#define DRI_DATA_OP_TYPE -1
#define NUM_OPERATIONS 5 

char valid_operations[NUM_OPERATIONS][5] = {
    "+", "-", "sum", "mean", "max"
};

DiceRollInstruction *dice_roll_instruction_init() {
    DiceRollInstruction *dri = malloc(sizeof(DiceRollInstruction));
    dri->value = NULL;
    return dri;
}

void dice_roll_instruction_free(DiceRollInstruction *dri) {
    if (dri->value != NULL) {
        free(dri->value);
    }
    free(dri);
}


int dice_roll_instruction_find_operation_type_for_string(char *stringRepresentation) {
    int search_index = 0, comparison_result;
    for (; search_index < NUM_OPERATIONS; search_index++)
    {
        comparison_result = strcmp(stringRepresentation, valid_operations[search_index]);
        if (comparison_result == 0) {
            return search_index;
        }
    }
    return DRI_DATA_OP_TYPE;
}

OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri) {
    return dri->operation_type;
}

void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType op_type) {
    dri->operation_type = op_type;
}

DiceRollInstruction *dice_roll_instruction_from_string(char *string_representation) {
    DiceRollInstruction *dri = dice_roll_instruction_init();
    OperationType op_type = dice_roll_instruction_find_operation_type_for_string(string_representation);
    dice_roll_instruction_set_operation_type(dri, op_type);
    if (op_type < 0) {
        DiceCollection *dc;
        char *end_of_double;
        double *double_value = malloc(sizeof(double));
        *double_value = strtod(string_representation, &end_of_double);
        
        if (end_of_double == string_representation ||
            end_of_double != &string_representation[0] + strlen(string_representation)) {
            free(double_value);            
            dc = dice_collection_from_notation(string_representation);
            dri->value = dc;
            dice_roll_instruction_set_operation_type(dri, dice_collection);
            return dri;
        }
        dri->value = double_value;       
        dice_roll_instruction_set_operation_type(dri, number);
    }
    return dri;
}
