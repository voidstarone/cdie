#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DiceRollInstruction.h"
#include "Die.h"

#define DRI_DATA_OP_TYPE 0
#define NUM_OPERATIONS 5 

char validOperations[NUM_OPERATIONS][5] = {
    "+", "-", "sum", "mean", "max"
};

DiceRollInstruction *dice_roll_instruction_init() {
    DiceRollInstruction *dri = malloc(sizeof(DiceRollInstruction));
    return dri;
}

void dice_roll_instruction_free(DiceRollInstruction *dri) {
    free(dri);
}


int dice_roll_instruction_find_operation_type_for_string(char *stringRepresentation) {
    int searchIndex = 1, comparisonResult;
    for (; searchIndex < NUM_OPERATIONS; searchIndex++)
    {
        comparisonResult = strcmp(stringRepresentation, validOperations[searchIndex]);
        if (comparisonResult == 0) {
            return searchIndex;
        }
    }
    return DRI_DATA_OP_TYPE;
}

OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri) {
    return dri->operationType;
}

void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType opType) {
    dri->operationType = opType;
}

DiceRollInstruction *dice_roll_instruction_from_string(char *stringRepresentation) {
    DiceRollInstruction *dri = dice_roll_instruction_init();
    OperationType operationType = dice_roll_instruction_find_operation_type_for_string(stringRepresentation);
    dice_roll_instruction_set_operation_type(dri, operationType);
    if (operationType == DRI_DATA_OP_TYPE){
        // TODO: implement dice collections
    }
    return dri;
}
