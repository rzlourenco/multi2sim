#include "WorkItem.h"
#include "Emulator.h"

#include <cmath>

namespace SI {

#define INST instruction->getBytes()->vop3a

/* D.u = VCC[i] ? S1.u : S0.u (i = threadID in wave); VOP3: specify VCC as a
 * scalar GPR in S2. */
void WorkItem::ISA_V_CNDMASK_B32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	int vcci;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	vcci = ReadBitmaskSReg(INST.src2);

	// Perform "floating-point negation"
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Calculate the result.
	result.as_uint = (vcci) ? s1.as_uint : s0.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u, %gf) (s1=%u, s0=%u)", 
			id, INST.vdst, result.as_uint, 
			result.as_float, s1.as_uint, s0.as_uint);
	}
}

// D.f = S0.f + S1.f.
void WorkItem::ISA_V_ADD_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

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
void WorkItem::ISA_V_SUB_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Calculate the sum.
	sum.as_float = s0.as_float - s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			sum.as_float);
	}
}

// D.f = S1.f - S0.f
void WorkItem::ISA_V_SUBREV_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register diff;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Calculate the diff.
	diff.as_float = s1.as_float - s0.as_float;

	// Write the results.
	WriteVReg(INST.vdst, diff.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			diff.as_float);
	}
}

// D.f = S0.f * S1.f (DX9 rules, 0.0*x = 0.0).
void WorkItem::ISA_V_MUL_LEGACY_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register product;

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

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
void WorkItem::ISA_V_MUL_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Calculate the result.
	result.as_float = s0.as_float * s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) (%gf*%gf)", id, 
			INST.vdst, result.as_float, s0.as_float, s1.as_float);
	}
}

// D.f = S0. * S1..
void WorkItem::ISA_V_MUL_I32_I24_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register product;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers or as a literal constant.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

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

// D.f = max(S0.f, S1.f).
void WorkItem::ISA_V_MAX_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Calculate the result.
	result.as_float = (s0.as_float > s1.as_float) ? 
		s0.as_float : s1.as_float;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			result.as_float);
	}
}

// D.u = S0.u | S1.u.
void WorkItem::ISA_V_OR_B32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.src2);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);
	assert(!INST.clamp);

	// Load operands from registers
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

        result.as_uint = s0.as_uint | s1.as_uint;

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
void WorkItem::ISA_V_MADMK_F32_VOP3a_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = S0.f * S1.f + K; K is a 32-bit inline constant
void WorkItem::ISA_V_MADAK_F32_VOP3a_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = S0.f * S1.f + S2.f.
void WorkItem::ISA_V_MAD_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	if (INST.abs & 4)
		s2.as_float = fabsf(s2.as_float);

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	if (INST.neg & 4)
		s2.as_float = -s2.as_float;

	// Calculate the result.
	result.as_float = s0.as_float * s1.as_float + s2.as_float;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			result.as_float);
	}
}

// D.u = S0.u[23:0] * S1.u[23:0] + S2.u[31:0].
void WorkItem::ISA_V_MAD_U32_U24_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	s0.as_uint = s0.as_uint & 0x00FFFFFF;
	s1.as_uint = s1.as_uint & 0x00FFFFFF;

	// Calculate the result.
	result.as_uint = s0.as_uint * s1.as_uint + s2.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			result.as_uint);
	}
}

/* D.u = (S0.u >> S1.u[4:0]) & ((1 << S2.u[4:0]) - 1); bitfield extract,
 * S0=data, S1=field_offset, S2=field_width. */
void WorkItem::ISA_V_BFE_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	s1.as_uint = s1.as_uint & 0x1F;
	s2.as_uint = s2.as_uint & 0x1F;

	// Calculate the result.
	result.as_uint = (s0.as_uint >> s1.as_uint) & ((1 << s2.as_uint) - 1);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: value:s0:%u, offset:s1:%u, width:s2:%u ",
			id, s0.as_uint, s1.as_uint, s2.as_uint);
		Emulator::isa_debug << misc::fmt("V%u<=(0x%x) ", INST.vdst, result.as_uint);
	}
}

/* D.i = (S0.i >> S1.u[4:0]) & ((1 << S2.u[4:0]) - 1); bitfield extract,
 * S0=data, S1=field_offset, S2=field_width. */
