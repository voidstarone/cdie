#include <stdlib.h>
#include <stdio.h>
//#include <argp.h>
#include <stdbool.h>

#include "DiceRollingSession.h"
//
//
// const char *argp_program_version = "0.0.1";
// const char *argp_program_bug_address = "tom@ratbum.com";
// static char doc[] = "Your program description.";
// static char args_doc[] = "[FILENAME]...";
// static struct argp_option options[] = {
//     { "line", 'l', 0, 0, "Compare lines instead of characters."},
//     { "word", 'w', 0, 0, "Compare words instead of characters."},
//     { "nocase", 'i', 0, 0, "Compare case insensitive instead of case sensitive."},
//     { 0 }
// };
//
// struct arguments {
//     enum { CHARACTER_MODE, WORD_MODE, LINE_MODE } mode;
//     bool isCaseInsensitive;
// };
//
// static error_t parse_opt(int key, char *arg, struct argp_state *state) {
//     struct arguments *arguments = state->input;
//     switch (key) {
//     case 'l': arguments->mode = LINE_MODE; break;
//     case 'w': arguments->mode = WORD_MODE; break;
//     case 'i': arguments->isCaseInsensitive = true; break;
//     case ARGP_KEY_ARG: return 0;
//     default: return ARGP_ERR_UNKNOWN;
//     }
//     return 0;
// }
//
// static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };


int main (int argc, char **argv) {
	// struct arguments arguments;
//
// 	arguments.mode = CHARACTER_MODE;
// 	arguments.isCaseInsensitive = false;
//
// 	argp_parse(&argp, argc, argv, 0, 0, &arguments);
// 	printf("die\n");
// 	/* code */
	
	DiceRollingSession *drs = dice_rolling_session_init();
	
	for(size_t i = 1; i < argc; ++i) {
			char *result = dice_rolling_session_resolve_notation(drs, argv[i]);
			printf("%s ", result);
			fflush(stdin);
			free(result);
	}
	printf("\n");

	dice_rolling_session_free(drs);
	
	return 0;
}