#include "WorkItem.h"
#include "Emulator.h"

#include <cmath>

namespace SI {

#define INST instruction->getBytes()->vopc

// vcc = (S0.f < S1.f).
void WorkItem::ISA_V_CMP_LT_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_float < s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.f == S1.f).
void WorkItem::ISA_V_CMP_EQ_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_float == s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.f > S1.f).
void WorkItem::ISA_V_CMP_GT_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_float > s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.f >= S1.f).
void WorkItem::ISA_V_CMP_GE_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_float >= s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}	
}

// vcc = (!isNaN(S0) && !isNaN(S1)).
void WorkItem::ISA_V_CMP_O_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = !std::isnan(s0.as_float) && !std::isnan(s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}


// vcc = !(S0.f > S1.f).
void WorkItem::ISA_V_CMP_NGT_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = !(s0.as_float > s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = !(S0.f == S1.f).
void WorkItem::ISA_V_CMP_NEQ_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = !(s0.as_float == s1.as_float);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.d < S1.d).
void WorkItem::ISA_V_CMP_LT_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = (S0.d == S1.d).
void WorkItem::ISA_V_CMP_EQ_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = (S0.d <= S1.d).
void WorkItem::ISA_V_CMP_LE_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = (S0.d > S1.d).
void WorkItem::ISA_V_CMP_GT_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = !(S0.d >= S1.d).
void WorkItem::ISA_V_CMP_NGE_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = !(S0.d == S1.d).
void WorkItem::ISA_V_CMP_NEQ_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = !(S0.d < S1.d). 
void WorkItem::ISA_V_CMP_NLT_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}


// vcc = (S0.i < S1.i).
void WorkItem::ISA_V_CMP_LT_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_int < s1.as_int);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.i == S1.i).
void WorkItem::ISA_V_CMP_EQ_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_int == s1.as_int);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.i <= S1.i).
void WorkItem::ISA_V_CMP_LE_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_int <= s1.as_int);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.i > S1.i).
void WorkItem::ISA_V_CMP_GT_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 255)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_int > s1.as_int);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.i <> S1.i).
void WorkItem::ISA_V_CMP_NE_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_int != s1.as_int);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// D.u = (S0.i >= S1.i).
void WorkItem::ISA_V_CMP_GE_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_int >= s1.as_int);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// D = IEEE numeric class function specified in S1.u, performed on S0.d.
void WorkItem::ISA_V_CMP_CLASS_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// vcc = (S0.u < S1.u).
void WorkItem::ISA_V_CMP_LT_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_uint < s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.u == S1.u).
void WorkItem::ISA_V_CMP_EQ_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_uint == s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.u <= S1.u).
void WorkItem::ISA_V_CMP_LE_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_uint <= s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// vcc = (S0.u > S1.u).
void WorkItem::ISA_V_CMP_GT_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_uint > s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// D.u = (S0.f != S1.f).
void WorkItem::ISA_V_CMP_NE_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_uint != s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}


void WorkItem::ISA_V_CMP_GE_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Compare the operands.
	result.as_uint = (s0.as_uint >= s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

// D.u = (S0 <> S1)
void WorkItem::ISA_V_CMP_LG_U64_Impl(Instruction *instruction)
{
	Instruction::Register64 s0, s1;
	Instruction::Register result;

	if (INST.src0 == 0xFF) {
		s0.as_ulong = (uint64_t)(int64_t)(int32_t)INST.lit_cnst;
	} else {
		s0 = Read_SRC_64(INST.src0);
	}
	s1 = Read_VSRC_64(INST.vsrc1);

	result.as_uint = s0.as_ulong != s1.as_ulong;
	WriteBitmaskSReg(Instruction::RegisterVcc, result.as_uint);

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: vcc<=(%u) ",
			id_in_wavefront, result.as_uint);
	}
}

} // namespace SI
