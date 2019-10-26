#include "WorkItem.h"
#include "Emulator.h"

#define INST instruction->getBytes()->sop2

namespace SI {

// D.u = S0.u + S1.u. SCC = carry out.
void WorkItem::ISA_S_ADD_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the sum and carry out.
	carry.as_uint = __builtin_uadd_overflow(s0.as_uint, s1.as_uint, &sum.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, sum.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, sum.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", carry.as_uint);
	}
}

// D.u = S0.u - S1.u. SCC = carry out.
void WorkItem::ISA_S_SUB_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the sum and carry out.
	carry.as_uint = __builtin_usub_overflow(s0.as_uint, s1.as_uint, &sum.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, sum.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, sum.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", carry.as_uint);
	}
}

// D.u = S0.i + S1.i. scc = overflow.
void WorkItem::ISA_S_ADD_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register ovf;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the sum and overflow.
	ovf.as_uint = __builtin_sadd_overflow(s0.as_int, s1.as_int, &sum.as_int);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, sum.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, ovf.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, sum.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", ovf.as_uint);
	}
}

// D.u = S0.i - S1.i. scc = overflow.
void WorkItem::ISA_S_SUB_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register diff;
	Instruction::Register ovf;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the sum and overflow.
	ovf.as_uint = __builtin_ssub_overflow(s0.as_int, s1.as_int, &diff.as_int);

	// Write the results.
		// Store the data in the destination register
	WriteSReg(INST.sdst, diff.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, ovf.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d) ", INST.sdst, diff.as_int);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", ovf.as_uint);
	}
}

// D.u = S0.u + S1.u + SCC. SCC = carry out.
void WorkItem::ISA_S_ADDC_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register scc;
	Instruction::Register sum;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);
	scc.as_uint = ReadSReg(Instruction::RegisterScc);

	// Calculate the sum and carry out.
	carry.as_uint = __builtin_uadd_overflow(s0.as_uint, s1.as_uint, &sum.as_uint);
	carry.as_uint += __builtin_uadd_overflow(sum.as_uint, scc.as_uint, &sum.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, sum.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, sum.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", carry.as_uint);
	}
}

// D.u = S0.u - S1.u - SCC. SCC = carry out.
void WorkItem::ISA_S_SUBB_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register scc;
	Instruction::Register sum;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);
	scc.as_uint = ReadSReg(Instruction::RegisterScc);

	// Calculate the sum and carry out.
	carry.as_uint = __builtin_usub_overflow(s0.as_uint, s1.as_uint, &sum.as_uint);
	carry.as_uint += __builtin_usub_overflow(sum.as_uint, scc.as_uint, &sum.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, sum.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, sum.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", carry.as_uint);
	}
}

// D.u = (S0.u < S1.u) ? S0.u : S1.u, scc = 1 if S0 is min.
void WorkItem::ISA_S_MIN_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register min;
	Instruction::Register s0_min;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the minimum operand.
	if (s0.as_uint < s1.as_uint)
	{
		min.as_uint = s0.as_uint;
		s0_min.as_uint = 1;
	}
	else
	{
		min.as_uint = s1.as_uint;
		s0_min.as_uint = 0;
	}

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, min.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, s0_min.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, min.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%d) ", s0_min.as_uint);
	}
}

// D.i = (S0.i > S1.i) ? S0.i : S1.i, scc = 1 if S0 is max.
void WorkItem::ISA_S_MAX_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register max;
	Instruction::Register s0_max;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the maximum operand.
	// Is max defined as GT or GTE?
	if (s0.as_int > s1.as_int)
	{
		max.as_int = s0.as_int;
		s0_max.as_uint = 1;
	}
	else
	{
		max.as_int = s1.as_int;
		s0_max.as_uint = 0;
	}

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, max.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, s0_max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d) ", INST.sdst, max.as_int);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", s0_max.as_uint);
	}
}

// D.u = (S0.u > S1.u) ? S0.u : S1.u, scc = 1 if S0 is max.
void WorkItem::ISA_S_MAX_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register max;
	Instruction::Register s0_max;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Calculate the maximum operand.
	if (s0.as_uint > s1.as_uint)
	{
		max.as_uint = s0.as_uint;
		s0_max.as_uint = 1;
	}
	else
	{
		max.as_uint = s1.as_uint;
		s0_max.as_uint = 0;
	}

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, max.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, s0_max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%u) ", INST.sdst, max.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", s0_max.as_uint);
	}
}

