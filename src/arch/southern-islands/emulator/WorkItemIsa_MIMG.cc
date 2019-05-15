#include "WorkItem.h"

namespace SI {

#define INST instruction->getBytes()->mimg

/*
 * MIMG
 */

void WorkItem::ISA_IMAGE_STORE_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

void WorkItem::ISA_IMAGE_SAMPLE_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

} // namespace SI
