#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->vop2

/* D.u = VCC[i] ? S1.u : S0.u (i = threadID in wave); VOP3: specify VCC as a
 * scalar GPR in S2. */
void WorkItem::ISA_V_CNDMASK_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	int vcci;

	// Load operands from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);
	vcci = ReadBitmaskSReg(Instruction::RegisterVcc);

	// Calculate the result.
	result.as_uint = (vcci) ? s1.as_uint : s0.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			result.as_uint);
	}
}

// D.f = S0.f + S1.f.
void WorkItem::ISA_V_ADD_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the sum.
	sum.as_float = s0.as_float + s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			sum.as_float);
	}
}

// D.f = S0.f - S1.f.
void WorkItem::ISA_V_SUB_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register dif;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the difference.
	dif.as_float = s0.as_float - s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, dif.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			dif.as_float);
	}
}

// D.f = S1.f - S0.f.
void WorkItem::ISA_V_SUBREV_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register dif;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the difference.
	dif.as_float = s1.as_float - s0.as_float;

	// Write the results.
	WriteVReg(INST.vdst, dif.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			dif.as_float);
	}
}

// D.f = S0.F * S1.f + D.f.
void WorkItem::ISA_V_MAC_LEGACY_F32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}


// D.f = S0.f * S1.f (DX9 rules, 0.0*x = 0.0).
void WorkItem::ISA_V_MUL_LEGACY_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register product;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the product.
	if (s0.as_float == 0.0f || s1.as_float == 0.0f)
	{
		product.as_float = 0.0f;
	}
	else
	{
		product.as_float = s0.as_float * s1.as_float;
	}

	// Write the results.
	WriteVReg(INST.vdst, product.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) (%gf * %gf) ", id, 
			INST.vdst, product.as_float, s0.as_float, s1.as_float);
	}
}

// D.f = S0.f * S1.f.
void WorkItem::ISA_V_MUL_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register product;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the product.
	product.as_float = s0.as_float * s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, product.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) (%gf*%gf)", id, 
			INST.vdst, product.as_float, s0.as_float, s1.as_float);
	}
}

// D.i = S0.i[23:0] * S1.i[23:0].
void WorkItem::ISA_V_MUL_I32_I24_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register product;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Truncate operands to 24-bit signed integers
	s0.as_uint = misc::SignExtend32(s0.as_uint, 24);
	s1.as_uint = misc::SignExtend32(s1.as_uint, 24);

	// Calculate the product.
	product.as_int = s0.as_int * s1.as_int;

	// Write the results.
	WriteVReg(INST.vdst, product.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d)", id, INST.vdst,
			product.as_int);
	}
}

// D.u = S0.u[23:0] * S1.u[23:0].
void WorkItem::ISA_V_MUL_U32_U24_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register product;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Truncate operands to 24-bit unsigned integers
	s0.as_uint = s0.as_uint & 0x00FFFFFF;
	s1.as_uint = s1.as_uint & 0x00FFFFFF;

	// Calculate the product.
	product.as_int = s0.as_uint * s1.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, product.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d)", id, INST.vdst,
			product.as_int);
	}
}

// D.f = max(S0.f, S1.f) (DX9 rules for NaN).
void WorkItem::ISA_V_MAX_LEGACY_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register max;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the minimum operand.
	if (s0.as_float < s1.as_float)
	{
		max.as_float = s1.as_float;
	}
	else
	{
		max.as_float = s0.as_float;
	}

	// Write the results.
	WriteVReg(INST.vdst, max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf)", id, INST.vdst,
			max.as_float);
	}
}

// D.f = min(S0.f, S1.f).
void WorkItem::ISA_V_MIN_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register min;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the minimum operand.
	if (s0.as_float < s1.as_float)
	{
		min.as_float = s0.as_float;
	}
	else
	{
		min.as_float = s1.as_float;
	}

	// Write the results.
	WriteVReg(INST.vdst, min.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf)", id, INST.vdst,
			min.as_float);
	}
}

// D.f = max(S0.f, S1.f).
void WorkItem::ISA_V_MAX_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register max;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the minimum operand.
	if (s0.as_float > s1.as_float)
	{
		max.as_float = s0.as_float;
	}
	else
	{
		max.as_float = s1.as_float;
	}

	// Write the results.
	WriteVReg(INST.vdst, max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf)", id, INST.vdst,
			max.as_float);
	}
}

// D.i = max(S0.i, S1.i).
void WorkItem::ISA_V_MAX_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register max;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the minimum operand.
	if (s0.as_int > s1.as_int)
	{
		max.as_int = s0.as_int;
	}
	else
	{
		max.as_int = s1.as_int;
	}

	// Write the results.
	WriteVReg(INST.vdst, max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d)", id, INST.vdst,
			max.as_int);
	}
}