void WorkItem::ISA_V_BFE_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	s1.as_uint = s1.as_uint & 0x1F;
	s2.as_uint = s2.as_uint & 0x1F;

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

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			result.as_int);
	}
}

// D.u = (S0.u & S1.u) | (~S0.u & S2.u).
void WorkItem::ISA_V_BFI_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	// Calculate the result.
	result.as_uint = (s0.as_uint & s1.as_uint) | 
		(~s0.as_uint & s2.as_uint);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			result.as_uint);
	}
}

// D.f = S0.f * S1.f + S2.f
void WorkItem::ISA_V_FMA_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	if (INST.abs & 4)
		s2.as_float = fabsf(s2.as_float);

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	if (INST.neg & 4)
		s2.as_float = -s2.as_float;

	// FMA
	result.as_float = (s0.as_float * s1.as_float) + s2.as_float;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) (%gf*%gf + %gf) ", 
			id, INST.vdst, result.as_float, 
			s0.as_float, s1.as_float, s2.as_float);
	}
}

// D.d = S0.d * S1.d + S2.d
void WorkItem::ISA_V_FMA_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
void WorkItem::ISA_V_ALIGNBIT_B32_Impl(Instruction *instruction)
{
	Instruction::Register src2;
	Instruction::Register result;

	union
	{
		unsigned long long as_b64;
		unsigned as_reg[2];

	} src;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	src.as_reg[0] = ReadReg(INST.src1);
	src.as_reg[1] = ReadReg(INST.src0);
	src2.as_uint = ReadReg(INST.src2);
	src2.as_uint = src2.as_uint & 0x1F;

	// ({S0,S1} >> S2.u[4:0]) & 0xFFFFFFFF.
	result.as_uint = (src.as_b64 >> src2.as_uint) & 0xFFFFFFFF;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x, %u) ({0x%x,0x%x} >> %u) ",
			id, INST.vdst, result.as_uint, 
			result.as_uint, src.as_reg[1], src.as_reg[0], 
			src2.as_uint);
	}
}

/* 
 *D.d = Special case divide fixup and flags(s0.d = Quotient, s1.d = Denominator, s2.d = Numerator).
 */
void WorkItem::ISA_V_DIV_FIXUP_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

void WorkItem::ISA_V_LSHL_B64_Impl(Instruction *instruction)
{
	Instruction::Register64 s0, s1, result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers. 
	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);
	
	// LSHFT_B64
	// Mask s1 to return s1[4:0] 
	// to extract left shift right operand
	result.as_ulong = s0.as_ulong << (s1.as_ulong & 0x001F);
	
	// Write the results. 
	// Cast uint32 to unsigned int 
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst + 1,
			result.hi.as_uint );
	}
}


// D.d = min(S0.d, S1.d).
void WorkItem::ISA_V_MIN_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.d = max(S0.d, S1.d).
void WorkItem::ISA_V_MAX_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = S0.u * S1.u.
void WorkItem::ISA_V_MUL_LO_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate the product.
	result.as_uint = s0.as_uint * s1.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			result.as_uint);
	}
}

/* 
 *D.d = Special case divide FMA with scale and flags(s0.d = Quotient, s1.d = Denominator,
 *s2.d = Numerator).
 */
void WorkItem::ISA_V_DIV_FMAS_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.d = Look Up 2/PI (S0.d) with segment select S1.u[4:0].
void WorkItem::ISA_V_TRIG_PREOP_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = (S0.u * S1.u)>>32
void WorkItem::ISA_V_MUL_HI_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate the product and shift right.
	result.as_uint = (unsigned) 
		(((unsigned long long)s0.as_uint * 
		(unsigned long long)s1.as_uint) >> 32);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			result.as_uint);
	}
}

// D.i = S0.i * S1.i.
void WorkItem::ISA_V_MUL_LO_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate the product.
	result.as_uint = s0.as_uint * s1.as_uint;


	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d)(%d*%d) ", id, 
			INST.vdst, result.as_int, s0.as_int, s1.as_int);
	}
}

// D.f = S0.f - floor(S0.f).
void WorkItem::ISA_V_FRACT_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);

	// Apply negation modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	assert (!(INST.abs & 2));
	assert (!(INST.abs & 4));

	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	assert (!(INST.neg & 2));
	assert (!(INST.neg & 4));

	// Calculate the product.
	result.as_float = s0.as_float - floorf(s0.as_float);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			result.as_float);
	}
}