// D.u = SCC ? S0.u : S1.u
void WorkItem::ISA_S_CSELECT_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register scc;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);
	scc.as_uint = ReadSReg(Instruction::RegisterScc);

	// Calculate the result
	result.as_uint = scc.as_uint ? s0.as_uint : s1.as_uint;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
	}
}

// D.u = S0.u & S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_AND_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;
	Instruction::Register nonzero;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	/* Bitwise AND the two operands and determine if the result is
	 * non-zero. */
	result.as_uint = s0.as_uint & s1.as_uint;
	nonzero.as_uint = ! !result.as_uint;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u & S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_AND_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Bitwise AND the two operands and determine if the result is
	 * non-zero. */
	result.as_ulong = s0.as_ulong & s1.as_ulong;
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, result.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u | S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_OR_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;
	Instruction::Register nonzero;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	/* Bitwise AND the two operands and determine if the result is
	 * non-zero. */
	result.as_uint = s0.as_uint | s1.as_uint;
	nonzero.as_uint = ! !result.as_uint;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u | S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_OR_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Bitwise OR the two operands and determine if the result is
	 * non-zero. */
	result.as_ulong = s0.as_ulong | s1.as_ulong;
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, result.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u ^ S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_XOR_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;
	Instruction::Register nonzero;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	/* Bitwise AND the two operands and determine if the result is
	 * non-zero. */
	result.as_uint = s0.as_uint ^ s1.as_uint;
	nonzero.as_uint = ! !result.as_uint;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u ^ S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_XOR_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Bitwise XOR the two operands and determine if the result is
	 * non-zero. */
	result.as_ulong = s0.as_ulong ^ s1.as_ulong;
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, result.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u & ~S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_ANDN2_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Bitwise AND the first operand with the negation of the second and
	 * determine if the result is non-zero. */
	result.as_ulong = s0.as_ulong & ~s1.as_ulong;
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, result.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

// D.u = S0.u | ~S1.u. scc = 1 if result is non-zero.
void WorkItem::ISA_S_ORN2_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Bitwise AND the first operand with the negation of the second and
	 * determine if the result is non-zero. */
	result.as_ulong = s0.as_ulong | ~s1.as_ulong;
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, result.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

// D.u = ~(S0.u & S1.u). scc = 1 if result is non-zero.
void WorkItem::ISA_S_NAND_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Bitwise AND the two operands and determine if the result is
	 * non-zero. */
	result.as_ulong = ~(s0.as_ulong & s1.as_ulong);
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	// Store the data in the destination register
	WriteSReg(INST.sdst + 1, result.hi.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", nonzero.as_uint);
	}
}

// D.u = S0.u << S1.u[4:0]. scc = 1 if result is non-zero.
void WorkItem::ISA_S_LSHL_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;
	Instruction::Register nonzero;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
	{
		s0.as_uint = INST.lit_cnst;
	}
	else
	{
		s0.as_uint = ReadSReg(INST.ssrc0);
	}
	if (INST.ssrc1 == 0xFF)
	{
		assert(INST.lit_cnst < 32);
		s1.as_uint = INST.lit_cnst;
	}
	else
	{
		s1.as_uint = ReadSReg(INST.ssrc1) & 0x1F;
	}

	/* Left shift the first operand by the second and determine if the
	 * result is non-zero. */
	result.as_uint = s0.as_uint << s1.as_uint;
	nonzero.as_uint = result.as_uint != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

// D.u = S0.u << S1.u[4:0]. scc = 1 if result is non-zero.
void WorkItem::ISA_S_LSHL_B64_Impl(Instruction *instruction)
{
	// Assert no literal constants for a 64 bit instruction.
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	Instruction::Register64 s0;
	Instruction::Register64 s1;
	Instruction::Register64 result;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0 = Read_SSRC_64(INST.ssrc0);
	s1 = Read_SSRC_64(INST.ssrc1);

	/* Left shift the first operand by the second and determine if the
	 * result is non-zero. */
	result.as_ulong = s0.as_ulong << (s1.as_ulong & 0x1F);
	nonzero.as_uint = result.as_ulong != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.lo.as_uint);
	WriteSReg(INST.sdst + 1, result.hi.as_uint);

	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<<(0x%x) ", INST.sdst, result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S%u<<(0x%x) ", INST.sdst + 1, result.hi.as_uint);
		Emulator::isa_debug << misc::fmt("scc<<(%u)", nonzero.as_uint);
	}
}

