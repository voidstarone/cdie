#include <stdlib.h>
#include "Die.h"

int die_last_result(Die *d) {
	return d->last_result;
}

int die_faces(Die *d) {
	return d->num_faces;
}


Die die_init(int numFaces) {
	Die d;
	d.num_faces = numFaces;
	d.last_result = -1;
	return d;
}

int die_roll(Die *d) {
	d->last_result = arc4random_uniform(d->num_faces)+1;
	return d->last_result;
}