// D.u = (S0.f < S1.f).
void WorkItem::ISA_V_CMP_LT_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_float < s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.f == S1.f).
void WorkItem::ISA_V_CMP_EQ_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_float == s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// vcc = (S0.f <= S1.f).
void WorkItem::ISA_V_CMP_LE_F32_VOP3a_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = (S0.f > S1.f).
void WorkItem::ISA_V_CMP_GT_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_float > s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.f >= S1.f).
void WorkItem::ISA_V_CMP_GE_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = std::fabs(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = std::fabs(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_float >= s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = !(S0.f > S1.f).
void WorkItem::ISA_V_CMP_NGT_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = std::fabs(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = std::fabs(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = !(s0.as_float > s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = !(S0.f <= S1.f).
void WorkItem::ISA_V_CMP_NLE_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = std::fabs(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = std::fabs(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = !(s0.as_float <= s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = !(S0.f == S1.f).
void WorkItem::ISA_V_CMP_NEQ_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = !(s0.as_float == s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = !(S0.f < S1.f).
void WorkItem::ISA_V_CMP_NLT_F32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_float = fabsf(s0.as_float);
	if (INST.abs & 2)
		s1.as_float = fabsf(s1.as_float);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_float = -s0.as_float;
	if (INST.neg & 2)
		s1.as_float = -s1.as_float;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = !(s0.as_float < s1.as_float);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// Comparison Operations
void WorkItem::ISA_V_CMP_OP16_F64_VOP3a_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = (S0.i < S1.i).
void WorkItem::ISA_V_CMP_LT_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_int < s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.i == S1.i).
void WorkItem::ISA_V_CMP_EQ_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_int == s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.i <= S1.i).
void WorkItem::ISA_V_CMP_LE_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_int <= s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.i > S1.i).
void WorkItem::ISA_V_CMP_GT_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_int > s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:%u]<=(%u) ",
			id_in_wavefront, INST.vdst, INST.vdst + 1, 
			result.as_uint);
	}
}

// D.u = (S0.i <> S1.i).
void WorkItem::ISA_V_CMP_NE_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = !(s0.as_int == s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.i >= S1.i).
void WorkItem::ISA_V_CMP_GE_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_int >= s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.i == S1.i). Also write EXEC
void WorkItem::ISA_V_CMPX_EQ_I32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Apply absolute value modifiers.
	if (INST.abs & 1)
		s0.as_int = abs(s0.as_int);
	if (INST.abs & 2)
		s1.as_int = abs(s1.as_int);
	assert(!(INST.abs & 4));

	// Apply negation modifiers.
	if (INST.neg & 1)
		s0.as_int = -s0.as_int;
	if (INST.neg & 2)
		s1.as_int = -s1.as_int;
	assert(!(INST.neg & 4));

	// Compare the operands.
	result.as_uint = (s0.as_int == s1.as_int);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Write EXEC
	WriteBitmaskSReg(Instruction::RegisterExec, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:%u],EXEC<=(%u) ",
			id_in_wavefront, INST.vdst, INST.vdst+1,
			result.as_uint);
	}
}

// D = IEEE numeric class function specified in S1.u, performed on S0.d.
void WorkItem::ISA_V_CMP_CLASS_F64_VOP3a_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.u = (S0.u < S1.u).
void WorkItem::ISA_V_CMP_LT_U32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Compare the operands.
	result.as_uint = (s0.as_uint < s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.u == S1.u).
void WorkItem::ISA_V_CMP_EQ_U32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Compare the operands.
	result.as_uint = (s0.as_uint == s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.u <= S1.u).
void WorkItem::ISA_V_CMP_LE_U32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Compare the operands.
	result.as_uint = (s0.as_uint <= s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.u > S1.u).
void WorkItem::ISA_V_CMP_GT_U32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Compare the operands.
	result.as_uint = (s0.as_uint > s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

void WorkItem::ISA_V_CMP_LG_U32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate result.
	result.as_uint = ((s0.as_uint < s1.as_uint) || 
		(s0.as_uint > s1.as_uint));

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0.u >= S1.u).
void WorkItem::ISA_V_CMP_GE_U32_VOP3a_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Compare the operands.
	result.as_uint = (s0.as_uint >= s1.as_uint);

	// Write the results.
	WriteBitmaskSReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%u) ",
			id_in_wavefront, INST.vdst,
			result.as_uint);
	}
}

