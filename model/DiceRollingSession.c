// Take string input and return formatted roll results

#include <stdio.h>

#include "DiceRollingSession.h"
#include "DiceDoodads.h"

DiceRollingSession * dice_rolling_session_init() {
	DiceRollingSession *drs = malloc(sizeof(DiceRollingSession));
	// drs->single_digit_d10s = false;
	return drs;
}

void dice_rolling_session_free(DiceRollingSession *drs) {
	free(drs);
}


char * dice_rolling_session_resolve_notation(DiceRollingSession *drs, char *notation) {
	
	DiceCollection *dc = dice_collection_from_notation(notation);

	if (dc == NULL) {
		return NULL;
	}
	
	dice_collection_roll_silent(dc);
	DiceCollectionResults *dcr = dice_collection_last_results(dc);
		
	char *retStr = dice_collection_results_string(dcr);
	
	dice_collection_free(dc);
	dice_collection_results_free(dcr);
	return retStr;
}