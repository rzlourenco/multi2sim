#include "WorkItem.h"

namespace SI {

#define INST    instruction->getBytes()->exp

/*
 * EXPORT
 */

void WorkItem::ISA_EXPORT_Impl(Instruction *instruction)
{
	// Left unimplemented because this instruction uses OpenGL
	ISAUnimplemented(instruction);
}

} // namespace SI