// D.u = (S0 < S1)
void WorkItem::ISA_V_CMP_LT_U64_VOP3a_Impl(Instruction *instruction)
{
	assert(!INST.abs);
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);

	Instruction::Register64 s0, s1, result;

	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	result.as_ulong = s0.as_ulong < s1.as_ulong;
	WriteBitmaskSReg(INST.vdst, (unsigned)result.as_ulong);

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%lu) ",
			id_in_wavefront, INST.vdst,
			result.as_ulong);
	}
}

// D.u = (S0 == S1)
void WorkItem::ISA_V_CMP_EQ_U64_VOP3a_Impl(Instruction *instruction)
{
	assert(!INST.abs);
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);

	Instruction::Register64 s0, s1, result;

	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	result.as_ulong = s0.as_ulong == s1.as_ulong;
	WriteBitmaskSReg(INST.vdst, (unsigned)result.as_ulong);

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%lu) ",
			id_in_wavefront, INST.vdst,
			result.as_ulong);
	}
}

// D.u = (S0 <> S1)
void WorkItem::ISA_V_CMP_LG_U64_VOP3a_Impl(Instruction *instruction)
{
	assert(!INST.abs);
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);

	Instruction::Register64 s0, s1, result;

	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	result.as_ulong = s0.as_ulong != s1.as_ulong;
	WriteBitmaskSReg(INST.vdst, (unsigned)result.as_ulong);

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%lu) ",
			id_in_wavefront, INST.vdst,
			result.as_ulong);
	}
}

// Max of three numbers.
void WorkItem::ISA_V_MAX3_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register max;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	// Determine the max.
	// max3(s0, s1, s2) == s0
	if (s0.as_int >= s1.as_int && s0.as_int >= s2.as_int)
	{
		max.as_int = s0.as_int;
	}
	// max3(s0, s1, s2) == s1
	else if (s1.as_int >= s0.as_int && s1.as_int >= s2.as_int)
	{
		max.as_int = s1.as_int;
	}
	// max3(s0, s1, s2) == s2
	else if (s2.as_int >= s0.as_int && s2.as_int >= s1.as_int)
	{
		max.as_int = s2.as_int;
	}
	else
	{
		throw misc::Panic("Max algorithm failed");
	}

	// Write the results.
	WriteVReg(INST.vdst, max.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%d) ",
			id_in_wavefront, INST.vdst, max.as_int);
	}
}

// D.i = S0.i * S1.i + S2.i.
void WorkItem::ISA_V_MAD_I32_I24_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register result;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = misc::SignExtend32(ReadReg(INST.src0), 24);
	s1.as_uint = misc::SignExtend32(ReadReg(INST.src1), 24);
	s2.as_uint = ReadReg(INST.src2);
	
	__builtin_smul_overflow(s0.as_int, s1.as_int, &result.as_int);
	__builtin_sadd_overflow(result.as_int, s2.as_int, &result.as_int);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%d) ",
			id_in_wavefront, INST.vdst, result.as_int);
	}
}

// Median of three numbers.
void WorkItem::ISA_V_MED3_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register s2;
	Instruction::Register median;

	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	s2.as_uint = ReadReg(INST.src2);

	// Determine the median.
	// max3(s0, s1, s2) == s0
	if (s0.as_int >= s1.as_int && s0.as_int >= s2.as_int)
	{
		// max(s1, s2)
		median.as_int = (s1.as_int >= s2.as_int) ? 
			s1.as_int : s2.as_int;
	}
	// max3(s0, s1, s2) == s1
	else if (s1.as_int >= s0.as_int && s1.as_int >= s2.as_int)
	{
		// max(s0, s2)
		median.as_int = (s0.as_int >= s2.as_int) ? 
			s0.as_int : s2.as_int;
	}
	// max3(s0, s1, s2) == s2
	else if (s2.as_int >= s0.as_int && s2.as_int >= s1.as_int)
	{
		// max(s0, s1)
		median.as_int = (s0.as_int >= s1.as_int) ? 
			s0.as_int : s1.as_int;
	}
	else
	{
		throw misc::Panic("Median algorithm failed");
	}

	// Write the results.
	WriteVReg(INST.vdst, median.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%d) ",
			id_in_wavefront, INST.vdst, median.as_int);
	}
}

