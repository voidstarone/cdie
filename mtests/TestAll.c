#include <stdbool.h>
#include "CUnit/Basic.h"
#include "TestSuiteDynArray.h"
#include "TestSuiteDie.h"
#include "TestSuiteDieFactory.h"
#include "TestSuiteDiceCollectionResults.h"
#include "TestSuiteDiceCollection.h"
#include "TestSuiteDiceNotationInterpreter.h"
#include "TestSuiteDiceRollInstruction.h"
#include "TestSuiteDiceRollInstructionResult.h"
#include "TestSuiteDiceRollInstructionStack.h"
#include "TestSuiteExpressionToDiceRollInstructionStack.h"

int init_suite(void)  { return 0; }
int clean_suite(void) { return 0; }


int main (/*int argc, char const *argv[]*/) {
	if (CUE_SUCCESS != CU_initialize_registry()) {
		return CU_get_error();
	}
	// test_suite_dyn_array(&init_suite, &clean_suite);
	// test_suite_die(&init_suite, &clean_suite);
	// test_suite_die_factory(&init_suite, &clean_suite);
	// test_suite_dice_collection_results(&init_suite, &clean_suite);
	// test_suite_dice_collection(&init_suite, &clean_suite);
	// test_suite_dice_notation(&init_suite, &clean_suite);
	// test_suite_dice_roll_instruction(&init_suite, &clean_suite);
	// test_suite_dice_roll_instruction_result(&init_suite, &clean_suite);
	// test_suite_dice_roll_instruction_stack(&init_suite, &clean_suite);
	test_suite_expression_to_dice_roll_instruction_stack(&init_suite, &clean_suite);
	// Sets the basic run mode, CU_BRM_VERBOSE will show maximum output of run details
	// Other choices are: CU_BRM_SILENT and CU_BRM_NORMAL
	CU_basic_set_mode(CU_BRM_VERBOSE);
	
	// Run the tests and show the run summary
	CU_basic_run_tests();
	return CU_get_error();
}