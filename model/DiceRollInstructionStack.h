#include "DiceRollInstruction.h"

typedef struct {
	DiceRollInstruction *first_instruction;
} DiceRollInstructionStack;

DiceRollInstructionStack *dice_roll_instruction_stack_init();
void dice_roll_instruction_stack_free(DiceRollInstructionStack *dris);