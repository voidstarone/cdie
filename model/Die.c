#include <stdlib.h>
#include "Die.h"

unsigned int die_last_result(Die *d) {
	return d->last_result;
}

size_t die_faces(Die *d) {
	return d->num_faces;
}

Die * die_create(size_t numFaces) {
	Die *d = malloc(sizeof(Die));
	d->num_faces = numFaces;
	return d;
}

unsigned int die_roll(Die *d) {
	d->last_result = arc4random_uniform((unsigned int) d->num_faces) + 1;
	return d->last_result;
}

void die_free(Die *d) {
	free(d);
	d = NULL;
}