// D = S0.u >> S1.u[4:0].
void WorkItem::ISA_V_LSHR_B64_Impl(Instruction *instruction)
{
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	Instruction::Register64 s0, s1, result;

	// Load operands from registers.
	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	// Shift s0.
	result.as_ulong = s0.as_ulong >> (s1.as_ulong & 0x1F);

	// Write the results.
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u]<=(0x%x) ",
			id_in_wavefront, INST.vdst,
			result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S[%u]<=(0x%x) ", INST.vdst + 1,
			result.hi.as_uint);
	}
}

// D = S0.u >> S1.u[4:0] (Arithmetic shift)
void WorkItem::ISA_V_ASHR_I64_Impl(Instruction *instruction)
{
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	Instruction::Register64 s0, s1, result;

	// Load operands from registers.
	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	// Shift s0.
	result.as_ulong = s0.as_long >> (s1.as_ulong & 0x1F);

	// Write the results.
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u]<=(0x%x) ",
			id_in_wavefront, INST.vdst,
			result.lo.as_uint);
		Emulator::isa_debug << misc::fmt("S[%u]<=(0x%x) ", INST.vdst + 1,
			result.hi.as_uint);
	}
}

// D.d = S0.d + S1.d.
void WorkItem::ISA_V_ADD_F64_Impl(Instruction *instruction)
{
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	Instruction::Register64 s0, s1, result;

	// Load operands from registers.
	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	// Add the operands, take into account special number cases.

	// s0 == NaN64 || s1 == NaN64
	if (std::fpclassify(s0.as_double) == FP_NAN ||
		std::fpclassify(s1.as_double) == FP_NAN)
	{
		// value <-- NaN64
		result.as_double = NAN;
	}
	// s0,s1 == infinity
	else if (std::fpclassify(s0.as_double) == FP_INFINITE &&
		std::fpclassify(s1.as_double) == FP_INFINITE)
	{
		// value <-- NaN64
		result.as_double = NAN;
	}
	// s0,!s1 == infinity
	else if (std::fpclassify(s0.as_double) == FP_INFINITE)
	{
		// value <-- s0(+-infinity)
		result.as_double = s0.as_double;
	}
	// s1,!s0 == infinity
	else if (std::fpclassify(s1.as_double) == FP_INFINITE)
	{
		// value <-- s1(+-infinity)
		result.as_double = s1.as_double;
	}
	// s0 == +-denormal, +-0
	else if (std::fpclassify(s0.as_double) == FP_SUBNORMAL ||
		std::fpclassify(s0.as_double) == FP_ZERO)
	{
		// s1 == +-denormal, +-0
		if (std::fpclassify(s1.as_double) == FP_SUBNORMAL ||
			std::fpclassify(s1.as_double) == FP_ZERO)
			// s0 && s1 == -denormal, -0
			if (std::signbit(s0.as_double)
				&& std::signbit(s1.as_double))
				// value <-- -0
				result.as_double = -0;
			else
				// value <-- +0
				result.as_double = +0;
		// s1 == F
		else
			// value <-- s1
			result.as_double = s1.as_double;
	}
	// s1 == +-denormal, +-0
	else if (std::fpclassify(s1.as_double) == FP_SUBNORMAL ||
		std::fpclassify(s1.as_double) == FP_ZERO)
	{
		// s0 == +-denormal, +-0
		if (std::fpclassify(s0.as_double) == FP_SUBNORMAL ||
			std::fpclassify(s0.as_double) == FP_ZERO)
			// s0 && s1 == -denormal, -0
			if (std::signbit(s0.as_double)
				&& std::signbit(s1.as_double))
				// value <-- -0
				result.as_double = -0;
			else
				// value <-- +0
				result.as_double = +0;
		// s0 == F
		else
			// value <-- s1
			result.as_double = s0.as_double;
	}
	// s0 && s1 == F
	else
	{
		result.as_double = s0.as_double + s1.as_double;
	}

	// Write the results.
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%lgf) ",
			id_in_wavefront, INST.vdst,
			result.as_double);
	}
}

