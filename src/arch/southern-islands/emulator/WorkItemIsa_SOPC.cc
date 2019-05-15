#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->sopc

// scc = (S0.i == S1.i).
void WorkItem::ISA_S_CMP_EQ_I32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_int == s1.as_int);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: scc<=(%u) (%u ==? %u)", 
			wavefront->getId(), result.as_uint, s0.as_int,
			s1.as_int);
	}
}

// scc = (S0.i > S1.i).
void WorkItem::ISA_S_CMP_GT_I32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_int > s1.as_int);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) (%u >? %u) ", result.as_uint,
			s0.as_uint, s1.as_uint);
	}
}

// scc = (S0.i >= S1.i).
void WorkItem::ISA_S_CMP_GE_I32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_int >= s1.as_int);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

// scc = (S0.i < S1.i).
void WorkItem::ISA_S_CMP_LT_I32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_int < s1.as_int);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

// scc = (S0.i <= S1.i).
void WorkItem::ISA_S_CMP_LE_I32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_int <= s1.as_int);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

// scc = (S0.u == S1.u).
void WorkItem::ISA_S_CMP_EQ_U32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_uint == s1.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: scc<=(%u) (%u ==? %u)", 
			wavefront->getId(), result.as_uint, s0.as_int,
			s1.as_int);
	}
}

// scc = (S0.u != S1.u).
void WorkItem::ISA_S_CMP_LG_U32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_uint != s1.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: scc<=(%u) (%u ==? %u)", 
			wavefront->getId(), result.as_uint, s0.as_int,
			s1.as_int);
	}
}

// scc = (S0.u > S1.u).
void WorkItem::ISA_S_CMP_GT_U32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_uint > s1.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

// scc = (S0.u >= S1.u).
void WorkItem::ISA_S_CMP_GE_U32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_uint >= s1.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

// scc = (S0.u < S1.u).
void WorkItem::ISA_S_CMP_LT_U32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_uint < s1.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

// scc = (S0.u <= S1.u).
void WorkItem::ISA_S_CMP_LE_U32_Impl(Instruction *instruction)
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

	// Compare the operands.
	result.as_uint = (s0.as_uint <= s1.as_uint);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(Instruction::RegisterScc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("scc<=(%u) ", result.as_uint);
	}
}

} // namespace SI
