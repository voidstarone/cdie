#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "DiceRollingSession.h"

typedef struct {
    long long botches_at;
    long long successes_at;
	char **args;
    int arg_count;
} roll_arguments;

static struct argp_option options[] = {
    {"botches", 'b', "BOTCHES", 0, "Upper bound for a botch"},
    {"successes", 's', "SUCCESSES", 0, "Lower bound for a success"},
    {0}
};

bool is_arg_operator(char *arg) {
	bool is_plus = strcmp(arg, "+") == 0;
	bool is_minus = strcmp(arg, "-") == 0;
	bool is_star = strcmp(arg, "*") == 0;
	bool is_slash = strcmp(arg, "/") == 0;
	return is_plus || is_minus || is_star || is_slash;
}

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
            arguments->args = realloc(arguments->args, sizeof(char*) * (arguments->arg_count + 1));
            arguments->args[arguments->arg_count] = arg;
            arguments->arg_count++;
            break;
        case ARGP_KEY_END:
            return 0;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, 0, 0};

int main (int argc, char **argv) {
	roll_arguments arguments;
	arguments.botches_at = 0;
	arguments.successes_at = __LONG_LONG_MAX__;
    arguments.arg_count = 0;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	DiceRollingSession *drs = dice_rolling_session_create();

	drs->success_lower_bound = arguments.successes_at;
	if (drs->success_lower_bound != __LONG_LONG_MAX__ && drs->botch_upper_bound == 0) {
		drs->botch_upper_bound = 1;
	} else {
		drs->botch_upper_bound = arguments.botches_at;
	}

	bool is_unquoted_expression = false;
	char *expression = NULL;
	for(size_t i = 0; i < arguments.arg_count; ++i) {
		if (is_arg_operator(arguments.args[i])) {
			is_unquoted_expression = true;
			break;
		}
	}

	if (is_unquoted_expression) {
		size_t length_required = 0;
		for(size_t i = 0; i < arguments.arg_count; ++i) {
			length_required = strlen(arguments.args[i]) + 1;
		}
		expression = malloc(length_required * sizeof(char));
		for(size_t i = 0; i < arguments.arg_count; ++i) {
			if (i != 0) {
				strcat(expression, " ");
			}
			strcat(expression, arguments.args[i]);
		}
		strcat(expression, "\0");
		arguments.arg_count = 1;
		arguments.args[0] = expression;
	}

	for(size_t i = 0; i < arguments.arg_count; ++i) {
		char *result = dice_rolling_session_resolve_notation(drs, arguments.args[i]);
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
	if (expression != NULL) {
		free(expression);
	}
	free(arguments.args);
	return 0;
}
