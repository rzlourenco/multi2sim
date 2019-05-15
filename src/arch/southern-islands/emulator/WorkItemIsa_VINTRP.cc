#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->vintrp

// D = P10 * S + P0
void WorkItem::ISA_V_INTERP_P1_F32_Impl(Instruction *instruction)
{
	// M0 must be intialized before using any VINTRP instruction
	struct M0ForInterp
	{
		unsigned b0 : 1;
		unsigned new_prim_mask : 15;
		unsigned lds_param_offset : 16;
	}__attribute__((packed));

	union VInterpM0
	{
		unsigned as_uint;
		struct M0ForInterp for_vintrp;
	};

	Instruction::Register s;
	Instruction::Register p0;
	Instruction::Register p10;
	Instruction::Register data;

	union VInterpM0 m0_vintrp;

	// Get lds offset and primitive mask information
	m0_vintrp.as_uint = ReadReg(Instruction::RegisterM0);

	// Read barycentric coordinates stored in VGPR
	s.as_uint = ReadVReg(INST.vsrc);

	// 12 successive dwords contain P0 P10 P20
	// 4dwords P0: X Y Z W, INST.attrchan decides which 1dword to be loaded
	lds->Read( 
		m0_vintrp.for_vintrp.lds_param_offset + 0 + 4 * INST.attrchan ,
		 4, (char *)&p0.as_uint);
	// 4dwords P10: X Y Z W, INST.attrchan decides which 1dword to be loaded
	lds->Read( 
		m0_vintrp.for_vintrp.lds_param_offset + 16 + 4 * INST.attrchan,
		 4, (char *)&p10.as_uint);

	// D = P10 * S + P0
	data.as_float = p10.as_float * s.as_float + p0.as_float;
	
	// Write the result
	WriteVReg(INST.vdst, data.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%f = P10(%f) * Lamda2(%f) + P0(%f)) \n", 
			id, INST.vdst, data.as_float, p10.as_float, 
			s.as_float, p0.as_float);
	}
}

// D = P20 * S + D
void WorkItem::ISA_V_INTERP_P2_F32_Impl(Instruction *instruction)
{
	// M0 must be intialized before using any VINTRP instruction
	struct M0ForInterp
	{
		unsigned b0 : 1;
		unsigned new_prim_mask : 15;
		unsigned lds_param_offset : 16;
	}__attribute__((packed));

	union VInterpM0
	{
		unsigned as_uint;
		struct M0ForInterp for_vintrp;
	};

	Instruction::Register s;
	Instruction::Register p20;
	Instruction::Register data;

	union VInterpM0 m0_vintrp;

	// Get lds offset and primitive mask information
	m0_vintrp.as_uint = ReadReg(Instruction::RegisterM0);

	// Read barycentric coordinates stored in VGPR
	s.as_uint = ReadVReg(INST.vsrc);

	// Read data stores in VGPR for later acclumulation
	data.as_uint = ReadVReg(INST.vdst);

	// 12 successive dwords contain P0 P10 P20
	// 4dwords P20: X Y Z W, INST.attrchan decides which 1dword to be loaded 
	lds->Read( m0_vintrp.for_vintrp.lds_param_offset + 32 + 4 * INST.attrchan,
		 4, (char *)&p20.as_uint);

	// D = P20 * S + D
	data.as_float += p20.as_float * s.as_float;

	// Write the result
	WriteVReg(INST.vdst, data.as_uint);

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%f += P20(%f) * Lamda2(%f)) \n", 
			id, INST.vdst, data.as_float, p20.as_float, 
			s.as_float);
	}
}

// D = {P10,P20,P0}[S]
void WorkItem::ISA_V_INTERP_MOV_F32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

} // namespace SI
