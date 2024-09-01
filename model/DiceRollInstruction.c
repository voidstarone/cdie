#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <regex.h>        

#include "DynArray.h"
#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"
#include "DiceRollInstruction.h"
#include "Die.h"

#define DRI_DATA_OP_TYPE -1
#define NUM_OPERATIONS 8


DiceRollInstruction * (*ops[7]) (DynArray *);
void setup_ops(void);

DiceRollInstruction *dice_roll_instruction_create(void) {
    DiceRollInstruction *dri = malloc(sizeof(DiceRollInstruction));
    dri->value = NULL;
    return dri;
}

DiceRollInstruction *dice_roll_instruction_create_number(double num) {
    DiceRollInstruction *dri = dice_roll_instruction_create();
    dri->operation_type = op_type_number;
    dri->value = malloc(sizeof(double));
    memcpy(dri->value, &num, sizeof(double));
    dice_roll_instruction_set_expected_result_type(dri, result_type_double);
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
    if (dri == NULL) return;
    if (dri->value != NULL) {
        if (dri->operation_type != op_type_dice_collection) {
            free(dri->value);
        }
    }
    free(dri);
    dri = NULL;
}

void dice_roll_instruction_free_deep(DiceRollInstruction *dri) {
    if (dri == NULL) return;
    if (dri->value != NULL) {
        if (dri->operation_type == op_type_dice_collection) {
            dice_collection_free(dri->value);
        } else {
            free(dri->value);
        }
    }
    free(dri);
    dri = NULL;
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
    if (dri == NULL) {
        printf("NULL dri access in dice_roll_instruction_get_number");
        return -1;
    }
    if (dri->operation_type == op_type_number) {
        double *dbl_ptr = dri->value;
        return *dbl_ptr;
    }
    if (dri->operation_type == op_type_dice_collection) {
        DiceCollection *dc = dri->value;
        return dice_collection_total(dc);
    }
    return 0;
}