// D.u = S0.u >> S1.u[4:0]. scc = 1 if result is non-zero.
void WorkItem::ISA_S_LSHR_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;
	Instruction::Register nonzero;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
	{
		s0.as_uint = INST.lit_cnst;
	}
	else
	{
		s0.as_uint = ReadSReg(INST.ssrc0);
	}
	if (INST.ssrc1 == 0xFF)
	{
		assert(INST.lit_cnst < 32);
		s1.as_uint = INST.lit_cnst;
	}
	else
	{
		s1.as_uint = ReadSReg(INST.ssrc1) & 0x1F;
	}

	/* Right shift the first operand by the second and determine if the
	 * result is non-zero. */
	result.as_uint = s0.as_uint >> s1.as_uint;
	nonzero.as_uint = result.as_uint != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

// D.i = signext(S0.i) >> S1.i[4:0]. scc = 1 if result is non-zero.
void WorkItem::ISA_S_ASHR_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;
	Instruction::Register nonzero;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
	{
		s0.as_uint = INST.lit_cnst;
	}
	else
	{
		s0.as_uint = ReadSReg(INST.ssrc0);
	}
	if (INST.ssrc1 == 0xFF)
	{
		assert(INST.lit_cnst < 32);
		s1.as_uint = INST.lit_cnst;
	}
	else
	{
		s1.as_uint = ReadSReg(INST.ssrc1) & 0x1F;
	}

	/* Right shift the first operand sign extended by the second and
	 * determine if the result is non-zero. */
	result.as_int = s0.as_int >> s1.as_int;
	nonzero.as_uint = result.as_uint != 0;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d) ", INST.sdst, result.as_int);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
}

// D.u = ((1 << S0.u[4:0]) - 1) << S1.u[4:0]; bitfield mask
void WorkItem::ISA_S_BFM_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Multiply the two operands.
	result.as_int = ((1U << (s0.as_uint & 0x1F)) - 1) << (s1.as_uint & 0x1F);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d)", INST.sdst, result.as_int);
	}
}

// D.i = S0.i * S1.i.
void WorkItem::ISA_S_MUL_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadSReg(INST.ssrc0);
	if (INST.ssrc1 == 0xFF)
		s1.as_uint = INST.lit_cnst;
	else
		s1.as_uint = ReadSReg(INST.ssrc1);

	// Multiply the two operands.
	result.as_int = s0.as_int * s1.as_int;

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(%d)", INST.sdst, result.as_int);
	}
}

/* D.i = (S0.i >> S1.u[4:0]) & ((1 << S2.u[4:0]) - 1); bitfield extract,
 * S0=data, S1=field_offset, S2=field_width. */
void WorkItem::ISA_S_BFE_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;
	Instruction::Register full_reg;
	Instruction::Register nonzero;

	// Load operands from registers.
	s0.as_uint = ReadSReg(INST.ssrc0);
	full_reg.as_uint = ReadSReg(INST.ssrc1);

	/* s1 (offset) should be [4:0] of ssrc1 and s2 (width) should be [22:16] of ssrc1*/
	s1.as_uint = full_reg.as_uint & 0x1F;
	s2.as_uint = (full_reg.as_uint >> 16) & 0x7F;

	// Calculate the result.
	if (s2.as_uint == 0)
	{
		result.as_int = 0;
	}
	else if (s2.as_uint + s1.as_uint < 32)
	{
		result.as_int = (s0.as_int << (32 - s1.as_uint - s2.as_uint)) >> 
			(32 - s2.as_uint);
	}
	else
	{
		result.as_int = s0.as_int >> s1.as_uint;
	}

	nonzero.as_uint = result.as_uint != 0;
	
	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.sdst, result.as_uint);
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, nonzero.as_uint);


	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u<=(0x%x) ", INST.sdst, result.as_uint);
		Emulator::isa_debug << misc::fmt("scc<=(%u)", nonzero.as_uint);
	}
	
}

} // namespace SI
