#ifndef DIE_ROLLING_SESSION_H_FA033981
#define DIE_ROLLING_SESSION_H_FA033981
#include <stdbool.h>

#include "DiceRollingSession.h"

typedef struct {
	// various settings
	// bool single_digit_d10s;
} DiceRollingSession;

DiceRollingSession * dice_rolling_session_init();

char * dice_rolling_session_resolve_notation(DiceRollingSession *drs, 
	char *notation);
	
void dice_rolling_session_free(DiceRollingSession *drs);

#endif //DIE_ROLLING_SESSION