DiceCollection *dice_roll_instruction_get_dice_collection(DiceRollInstruction *dri) {
    if (dri->operation_type == op_type_dice_collection) {
        DiceCollection *dc = dri->value;
        return dc;
    }
    return NULL;
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
                dice_roll_instruction_free(dri);
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
//DiceRollInstructionResult *dice_roll_instruction_do_op(DiceRollInstruction *dri, DynArray *argv) {
//    if (dri_should_setup_ops) {
//        setup_ops();
//        dri_should_setup_ops = false;
//    }
//    return ops[dri->operation_type](argv);
//}

DiceRollInstruction *dice_roll_instruction_do_op(DiceRollInstruction *dri, DynArray *argv) {
    if (dri_should_setup_ops) {
        setup_ops();
        dri_should_setup_ops = false;
    }
    return ops[dri->operation_type](argv);
}

//DiceRollInstructionResult *op_add(DynArray *argv) {
//    DiceRollInstructionResult *arg1 = dyn_array_pop(argv);
//    DiceRollInstructionResult *arg2 = dyn_array_pop(argv);
//    double num1, num2;
//    num1 = dice_roll_instruction_result_get_number(arg1);
//    num2 = dice_roll_instruction_result_get_number(arg2);
//    DiceRollInstructionResult *result = dice_roll_instruction_result_with_double(num1 + num2);
//    dice_roll_instruction_result_free(arg1);
//    dice_roll_instruction_result_free(arg2);
//    return result;
//}
//
//DiceRollInstructionResult *op_subtract(DynArray *argv) {
//    DiceRollInstructionResult *arg1 = dyn_array_pop(argv);
//    DiceRollInstructionResult *arg2 = dyn_array_pop(argv);
//    double num1, num2;
//    num1 = dice_roll_instruction_result_get_number(arg1);
//    num2 = dice_roll_instruction_result_get_number(arg2);
//    DiceRollInstructionResult *result = dice_roll_instruction_result_with_double(num2 - num1);
//    dice_roll_instruction_result_free(arg1);
//    dice_roll_instruction_result_free(arg2);
//    return result;
//}
//
//DiceRollInstructionResult *op_multiply(DynArray *argv) {
//    DiceRollInstructionResult *arg1 = dyn_array_pop(argv);
//    DiceRollInstructionResult *arg2 = dyn_array_pop(argv);
//    double num1, num2;
//    num1 = dice_roll_instruction_result_get_number(arg1);
//    num2 = dice_roll_instruction_result_get_number(arg2);
//    DiceRollInstructionResult *result = dice_roll_instruction_result_with_double(num1 * num2);
//    dice_roll_instruction_result_free(arg1);
//    dice_roll_instruction_result_free(arg2);
//    return result;
//}
//
//DiceRollInstructionResult *op_divide(DynArray *argv) {
//    DiceRollInstructionResult *arg1 = dyn_array_pop(argv);
//    DiceRollInstructionResult *arg2 = dyn_array_pop(argv);
//    double num1, num2;
//    num1 = dice_roll_instruction_result_get_number(arg1);
//    num2 = dice_roll_instruction_result_get_number(arg2);
//    DiceRollInstructionResult *result = dice_roll_instruction_result_with_double(num2 / num1);
//    dice_roll_instruction_result_free(arg1);
//    dice_roll_instruction_result_free(arg2);
//    return result;
//}
//
//DiceRollInstructionResult *op_max(DynArray *argv) {
//    DiceRollInstructionResult *arg1 = dyn_array_pop(argv);
//    if (arg1->type != result_type_dice_collection) {
//        return NULL;
//    }
//    DiceCollection *dc = arg1->result_value;
//    double maximum = DBL_MIN;
//    DiceCollectionResults *dcr = dice_collection_last_results(dc);
//
//
//    for (size_t i = dice_collection_results_count(dcr) - 1; i > 0; --i) {
//        double current_val = dice_collection_results_result_at(dcr, i);
//        if (current_val > maximum) {
//            maximum = current_val;
//        }
//    }
//
//    DiceRollInstructionResult *result = dice_roll_instruction_result_with_double(maximum);
//    dice_roll_instruction_result_free(arg1);
//    return result;
//}

DiceRollInstruction *op_add(DynArray *argv) {
    DiceRollInstruction *arg1 = dyn_array_pop(argv);
    DiceRollInstruction *arg2 = dyn_array_pop(argv);
    double num1, num2;
    num1 = dice_roll_instruction_get_number(arg1);
    num2 = dice_roll_instruction_get_number(arg2);
    
    DiceRollInstruction *new_instruction = dice_roll_instruction_create_number(num1 + num2);
    dice_roll_instruction_free(arg1);
    dice_roll_instruction_free(arg2);
    return new_instruction;
}

DiceRollInstruction *op_subtract(DynArray *argv) {
    DiceRollInstruction *arg1 = dyn_array_pop(argv);
    DiceRollInstruction *arg2 = dyn_array_pop(argv);
    double num1, num2;
    num1 = dice_roll_instruction_get_number(arg1);
    num2 = dice_roll_instruction_get_number(arg2);
    
    DiceRollInstruction *new_instruction = dice_roll_instruction_create_number(num2 - num1);
    dice_roll_instruction_free(arg1);
    dice_roll_instruction_free(arg2);
    return new_instruction;
}

DiceRollInstruction *op_multiply(DynArray *argv) {
    DiceRollInstruction *arg1 = dyn_array_pop(argv);
    DiceRollInstruction *arg2 = dyn_array_pop(argv);
    double num1, num2;
    num1 = dice_roll_instruction_get_number(arg1);
    num2 = dice_roll_instruction_get_number(arg2);
    
    DiceRollInstruction *new_instruction = dice_roll_instruction_create_number(num1 * num2);
    dice_roll_instruction_free(arg1);
    dice_roll_instruction_free(arg2);
    return new_instruction;
}

DiceRollInstruction *op_divide(DynArray *argv) {
    DiceRollInstruction *arg1 = dyn_array_pop(argv);
    DiceRollInstruction *arg2 = dyn_array_pop(argv);
    double num1, num2;
    num1 = dice_roll_instruction_get_number(arg1);
    num2 = dice_roll_instruction_get_number(arg2);
   
    DiceRollInstruction *new_instruction = dice_roll_instruction_create_number(num2 / num1);
    dice_roll_instruction_free(arg1);
    dice_roll_instruction_free(arg2);
    return new_instruction;
}

DiceRollInstruction *op_max(DynArray *argv) {
    DiceRollInstruction *arg1 = dyn_array_pop(argv);
    if (arg1->operation_type != op_type_dice_collection) {
        return NULL;
    }
    DiceCollection *dc = dice_roll_instruction_get_dice_collection(arg1);
    double maximum = DBL_MIN;
    DiceCollectionResults *dcr = dice_collection_last_results(dc);

    for (size_t i = dice_collection_results_count(dcr) - 1; i > 0; --i) {
        double current_val = dice_collection_results_result_at(dcr, i);
        if (current_val > maximum) {
            maximum = current_val;
        }
    }

    DiceRollInstruction *new_instruction = dice_roll_instruction_create_number(maximum);
    dice_roll_instruction_free(arg1);
    return new_instruction;
}

void dice_roll_instruction_print(DiceRollInstruction *dri) {
    OperationType op_type = dri->operation_type;
    double num;
    DiceCollection *dc;
    switch (op_type) {
        case op_type_add:
            printf("+");
            break;
        case op_type_subtract:
            printf("-");
            break;
        case op_type_multiply:
            printf("*");
            break;
        case op_type_divide:
            printf("/");
            break;
        case op_type_sum:
            printf("sum");
            break;
        case 5:
            printf("mean");
            break;
        case op_type_max:
            printf("max");
            break;
        case op_type_number:
            num = dice_roll_instruction_get_number(dri);
            printf("%lf", num);
            break;
        case op_type_dice_collection:
            dc = dice_roll_instruction_get_dice_collection(dri);
            printf("%zud%zu", dc->_size, dc->num_faces);
            break;
        default:
            printf("Bad Op!");
    }
}

void setup_ops(void) {
    ops[(int) op_type_add] = op_add;
    ops[(int) op_type_subtract] = op_subtract;
    ops[(int) op_type_multiply] = op_multiply;
    ops[(int) op_type_divide] = op_divide;
    ops[(int) op_type_max] = op_max;
}