// D.i = min(S0.i, S1.i).
void WorkItem::ISA_V_MIN_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register min;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the minimum operand.
	if (s0.as_int < s1.as_int)
	{
		min.as_int = s0.as_int;
	}
	else
	{
		min.as_int = s1.as_int;
	}

	// Write the results.
	WriteVReg(INST.vdst, min.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d)", id, INST.vdst,
			min.as_int);
	}
}

// D.u = min(S0.u, S1.u).
void WorkItem::ISA_V_MIN_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register min;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the minimum operand.
	if (s0.as_uint < s1.as_uint)
	{
		min.as_uint = s0.as_uint;
	}
	else
	{
		min.as_uint = s1.as_uint;
	}

	// Write the results.
	WriteVReg(INST.vdst, min.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)", id, INST.vdst,
			min.as_uint);
	}
}

// D.u = max(S0.u, S1.u).
void WorkItem::ISA_V_MAX_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register max;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the maximum operand.
	if (s0.as_uint > s1.as_uint)
	{
		max.as_uint = s0.as_uint;
	}
	else
	{
		max.as_uint = s1.as_uint;
	}

	// Write the results.
	WriteVReg(INST.vdst, max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)", id, INST.vdst,
			max.as_uint);
	}
}

// D.u = S1.u >> S0.u[4:0].
void WorkItem::ISA_V_LSHRREV_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
	{
		assert(INST.lit_cnst < 32);
		s0.as_uint = INST.lit_cnst;
	}
	else
	{
		s0.as_uint = ReadReg(INST.src0) & 0x1F;
	}
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Right shift s1 by s0.
	result.as_uint = s1.as_uint >> s0.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) (%u >> %u) ", id,
			INST.vdst, result.as_uint, s1.as_uint, s0.as_uint);
	}
}

// D.i = S1.i >> S0.i[4:0].
void WorkItem::ISA_V_ASHRREV_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
	{
		assert(INST.lit_cnst < 32);
		s0.as_uint = INST.lit_cnst & 0x1F;
	}
	else
	{
		s0.as_uint = ReadReg(INST.src0) & 0x1F;
	}
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Right shift s1 by s0.
	result.as_int = s1.as_int >> s0.as_int;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d, %u) ", id, INST.vdst,
			result.as_int, result.as_uint);
	}
}

// D.u = S0.u << S1.u[4:0].
void WorkItem::ISA_V_LSHL_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1) & 0x1F;

	// Left shift s1 by s0.
	result.as_uint = s0.as_uint << s1.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			result.as_uint);
	}
}

// D.u = S1.u << S0.u[4:0].
void WorkItem::ISA_V_LSHLREV_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
	{
		assert(INST.lit_cnst < 32);
		s0.as_uint = INST.lit_cnst;
	}
	else
	{
		s0.as_uint = ReadReg(INST.src0) & 0x1F;
	}
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Left shift s1 by s0.
	result.as_uint = s1.as_uint << s0.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) (%u << %u) ", id,
			INST.vdst, result.as_uint, s1.as_uint, s0.as_uint);
	}
}

// D.u = S0.u & S1.u.
void WorkItem::ISA_V_AND_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
	{
		s0.as_uint = INST.lit_cnst;
	}
	else
	{
		s0.as_uint = ReadReg(INST.src0);
	}
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Bitwise OR the two operands.
	result.as_uint = s0.as_uint & s1.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) (%u & %u) ", id, 
			INST.vdst, result.as_uint, s0.as_uint, s1.as_uint);
	}
}

// D.u = S0.u | S1.u.
void WorkItem::ISA_V_OR_B32_Impl(Instruction *instruction)
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

	// Bitwise OR the two operands.
	result.as_uint = s0.as_uint | s1.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) (%u | %u) ", 
			id, INST.vdst, result.as_uint, s0.as_uint,
			s1.as_uint);
	}
}

// D.u = S0.u ^ S1.u.
void WorkItem::ISA_V_XOR_B32_Impl(Instruction *instruction)
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

	// Bitwise OR the two operands.
	result.as_uint = s0.as_uint ^ s1.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%u ^ %u) ", id, 
			INST.vdst, result.as_uint, s0.as_uint, s1.as_uint);
	}
}

//D.u = ((1<<S0.u[4:0])-1) << S1.u[4:0]; S0=bitfield_width, S1=bitfield_offset.
void WorkItem::ISA_V_BFM_B32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = S0.f * S1.f + D.f.
void WorkItem::ISA_V_MAC_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register dst;
	Instruction::Register result;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);
	dst.as_uint = ReadVReg(INST.vdst);

	// Calculate the result.
	result.as_float = s0.as_float * s1.as_float + dst.as_float;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			result.as_float);
	}
}

