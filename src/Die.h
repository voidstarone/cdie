#ifndef DIE_H_FA033981
#define DIE_H_FA033981

typedef struct {
	int num_faces;
	int last_result;
} Die;

Die die_init(int faces);

int die_roll(Die *d);

int die_last_result(Die *d);
int die_faces(Die *d);


#endif /* end of include guard: DIE_H_FA033981 */

