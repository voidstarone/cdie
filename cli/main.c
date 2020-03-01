#include <stdlib.h>
#include <stdio.h>
//#include <argp.h>
#include <stdbool.h>

#include "DiceRollingSession.h"

int main (int argc, char **argv) {
	DiceRollingSession *drs = dice_rolling_session_init();
	
	for(int i = 1; i < argc; ++i) {
			char *result = dice_rolling_session_resolve_notation(drs, argv[i]);
			if (result == NULL) {
				printf("Invalid input");
				exit(1);
			}
			printf("%s ", result);
			fflush(stdin);
			free(result);
	}
	printf("\n");

	dice_rolling_session_free(drs);
	
	return 0;
}