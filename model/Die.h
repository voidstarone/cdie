#ifndef DIE_H_FA033981
#define DIE_H_FA033981

typedef struct {
	size_t num_faces;
	unsigned int last_result;
} Die;

Die * die_create(size_t faces);

void die_free(Die *d);

unsigned int die_roll(Die *d);

unsigned int die_last_result(Die *d);
size_t die_faces(Die *d);


#endif /* end of include guard: DIE_H_FA033981 */

