#include <stdbool.h>
#include "CUnit/Basic.h"
#include "Die.h"
#include "DieFactory.h"
#include "DiceCollection.h"
#include "DiceNotationInterpreter.h"
#include "DiceRollInstruction.h"
#include "DiceRollInstructionStack.h"

int test_suite_expression_to_dice_roll_instruction_stack(int(*init_suite)(void), int(*clean_suite)(void) );
	