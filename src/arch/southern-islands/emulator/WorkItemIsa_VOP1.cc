#include "WorkItem.h"
#include "Emulator.h"

#include <cmath>

namespace SI {

#define INST instruction->getBytes()->vop1

// Do nothing
void WorkItem::ISA_V_NOP_Impl(Instruction *instruction)
{
	// Do nothing
}

// D.u = S0.u.
void WorkItem::ISA_V_MOV_B32_Impl(Instruction *instruction)
{
	Instruction::Register value;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		value.as_uint = INST.lit_cnst;
	else
		value.as_uint = ReadReg(INST.src0);

	// Write the results.
	WriteVReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			value.as_uint);
	}
}

// Copy one VGPR value to one SGPR.
void WorkItem::ISA_V_READFIRSTLANE_B32_Impl(Instruction *instruction)
{
	Instruction::Register value;

	// Load operand from register.
	assert(INST.src0 >= 256 || INST.src0 == Instruction::RegisterM0);
	value.as_uint = ReadReg(INST.src0);

	// Write the results.
	// Store the data in the destination register
	WriteSReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			value.as_uint);
	}
}

// D.i = (int)S0.d.
void WorkItem::ISA_V_CVT_I32_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = (double)S0.i.
void WorkItem::ISA_V_CVT_F64_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register64 result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Convert and separate value.
	result.as_double = (double) s0.as_int;

	// Write the results.
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u:+1]<=(%lgf) ", id,
			INST.vdst, result.as_double);
	}
}

// D.f = UINT2FLT(S0.u[7:0])
void WorkItem::ISA_V_CVT_F32_UBYTE0_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	result.as_float = (float)(s0.as_uint & 0xFF);
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%lgf) ", id,
			INST.vdst, result.as_float);
	}
}

// D.f = UINT2FLT(S0.u[15:8])
void WorkItem::ISA_V_CVT_F32_UBYTE1_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	result.as_float = (float)((s0.as_uint >> 8) & 0xFF);
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%lgf) ", id,
			INST.vdst, result.as_float);
	}
}

// D.f = UINT2FLT(S0.u[23:16])
void WorkItem::ISA_V_CVT_F32_UBYTE2_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	result.as_float = (float)((s0.as_uint >> 16) & 0xFF);
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%lgf) ", id,
			INST.vdst, result.as_float);
	}
}

// D.f = UINT2FLT(S0.u[31:24])
void WorkItem::ISA_V_CVT_F32_UBYTE3_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	result.as_float = (float)((s0.as_uint >> 24) & 0xFF);
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(%lgf) ", id,
			INST.vdst, result.as_float);
	}
}

// D.f = (float)S0.i.
void WorkItem::ISA_V_CVT_F32_I32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register value;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	value.as_float = (float) s0.as_int;

	// Write the results.
	WriteVReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			value.as_float);
	}
}

// D.f = (float)S0.u.
void WorkItem::ISA_V_CVT_F32_U32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register value;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	value.as_float = (float) s0.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			value.as_float);
	}
}

// D.i = (uint)S0.f.
void WorkItem::ISA_V_CVT_U32_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register value;

	float fvalue;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	fvalue = s0.as_float;

	// Handle special number cases and cast to an unsigned

	// -inf, NaN, 0, -0 --> 0
	if ((std::isinf(fvalue) && fvalue < 0.0f) || std::isnan(fvalue)
		|| fvalue == 0.0f || fvalue == -0.0f)
		value.as_uint = 0;
	// inf, > max_uint --> max_uint
	else if (std::isinf(fvalue) || fvalue >= std::numeric_limits<unsigned int>::max())
		value.as_uint = std::numeric_limits<unsigned int>::max();
	else
		value.as_uint = (unsigned) fvalue;

	// Write the results.
	WriteVReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			value.as_uint);
	}
}

// D.i = (int)S0.f.
void WorkItem::ISA_V_CVT_I32_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register value;

	float fvalue;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	fvalue = s0.as_float;

	// Handle special number cases and cast to an int

	// inf, > max_int --> max_int
	if ((std::isinf(fvalue) && fvalue > 0.0f) || fvalue >= std::numeric_limits<int>::max())
		value.as_int = std::numeric_limits<int>::max();
	// -inf, < -max_int --> -max_int
	else if (std::isinf(fvalue) || fvalue < std::numeric_limits<int>::min())
		value.as_int = std::numeric_limits<int>::min();
	// NaN, 0, -0 --> 0
	else if (std::isnan(fvalue) || fvalue == 0.0f || fvalue == -0.0f)
		value.as_int = 0;
	else
		value.as_int = (int) fvalue;

	// Write the results.
	WriteVReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%d) ", id, INST.vdst,
			value.as_int);
	}
}

// D.f = (float)S0.d.
void WorkItem::ISA_V_CVT_F32_F64_Impl(Instruction *instruction)
{
	Instruction::Register64 s0;
	Instruction::Register result;

	assert(INST.src0 != 0xFF);

	// Load operand from registers.
	s0 = Read_SRC_64(INST.src0);

	// Cast to a single precision float
	result.as_float = (float) s0.as_double;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			result.as_float);
	}
}


