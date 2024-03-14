#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <regex.h>        

#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"
#include "DiceRollInstruction.h"
#include "Die.h"

#define DRI_DATA_OP_TYPE -1
#define NUM_OPERATIONS 8


DiceRollInstructionResult * (*ops[7]) (int, DiceRollInstruction **);
void setup_ops();

DiceRollInstruction *dice_roll_instruction_create() {
    DiceRollInstruction *dri = malloc(sizeof(DiceRollInstruction));
    dri->value = NULL;
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

bool true_if_regex_match(char *regex_str, char *string_rep) {
    regex_t regex;
    int reti;
    char msgbuf[100];
    bool is_match = false;

    /* Compile regular expression */
    reti = regcomp(&regex, regex_str, REG_EXTENDED|REG_ICASE);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    /* Execute regular expression */
    reti = regexec(&regex, string_rep, 0, NULL, 0);
    if (!reti) {
        is_match = true;
    } else if (reti != REG_NOMATCH) {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    regfree(&regex);
    return is_match;
}

bool dice_roll_instruction_string_is_dice_collection(char *string_rep) {
    return true_if_regex_match("[0-9]+(d[0-9]+|%)!?", string_rep);
}

bool dice_roll_instruction_string_is_double(char *string_rep) {
    return true_if_regex_match("[0-9]+(.[0-9]*)?", string_rep);
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
    if (dice_roll_instruction_string_is_dice_collection(string_rep)) {
        return op_type_dice_collection;
    }
    if (dice_roll_instruction_string_is_double(string_rep)) {
        return op_type_number;
    }

    return op_type_unknown;
}

OperationType dice_roll_instruction_get_operation_type(DiceRollInstruction *dri) {
    return dri->operation_type;
}

void dice_roll_instruction_set_operation_type(DiceRollInstruction *dri, OperationType op_type) {
    if (op_type < 0) {
        dri->num_args = 0;
    } else if (op_type >= op_type_sum && op_type <= op_type_max) {
        dri->num_args = 1;
    } else if (op_type >= op_type_add && op_type <= op_type_divide) {
        dri->num_args = 2;
    } else {
        dri->num_args = -1;
    }
    
    dri->operation_type = op_type;
}

double dice_roll_instruction_get_number(DiceRollInstruction *dri) {
    if (dri->operation_type == op_type_number) {
        double *dbl_ptr = dri->value;
        return *dbl_ptr;
    }
    if (dri->operation_type == op_type_dice_collection) {
        DiceCollection *dc = dri->value;
        dice_collection_roll_silent(dc);
        return dice_collection_total(dc);
    }
    return 0;
}

int dice_roll_instruction_get_num_args(DiceRollInstruction *dri) {
    return dri->num_args;
}

void dice_roll_instruction_set_expected_result_type(DiceRollInstruction *dri, ResultType result_type) {
    dri->expected_result_type = result_type;
}

DiceRollInstruction *dice_roll_instruction_from_string(char *string_representation) {
    DiceRollInstruction *dri = dice_roll_instruction_create();
    dice_roll_instruction_set_expected_result_type(dri, result_type_double);
    OperationType op_type = dice_roll_instruction_find_operation_type_for_string(string_representation);
    dice_roll_instruction_set_operation_type(dri, op_type);
    double double_value = -1;
    DiceCollection *dc = NULL;
    switch (op_type) {
        case op_type_number:
            double_value = strtod(string_representation, NULL);
            dri->value = malloc(sizeof(double));
            memcpy(dri->value, &double_value, sizeof(double));
            dice_roll_instruction_set_expected_result_type(dri, result_type_double);
            break;
        case op_type_dice_collection:
            dc = dice_collection_create_from_notation(string_representation);
            if (dc == NULL) {
                return NULL;
            }
            dri->value = dc;
            dice_roll_instruction_set_expected_result_type(dri, result_type_dice_collection);
            break;
        default:
            break;
    }
    return dri;
}

static bool dri_should_setup_ops = true;
DiceRollInstructionResult *dice_roll_instruction_do_op(DiceRollInstruction *dri, int argc, DiceRollInstruction **argv) {
    if (dri_should_setup_ops) {
        setup_ops();
        dri_should_setup_ops = false;
    }
    return ops[dri->operation_type](argc, argv);
}

DiceRollInstructionResult *op_add(int argc, DiceRollInstruction **argv) {
    if (argc != 2) return NULL;
    DiceRollInstruction *arg1 = argv[0];
    DiceRollInstruction *arg2 = argv[1];
    double num1, num2;
    num1 = dice_roll_instruction_get_number(arg1);
    num2 = dice_roll_instruction_get_number(arg2);
    DiceRollInstructionResult *result = dice_roll_instruction_result_with_double(num1 + num2);
    return result;
}

DiceRollInstructionResult *op_max(int argc, DiceRollInstruction **argv) {
    if (argc != 1) return NULL;
    DiceRollInstruction *arg1 = argv[0];
    if (arg1->operation_type != op_type_dice_collection) {
        return NULL;
    }
    DiceCollection *dc = arg1->value;
    double maximum = DBL_MIN;
    DiceCollectionResults *dcr = dice_collection_last_results(dc);

    for (size_t i = dice_collection_results_count(dcr) - 1; i > 0; --i) {
        double current_val = dice_collection_results_result_at(dcr, i);
        printf("current_val: %lf", current_val);
        if (current_val > maximum) {
            maximum = current_val;
        }
    }
    return dice_roll_instruction_result_with_double(maximum);
}

void setup_ops() {
    ops[(int) op_type_add] = op_add;
    ops[(int) op_type_max] = op_max;
}