// D.f = S0.f * K + S1.f; K is a 32-bit inline constant
void WorkItem::ISA_V_MADMK_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register K;
	Instruction::Register dst;

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);
	K.as_uint = INST.lit_cnst;
	
	// Calculate the result
	dst.as_float = s0.as_float * K.as_float + s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, dst.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%f) (%f * %f + %f)", id, 
			INST.vdst, dst.as_float, s0.as_float, K.as_float, 
			s1.as_float);
	}
}

// D.f = S0.f * S1.f + K; K is a 32-bit inline constant
void WorkItem::ISA_V_MADAK_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register K;
	Instruction::Register dst;

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);
	K.as_uint = INST.lit_cnst;
	
	// Calculate the result
	dst.as_float = s0.as_float * s1.as_float + K.as_float;

	// Write the results.
	WriteVReg(INST.vdst, dst.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%f) (%f * %f + %f)", id, 
			INST.vdst, dst.as_float, s0.as_float, K.as_float, 
			s1.as_float);
	}
}

// D.u = S0.u + S1.u, vcc = carry-out.
void WorkItem::ISA_V_ADD_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the sum and carry.
	carry.as_uint = __builtin_sadd_overflow(s0.as_int, s1.as_int, &sum.as_int);

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);
	WriteBitmaskSReg(Instruction::RegisterVcc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) (%d + %d) ", id, 
			INST.vdst, sum.as_int, s0.as_int, s1.as_int);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry.as_uint);
	}
}

// D.u = S0.u - S1.u; vcc = carry-out.
void WorkItem::ISA_V_SUB_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register dif;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the difference and carry.
	carry.as_uint = __builtin_ssub_overflow(s0.as_int, s1.as_int, &dif.as_int);

	// Write the results.
	WriteVReg(INST.vdst, dif.as_uint);
	WriteBitmaskSReg(Instruction::RegisterVcc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			dif.as_int);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry.as_uint);
	}
}

// D.u = S1.u - S0.u; vcc = carry-out.
void WorkItem::ISA_V_SUBREV_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register dif;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Calculate the difference and carry.
	dif.as_int = s1.as_int - s0.as_int;
	carry.as_uint = (s0.as_int > s1.as_int);

	// Write the results.
	WriteVReg(INST.vdst, dif.as_uint);
	WriteBitmaskSReg(Instruction::RegisterVcc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			dif.as_int);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry.as_uint);
	}
}

// D.u = S0.u + S1.u + VCC
void WorkItem::ISA_V_ADDC_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register vcc;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);
	vcc.as_uint = ReadReg(Instruction::RegisterVcc);

	// Calculate the sum and carry. XXX(rzl): is there a better way than to
	// use uint64_t?
	uint64_t sum = (uint64_t)s0.as_uint + (uint64_t)s1.as_uint + vcc.as_uint;
	carry.as_uint = sum > (uint32_t)sum;

	// Write the results.
	WriteVReg(INST.vdst, (uint32_t)sum);
	WriteBitmaskSReg(Instruction::RegisterVcc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			(uint32_t)sum);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry.as_uint);
	}
}

// D.u = S0.u - S1.u - VCC
void WorkItem::ISA_V_SUBB_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register vcc;
	Instruction::Register result;
	Instruction::Register carry;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);
	vcc.as_uint = ReadReg(Instruction::RegisterVcc);

	carry.as_uint = __builtin_usub_overflow(s0.as_uint, s1.as_uint, &result.as_uint);
	carry.as_uint += __builtin_usub_overflow(result.as_uint, vcc.as_uint, &result.as_uint);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);
	WriteBitmaskSReg(Instruction::RegisterVcc, carry.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			result.as_uint);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry.as_uint);
	}
}

// D = {flt32_to_flt16(S1.f),flt32_to_flt16(S0.f)}, with round-toward-zero.
void WorkItem::ISA_V_CVT_PKRTZ_F16_F32_Impl(Instruction *instruction)
{
	union hfpack
	{
		unsigned as_uint32;
		struct
		{
			unsigned short s1f;
			unsigned short s0f;
		} as_f16f16;
	};

	Instruction::Register s0;
	Instruction::Register s1;
	unsigned short s0f;
	unsigned short s1f;
	union hfpack float_pack;

	// Load operands from registers or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadVReg(INST.vsrc1);

	// Convert to half float
	s0f = Float32to16(s0.as_float);
	s1f = Float32to16(s1.as_float);
	float_pack.as_f16f16.s0f = s0f;
	float_pack.as_f16f16.s1f = s1f;

	// Write the results.
	WriteVReg(INST.vdst, float_pack.as_uint32);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			float_pack.as_uint32);
	}	
}

} // namespace SI