// D.d = (double)S0.f.
void WorkItem::ISA_V_CVT_F64_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register64 result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Cast to a single precision float
	result.as_double = (double) s0.as_float;

	// Write the results.
	WriteVReg(INST.vdst, result.lo.as_uint);
	WriteVReg(INST.vdst + 1, result.hi.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u:+1]<=(%lgf) ", id,
			INST.vdst, result.as_double);
	}
}

// D.d = (double)S0.u.
void WorkItem::ISA_V_CVT_F64_U32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = S0.f - floor(S0.f).
void WorkItem::ISA_V_FRACT_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Truncate decimal portion
	result.as_float = s0.as_float - std::floor(s0.as_float);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			result.as_float);
	}
}

// D.f = trunc(S0.f), return integer part of S0.
void WorkItem::ISA_V_TRUNC_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register value;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Truncate decimal portion
	value.as_float = (float)((int)s0.as_float);

	// Write the results.
	WriteVReg(INST.vdst, value.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			value.as_float);
	}
}

// D.f = trunc(S0); if ((S0 < 0.0) && (S0 != D)) D += -1.0.
void WorkItem::ISA_V_FLOOR_F32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = log2(S0.f).
void WorkItem::ISA_V_LOG_F32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = 1.0 / S0.f.
void WorkItem::ISA_V_RCP_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register rcp;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	rcp.as_float = 1.0f / s0.as_float;

	// Write the results.
	WriteVReg(INST.vdst, rcp.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			rcp.as_float);
	}
}

// D.f = 1.0 / S0.f, only integer div_by_zero flag can be raised.
void WorkItem::ISA_V_RCP_IFLAG_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register rcp;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// TODO(rzl): flag div_by_zero
	rcp.as_float = 1.0f / s0.as_float;

	// Write the results.
	WriteVReg(INST.vdst, rcp.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			rcp.as_float);
	}
}

// D.f = 1.0 / sqrt(S0.f).
void WorkItem::ISA_V_RSQ_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register rsq;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	rsq.as_float = 1.0f / sqrt(s0.as_float);

	// Write the results.
	WriteVReg(INST.vdst, rsq.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			rsq.as_float);
	}
}


// D.d = 1.0 / (S0.d).
void WorkItem::ISA_V_RCP_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = 1.0 / sqrt(S0.f).
void WorkItem::ISA_V_RSQ_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.f = sqrt(S0.f).
void WorkItem::ISA_V_SQRT_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register srt;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	srt.as_float = sqrtf(s0.as_float);

	// Write the results.
	WriteVReg(INST.vdst, srt.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) ", id, INST.vdst,
			srt.as_float);
	}
}

// D.f = sin(S0.f)
void WorkItem::ISA_V_SIN_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Normalize input
	// XXX Should it be module instead of dividing?
	s0.as_float = s0.as_float * (2 * M_PI);

	if (misc::inRange(s0.as_float, -256, 256))
	{
		result.as_float = sinf(s0.as_float);
	}
	else
	{
		assert(0); // Haven't debugged this yet
		result.as_float = 0;
	}

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) (sin %gf) ", id, 
			INST.vdst, result.as_float, s0.as_float);
	}
}

// D.f = cos(S0.f)
void WorkItem::ISA_V_COS_F32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Normalize input
	// XXX Should it be module instead of dividing?
	s0.as_float = s0.as_float * (2 * M_PI);

	if (misc::inRange(s0.as_float, -256, 256))
	{
		result.as_float = cosf(s0.as_float);
	}
	else
	{
		assert(0); // Haven't debugged this yet
		result.as_float = 1;
	}

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%gf) (cos %gf) ", id, 
			INST.vdst, result.as_float, s0.as_float);
	}
}

// D.u = ~S0.u.
void WorkItem::ISA_V_NOT_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	// Bitwise not
	result.as_uint = ~s0.as_uint;

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			result.as_uint);
	}
}

// D.u[31:0] = S0.u[0:31]
void WorkItem::ISA_V_BFREV_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register result;

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);

	result.as_uint = misc::BitReverse32(s0.as_uint);

	// Write the results.
	WriteVReg(INST.vdst, result.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x) ", id, INST.vdst,
			result.as_uint);
	}
}

// D.u = position of first 1 in S0 from MSB; D=0xFFFFFFFF if S0==0.
void WorkItem::ISA_V_FFBH_U32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// D.d = FRAC64(S0.d);
void WorkItem::ISA_V_FRACT_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// VGPR[D.u + M0.u] = VGPR[S0.u].
void WorkItem::ISA_V_MOVRELD_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register m0;
	
	assert(INST.src0 != 0xFF);

	// Load operand from register or as a literal constant.
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0);
	m0.as_uint = ReadReg(Instruction::RegisterM0);

	// Write the results.
	WriteVReg(INST.vdst+m0.as_uint, s0.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u+%u]<=(0x%x) ", id, 
			INST.vdst, m0.as_uint, s0.as_uint);
	}
}


// VGPR[D.u] = VGPR[S0.u + M0.u].
void WorkItem::ISA_V_MOVRELS_B32_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register m0;
	
	assert(INST.src0 != 0xFF);

	// Load operand from register or as a literal constant.
	m0.as_uint = ReadReg(Instruction::RegisterM0);
	if (INST.src0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0.as_uint = ReadReg(INST.src0 + m0.as_uint);

	// Write the results.
	WriteVReg(INST.vdst, s0.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V[%u]<=(0x%x) ", id, 
			INST.vdst, s0.as_uint);
	}
}

}
