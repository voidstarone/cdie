#include <stdbool.h>
#include "CUnit/Basic.h"
#include "DiceRollingSession.h"

void test_dice_rolling_session_resolve_notation_simple_dice() {
    DiceRollingSession *drs = dice_rolling_session_create();

    dice_rolling_session_free(drs);
}

int test_suite_dice_rolling_session( int(*init_suite)(void), int(*clean_suite)(void)  ) {

	CU_pSuite pSuite = CU_add_suite("die_test_suite", init_suite, clean_suite);
	printf("test_suite_die\n");
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite, "test_dice_rolling_session_resolve_notation_simple_dice", 
		test_dice_rolling_session_resolve_notation_simple_dice)) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	return CU_get_error();
}