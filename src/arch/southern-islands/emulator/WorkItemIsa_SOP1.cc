#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->sop1

// D.u = S0.u.
void WorkItem::ISA_S_MOV_B64_Impl(Instruction *instruction)
{
	// Assert no literal constant with a 64 bit instruction.
	// assert(!(INST.ssrc0 == 0xFF));

	Instruction::Register64 s0;

	// Load operand from registers.
	if (INST.ssrc0 == 0xFF){
		s0.as_ulong = (uint64_t)(int64_t)(int32_t)INST.lit_cnst;
	}
	else{
		s0 = Read_SSRC_64(INST.ssrc0);
	}

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, s0.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, s0.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, s0.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x)", INST.sdst + 1, s0.hi.as_uint);
	}
}

/*
 * SOP1
 */

// D.u = S0.u.
void WorkItem::ISA_S_MOV_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;

	// Load operand from registers or as a literal constant.
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, s0.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, s0.as_uint);
	}
}

// D.u = ~S0.u SCC = 1 if result non-zero.
void WorkItem::ISA_S_NOT_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register nonzero;

	// Load operand from registers or as a literal constant.
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = ~INST.lit_cnst;
	else
		s0.as_uint = ~ReadSReg(INST.ssrc0);
	nonzero.as_uint = ! !s0.as_uint;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, s0.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, s0.as_uint);
	}
}

// D.u = WholeQuadMode(S0.u). SCC = 1 if result is non-zero.
void WorkItem::ISA_S_WQM_B64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = S0.u[0:31] (reverse bits).
void WorkItem::ISA_S_BREV_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;

	// Load operand from registers or as a literal constant.
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	s0.as_uint = misc::BitReverse32(s0.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, s0.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, s0.as_uint);
	}
}


// D.u[S0.u[4:0]] = 0.
void WorkItem::ISA_S_BITSET0_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register dst;

	// Load operand from registers or as a literal constant.
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	dst.as_uint = ReadSReg(INST.sdst);
	dst.as_uint &= ~(1U << (s0.as_uint & 0x1F));

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, dst.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, s0.as_uint);
	}
}

// D.u = PC + 4; destination receives the byte address of the next instruction.
void WorkItem::ISA_S_GETPC_B64_Impl(Instruction *instruction)
{
	Instruction::Register64 dst;

	uint32_t pc = wavefront->getPC() + 4;
	dst.lo.as_uint = pc;
	dst.hi.as_uint = 0;

	WriteSReg(INST.sdst, dst.lo.as_uint);
	WriteSReg(INST.sdst + 1, dst.hi.as_uint);

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, dst.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, dst.hi.as_uint);
	}
}

// PC = S0.u; S0.u is a byte address of the instruction to jump to.
void WorkItem::ISA_S_SETPC_B64_Impl(Instruction *instruction)
{
	Instruction::Register64 s0;

	s0 = Read_SSRC_64(INST.ssrc0);
	// Check if we aren't addressing more than 32 bits.
	assert(s0.as_ulong == s0.lo.as_uint);

	wavefront->setPC(s0.lo.as_uint);
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("PC<=(0x%x)", wavefront->getPC());
	}
}

// D.u = PC + 4, PC = S0.u
void WorkItem::ISA_S_SWAPPC_B64_Impl(Instruction *instruction)
{
	Instruction::Register64 s0;

	// FIXME: cuurently PC is implemented as 32-bit offset
	// Load operands from registers
	s0 = Read_SSRC_64(INST.ssrc0);

	// Write the results
	// Store the data in the destination register
	WriteSReg(INST.sdst, wavefront->getPC() + 4);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, 0);

	// Set the new PC
	unsigned pc = wavefront->getPC();
	wavefront->setPC(s0.lo.as_uint - 4);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, pc + 4);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, 0);
		Emulator::isa_debug << misc::fmt("PC<=(0x%x)", wavefront->getPC());
	}
}

/* D.u = EXEC, EXEC = S0.u & EXEC. scc = 1 if the new value of EXEC is
 * non-zero. */
void WorkItem::ISA_S_AND_SAVEEXEC_B64_Impl(Instruction *instruction)
{
	// Assert no literal constant with a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF));

	Instruction::Register64 exec;
	Instruction::Register64 s0;
	Instruction::Register64 exec_new;
	Instruction::Register nonzero;

	// Load operands from registers.
	exec = Read_SSRC_64(Instruction::RegisterExec);
	s0 = Read_SSRC_64(INST.ssrc0);

	/* Bitwise AND exec and the first operand and determine if the result 
	 * is non-zero. */
	exec_new.as_ulong = s0.as_ulong & exec.as_ulong;
	nonzero.as_uint = exec_new.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, exec.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, exec.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterExec, exec_new.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterExec + 1, exec_new.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, exec.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, exec.hi.as_uint);
		Emulator::isa_debug << misc::fmt("exec.lo<=(0x%x) ", exec_new.lo.as_uint);
		Emulator::isa_debug << misc::fmt("exec.hi<=(0x%x) ", exec_new.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

/* D.u = EXEC, EXEC = S0.u | EXEC. scc = 1 if the new value of EXEC is
 * non-zero. */
void WorkItem::ISA_S_OR_SAVEEXEC_B64_Impl(Instruction *instruction)
{
	// Assert no literal constant with a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF));

	Instruction::Register64 exec;
	Instruction::Register64 s0;
	Instruction::Register64 exec_new;
	Instruction::Register nonzero;

	// Load operands from registers.
	exec = Read_SSRC_64(Instruction::RegisterExec);
	s0 = Read_SSRC_64(INST.ssrc0);

	/* Bitwise OR exec and the first operand and determine if the result 
	 * is non-zero. */
	exec_new.as_ulong = s0.as_ulong | exec.as_ulong;
	nonzero.as_uint = exec_new.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, exec.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, exec.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterExec, exec_new.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterExec + 1, exec_new.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, exec.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, exec.hi.as_uint);
		Emulator::isa_debug << misc::fmt("exec.lo<=(0x%x) ", exec_new.lo.as_uint);
		Emulator::isa_debug << misc::fmt("exec.hi<=(0x%x) ", exec_new.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

} // namespace SI
