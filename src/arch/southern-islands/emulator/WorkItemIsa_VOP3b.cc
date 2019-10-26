#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->vop3b

/* D.u = S0.u + S1.u; VCC=carry-out (VOP3:sgpr=carry-out). */
void WorkItem::ISA_V_ADD_I32_VOP3b_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry_out;

	assert(!INST.omod);
	assert(!INST.neg); 

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate sum and carry.
	sum.as_uint = s0.as_uint + s1.as_uint;
	carry_out.as_uint =
		! !(((unsigned long long) s0.as_uint + 
			(unsigned long long) s1.as_uint) >> 32);

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);
	WriteBitmaskSReg(INST.sdst, carry_out.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			sum.as_uint);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry_out.as_uint);
	}
}

/* D.u = S0.u - S1.u; VCC=carry-out (VOP3:sgpr=carry-out). */
void WorkItem::ISA_V_SUB_I32_VOP3b_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry_out;

	assert(!INST.omod);
	assert(!INST.neg); 

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate sum and carry.
	carry_out.as_uint = __builtin_usub_overflow(s0.as_uint, s1.as_uint, &sum.as_uint);

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);
	WriteBitmaskSReg(INST.sdst, carry_out.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			sum.as_uint);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry_out.as_uint);
	}
}

/* D.u = S1.u - S0.u; VCC=carry-out (VOP3:sgpr=carry-out). */
void WorkItem::ISA_V_SUBREV_I32_VOP3b_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry_out;

	assert(!INST.omod);
	assert(!INST.neg); 

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);

	// Calculate sum and carry.
	carry_out.as_uint = __builtin_usub_overflow(s1.as_uint, s0.as_uint, &sum.as_uint);

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);
	WriteBitmaskSReg(INST.sdst, carry_out.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			sum.as_uint);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry_out.as_uint);
	}
}

/* D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carry-out,
 * S2.u=carry-in). */
void WorkItem::ISA_V_ADDC_U32_VOP3b_Impl(Instruction *instruction)
{
	Instruction::Register s0;
	Instruction::Register s1;
	Instruction::Register sum;
	Instruction::Register carry_in;
	Instruction::Register carry_out;

	assert(!INST.omod);
	assert(!INST.neg); 

	// Load operands from registers.
	s0.as_uint = ReadReg(INST.src0);
	s1.as_uint = ReadReg(INST.src1);
	carry_in.as_uint = ReadBitmaskSReg(INST.src2);

	// Calculate sum and carry.
	sum.as_uint = s0.as_uint + s1.as_uint + carry_in.as_uint;
	carry_out.as_uint =
		! !(((unsigned long long) s0.as_uint + 
			(unsigned long long) s1.as_uint +
			(unsigned long long) carry_in.as_uint) >> 32);

	// Write the results.
	WriteVReg(INST.vdst, sum.as_uint);
	WriteBitmaskSReg(INST.sdst, carry_out.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u) ", id, INST.vdst,
			sum.as_uint);
		Emulator::isa_debug << misc::fmt("vcc<=(%u) ", carry_out.as_uint);
	}
}

/* 
 *D.d = Special case divide preop and flags(s0.d = Quotient, s1.d = Denominator, s2.d = Numerator)
 *s0 must equal s1 or s2.
 */
void WorkItem::ISA_V_DIV_SCALE_F64_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

} // namespace SI