// D.d = S0.d * S1.d.
void WorkItem::ISA_V_MUL_F64_Impl(Instruction *instruction)
{
	assert(!INST.clamp);
	assert(!INST.omod);
	assert(!INST.neg);
	assert(!INST.abs);

	Instruction::Register64 s0, s1, result;

	// Load operands from registers.
	s0 = Read_SRC_64(INST.src0);
	s1 = Read_SRC_64(INST.src1);

	// Multiply the operands, take into account special number cases.

	// s0 == NaN64 || s1 == NaN64
	if (std::fpclassify(s0.as_double) == FP_NAN ||
		std::fpclassify(s1.as_double) == FP_NAN)
	{
		// value <-- NaN64
		result.as_double = NAN;
	}
	// s0 == +denormal, +0
	else if ((std::fpclassify(s1.as_double) == FP_SUBNORMAL ||
			std::fpclassify(s1.as_double) == FP_ZERO) &&
		!std::signbit(s0.as_double))
	{
		// s1 == +-infinity
		if (std::isinf(s1.as_double))
			// value <-- NaN64
			result.as_double = NAN;
		// s1 > 0
		else if (!std::signbit(s1.as_double))
			// value <-- +0
			result.as_double = +0;
		// s1 < 0
		else if (std::signbit(s1.as_double))
			// value <-- -0
			result.as_double = -0;
	}
	// s0 == -denormal, -0
	else if ((std::fpclassify(s1.as_double) == FP_SUBNORMAL ||
			std::fpclassify(s1.as_double) == FP_ZERO) &&
		std::signbit(s0.as_double))
	{
		// s1 == +-infinity
		if (std::isinf(s1.as_double))
			// value <-- NaN64
			result.as_double = NAN;
		// s1 > 0
		else if (!std::signbit(s1.as_double))
			// value <-- -0
			result.as_double = -0;
		// s1 < 0
		else if (std::signbit(s1.as_double))
			// value <-- +0
			result.as_double = +0;
	}
	// s0 == +infinity
	else if (std::fpclassify(s0.as_double) == FP_INFINITE &&
		!std::signbit(s0.as_double))
	{
		// s1 == +-denormal, +-0
		if (std::fpclassify(s1.as_double) == FP_SUBNORMAL ||
			std::fpclassify(s1.as_double) == FP_ZERO)
			// value <-- NaN64
			result.as_double = NAN;
		// s1 > 0
		else if (!std::signbit(s1.as_double))
			// value <-- +infinity
			result.as_double = +INFINITY;
		// s1 < 0
		else if (std::signbit(s1.as_double))
			// value <-- -infinity
			result.as_double = -INFINITY;
	}
	// s0 == -infinity
	else if (std::fpclassify(s0.as_double) == FP_INFINITE &&
		std::signbit(s0.as_double))
	{
		// s1 == +-denormal, +-0
		if (std::fpclassify(s1.as_double) == FP_SUBNORMAL ||
			std::fpclassify(s1.as_double) == FP_ZERO)
			// value <-- NaN64
			result.as_double = NAN;
		// s1 > 0
		else if (!std::signbit(s1.as_double))
			// value <-- -infinity
			result.as_double = -INFINITY;
		// s1 < 0
		else if (std::signbit(s1.as_double))
			// value <-- +infinity
			result.as_double = +INFINITY;
	}
	else
	{
		result.as_double = s0.as_double * s1.as_double;
	}

	// Write the results.
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: S[%u:+1]<=(%lgf) ",
			id_in_wavefront, INST.vdst,
			result.as_double);
	}
}

// D.d = Look Up 2/PI (S0.d) with segment select S1.u[4:0].
void WorkItem::ISA_V_LDEXP_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.i = (S0.i * S1.i)>>32.
void WorkItem::ISA_V_MUL_HI_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0, s1;
	Instruction::Register result;

	assert(!INST.abs);
	assert(!INST.clamp);
	assert(!INST.neg);
	assert(!INST.omod);

	s0 = Read_SRC(INST.src0);
	s1 = Read_SRC(INST.src1);
	result.as_int = ((int64_t)s0.as_int * (int64_t)s1.as_int) >> 32;

	WriteVReg(INST.vdst, result.as_uint);
	if (Emulator::isa_debug) {
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%gf) ",
				id_in_wavefront, INST.vdst, result.as_float);
	}
}

} // namespace SI
