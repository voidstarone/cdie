#include <stdlib.h>

#include "DieFactory.h"


Die ** diefactory_make_die_array(size_t sides, size_t count) {
	Die **dice = malloc(sizeof(Die *) * count);
	for(size_t i = 0; i < count; ++i) {
		dice[i] = die_create(sides);
	}
	return dice;
}