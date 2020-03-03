#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"
#include "DiceRollInstruction.h"
#include "Die.h"

#define DRI_DATA_OP_TYPE -1
#define NUM_OPERATIONS 8

bool has_first_instruction_been_created = false;
int (*ops[7]) (DiceRollInstructionResult *, int, DiceRollInstruction **);
void setup_ops();

DiceRollInstruction *dice_roll_instruction_init() {
    DiceRollInstruction *dri = malloc(sizeof(DiceRollInstruction));
    dri->value = NULL;
    if (has_first_instruction_been_created) {
        setup_ops();
        has_first_instruction_been_created = true;
    }
    return dri;
}

DiceRollInstruction *dice_roll_instruction_clone(DiceRollInstruction *original) {
    DiceRollInstruction *clone = malloc(sizeof(DiceRollInstruction));
    clone->value = original->value;
    clone->expected_result_type = original->expected_result_type;
    clone->num_args = original->num_args;
    clone->operation_type = original->operation_type;
    return clone;
}

bool dice_roll_instruction_is_equal(DiceRollInstruction *dri1, DiceRollInstruction *dri2) {
    return dri1->value == dri2->value &&
           dri1->expected_result_type == dri2->expected_result_type &&
           dri1->num_args == dri2->num_args &&
           dri1->operation_type == dri2->operation_type;
}

void dice_roll_instruction_free(DiceRollInstruction *dri) {
    if (dri->value != NULL) {
        free(dri->value);
    }
    free(dri);
}


OperationType dice_roll_instruction_find_operation_type_for_string(char *string_rep) {
    if (strcmp(string_rep, "+") == 0) {
        return op_type_add;
    }
    if (strcmp(string_rep, "-") == 0) {
        return op_type_subtract;
    }
    if (strcmp(string_rep, "*") == 0) {
        return op_type_multiply;
    }
    if (strcmp(string_rep, "/") == 0) {
        return op_type_divide;
    }
    if (strcmp(string_rep, "sum") == 0) {
        return op_type_sum;
    }
    if (strcmp(string_rep, "mean") == 0) {
        return op_type_mean;
    }
    if (strcmp(string_rep, "avg") == 0) {
        return op_type_mean;
    }
    if (strcmp(string_rep, "max") == 0) {
        return op_type_max;
    }
    return op_type_unknown;
}

OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri) {
    return dri->operation_type;
}

void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType op_type) {
    if (op_type < 0) {
        dri->num_args = 0;
    } else if (op_type >= op_type_add && op_type <= op_type_divide) {
        dri->num_args = 2;
    } else {
        dri->num_args = -1;
    }
    
    dri->operation_type = op_type;
}

int dice_roll_instruction_get_num_args(DiceRollInstruction *dri) {
    return dri->num_args;
}

void dice_roll_instruction_set_expected_result_type(DiceRollInstruction *dri, ResultType result_type) {
    dri->expected_result_type = result_type;
}

DiceRollInstruction *dice_roll_instruction_from_string(char *string_representation) {
    DiceRollInstruction *dri = dice_roll_instruction_init();
    dice_roll_instruction_set_expected_result_type(dri, result_type_double);
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
            if (dc == NULL) {
                return NULL;
            }
            dri->value = dc;
            dice_roll_instruction_set_operation_type(dri, op_type_dice_collection);
            dice_roll_instruction_set_expected_result_type(dri, result_type_dice_collection);
            return dri;
        }
        dri->value = double_value;       
        dice_roll_instruction_set_operation_type(dri, op_type_number);
    }
    return dri;
}

// This atrocity takes a DiceRollInstruction, returns a pointer to a function with the signature
// int func(DiceRollInstructionResult *result, int argc, DiceRollInstruction **argv)
int (*dice_roll_instruction_get_op(DiceRollInstruction *dri)) (DiceRollInstructionResult *, int, DiceRollInstruction **) {
    return ops[dri->operation_type];
}


int op_add(DiceRollInstructionResult *result, int argc, DiceRollInstruction **argv) {

    //TODO: Implement this
    
    return 0;
}

void setup_ops() {
    ops[(int) op_type_add] = op_add;
}

