#ifndef DIE_ROLLING_SESSION_H_FA033981
#define DIE_ROLLING_SESSION_H_FA033981
#include <stdbool.h>

#include "DynArray.h"

typedef struct {
	DynArray *dice_collections;
	// various settings
	long long int botch_upper_bound;
	long long int success_lower_bound;
	// bool single_digit_d10s;
} DiceRollingSession;

DiceRollingSession * dice_rolling_session_create(void);

char * dice_rolling_session_resolve_notation(DiceRollingSession *drs, 
	char *notation);
	
void dice_rolling_session_free(DiceRollingSession *drs);

#endif //DIE_ROLLING_SESSION
