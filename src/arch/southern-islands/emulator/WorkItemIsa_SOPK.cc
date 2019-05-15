#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->sopk

// D.i = signext(simm16).
void WorkItem::ISA_S_MOVK_I32_Impl(Instruction *instruction)
{
	Instruction::Register simm16;
	Instruction::Register result;

	// Load constant operand from instruction.
	simm16.as_ushort[0] = INST.simm16;

	// Sign extend the short constant to an integer.
	result.as_int = (int) simm16.as_short[0];

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d)", INST.sdst, result.as_int);
	}
}

// SCC = (D.u == SIMM16)
void WorkItem::ISA_S_CMPK_EQ_U32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = SCC = (D.u <= SIMM16)
void WorkItem::ISA_S_CMPK_LE_U32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.i = D.i + signext(SIMM16). scc = overflow.
void WorkItem::ISA_S_ADDK_I32_Impl(Instruction *instruction)
{
	Instruction::Register simm16;
	Instruction::Register sum;
	Instruction::Register ovf;
	Instruction::Register dst;

	int se_simm16;

	/* Load short constant operand from instruction and sign extend to an 
	 * integer. */
	simm16.as_ushort[0] = INST.simm16;
	se_simm16 = (int) simm16.as_short[0];

	// Load operand from destination register.
	dst.as_uint = ReadSReg(INST.sdst);

	// Add the two operands and determine overflow.
	sum.as_int = dst.as_int + se_simm16;
	ovf.as_uint = (dst.as_int >> 31 != se_simm16 >> 31) ? 0 :
		((dst.as_int > 0 && sum.as_int < 0) || 
		 (dst.as_int < 0 && sum.as_int > 0));

	// Write the results.
		// Store the data in the destination register
	WriteSReg(INST.sdst, sum.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, ovf.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d)", INST.sdst, sum.as_int);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", ovf.as_uint);
	}
}

// D.i = D.i * signext(SIMM16). scc = overflow.
void WorkItem::ISA_S_MULK_I32_Impl(Instruction *instruction)
{
	Instruction::Register simm16;
	Instruction::Register product;
	Instruction::Register ovf;
	Instruction::Register dst;

	int se_simm16;

	/* Load short constant operand from instruction and sign extend to an 
	 * integer. */
	simm16.as_ushort[0] = INST.simm16;
	se_simm16 = (int) simm16.as_short[0];

	// Load operand from destination register.
	dst.as_uint = ReadSReg(INST.sdst);

	// Multiply the two operands and determine overflow.
	product.as_int = dst.as_int * se_simm16;
	ovf.as_uint = ((long long) dst.as_int * (long long) se_simm16) > 
		(long long) product.as_int;

	// Write the results.
		// Store the data in the destination register
	WriteSReg(INST.sdst, product.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, ovf.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d)", INST.sdst, product.as_int);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", ovf.as_uint);
	}
}

// D.u = hardware register. Read some or all of a hardware register into the
// LSBs of D. See Table 5.7 on page 5-7. SIMM16 = {size[4:0], offset[4:0],
// hwRegId[5:0]}; offset is 0-31, size is 1-32.
void WorkItem::ISA_S_GETREG_B32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// hardware register = D.u. Write some or all of the LSBs of D into a hardware
// register (note that D is a source SGPR). See Table 5.7 on page 5-7. SIMM16
// = {size[4:0], offset[4:0], hwRegId[5:0]}; offset is 0-31, size is 1-32.
void WorkItem::ISA_S_SETREG_B32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// This instruction uses a 32-bit literal oconstant. Write some or all of the
// LSBs of IMM32 into a hardware register. SIMM16 = {size[4:0], offset[4:0],
// hwRegId[5:0]}; offset is 0-31, size is 1-32.
void WorkItem::ISA_S_SETREG_IMM32_B32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

} // namespace SI
