#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <limits.h>
#include <stdbool.h>

#include "DiceRollingSession.h"

typedef struct {
    long long botches_at;
    long long successes_at;
} roll_arguments;

static struct argp_option options[] = {
    {"botches", 'b', "BOTCHES", 0, "Upper bound for a botch"},
    {"successes", 's', "SUCCESSES", 0, "Lower bound for a success"},
    {0}
};

// Function to parse a single option
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    roll_arguments *arguments = state->input;
    switch (key) {
        case 'b':
            arguments->botches_at = atoll(arg);
            break;
        case 's':
            arguments->successes_at = atoll(arg);
            break;
        case ARGP_KEY_ARG:
            return 0;
        case ARGP_KEY_END:
            return 0;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// Argp parser
static struct argp argp = {options, parse_opt, 0, 0};

int main (int argc, char **argv) {
	roll_arguments arguments;
	arguments.botches_at = 0;
	arguments.successes_at = __LONG_LONG_MAX__;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	DiceRollingSession *drs = dice_rolling_session_create();

	drs->botch_upper_bound = arguments.botches_at;
	drs->success_lower_bound = arguments.successes_at;
	
	for(int i = 1; i < argc; ++i) {
		char *result = dice_rolling_session_resolve_notation(drs, argv[i]);
		if (result == NULL) {
			printf("Invalid input");
			exit(1);
		}
		printf("%s ", result);
		fflush(stdin);
		// free(result);
	}
	printf("\n");

	dice_rolling_session_free(drs);
	
	return 0;
}