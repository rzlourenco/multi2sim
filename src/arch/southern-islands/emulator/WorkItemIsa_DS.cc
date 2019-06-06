#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->ds

// DS[A] = DS[A] + D0; uint add.
void WorkItem::ISA_DS_ADD_U32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// DS[A] = (DS[A] >= D0 ? 0 : DS[A] + 1); uint increment.
void WorkItem::ISA_DS_INC_U32_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

// DS[A] = D0; write a Dword.
void WorkItem::ISA_DS_WRITE_B32_Impl(Instruction *instruction)
{
	assert(!INST.gds);

	Instruction::Register addr;
	Instruction::Register data0;

	// Load address and data from registers.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += ((INST.offset1 << 8) | INST.offset0) * 4;
	data0.as_uint = ReadVReg(INST.data0);

	bool ignored = addr.as_uint >= std::min(
			work_group->getNDRange()->getLocalMemTop(), 
			ReadSReg(Instruction::RegisterM0));
	if (!ignored) {
		lds->Write(addr.as_uint, 4, (char *)&data0.as_uint);
		// Record last memory access for the detailed simulator.
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessWrite;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 4;
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(%u,%f) ", id, 
					addr.as_uint, data0.as_uint, data0.as_float);
		}
	}

	// Print isa debug information.
}

// DS[ADDR+offset0*4] = D0; DS[ADDR+offset1*4] = D1; Write 2 Dwords
void WorkItem::ISA_DS_WRITE2_B32_Impl(Instruction *instruction)
{
	assert(!INST.gds);

	Instruction::Register addr0, addr1;
	Instruction::Register data0, data1;

	// Load address and data from registers.
	addr0.as_uint = ReadVReg(INST.addr);
	addr0.as_uint += INST.offset0*4;
	addr1.as_uint = ReadVReg(INST.addr);
	addr1.as_uint += INST.offset1*4;
	data0.as_uint = ReadVReg(INST.data0);
	data1.as_uint = ReadVReg(INST.data1);

	bool ignored0 =  addr0.as_uint >= std::min(
			 work_group->getNDRange()->getLocalMemTop(), 
			 ReadSReg(Instruction::RegisterM0));
	bool ignored1 =  addr0.as_uint >= std::min(
			 work_group->getNDRange()->getLocalMemTop(), 
			 ReadSReg(Instruction::RegisterM0));
	lds_access_count = 0;
	if (!ignored0) {
		lds->Write(addr0.as_uint, 4,
			(char *)&data0.as_uint);
		lds_access[lds_access_count].type = MemoryAccessWrite;
		lds_access[lds_access_count].addr = addr0.as_uint;
		lds_access[lds_access_count].size = 4;
		++lds_access_count;
	}
	if (!ignored1) {
		lds->Write(addr1.as_uint, 4,
			(char *)&data1.as_uint);
		lds_access[lds_access_count].type = MemoryAccessWrite;
		lds_access[lds_access_count].addr = addr1.as_uint;
		lds_access[lds_access_count].size = 4;
		++lds_access_count;
	}

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		if (!(ignored0 && ignored1))
			Emulator::isa_debug << misc::fmt("t%d: ", id);
		if (!ignored0) {
			Emulator::isa_debug << misc::fmt("LDS[%u]<=(%u,%f) ",
				addr0.as_uint, data0.as_uint, data0.as_float);
		}
		if (!ignored1) {
			Emulator::isa_debug << misc::fmt("LDS[%u]<=(%u,%f) ",
				addr1.as_uint, data1.as_uint, data1.as_float);
		}
	}
}

// DS[A] = D0[7:0]; byte write. 
void WorkItem::ISA_DS_WRITE_B8_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data0;

	assert(!INST.gds);

	// Load address and data from registers.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += ((INST.offset1 << 8) | INST.offset0) * 4;
	data0.as_uint = ReadVReg(INST.data0);

	// Global data store not supported
	assert(!INST.gds);

	// Write Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Write(addr.as_uint, 1, 
			(char *)data0.as_ubyte);
	}

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessWrite;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 1;
	}

	// Print isa debug information.
	if (Emulator::isa_debug && INST.gds)
	{
		Emulator::isa_debug << misc::fmt("t%d: GDS[%u]<=(0x%x) ", id, 
			addr.as_uint, data0.as_ubyte[0]);
	}
	else
	{
		Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(0x%x) ", id, 
			addr.as_uint, data0.as_ubyte[0]);
	}
}

// DS[A] = D0[15:0]; short write. 
void WorkItem::ISA_DS_WRITE_B16_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data0;

	assert(!INST.gds);

	// Load address and data from registers.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += ((INST.offset1 << 8) | INST.offset0) * 4;
	data0.as_uint = ReadVReg(INST.data0);

	// Global data store not supported
	assert(!INST.gds);

	// Write Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Write(addr.as_uint, 2, 
			(char *)data0.as_ushort);
	}

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessWrite;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 2;
	}

	// Print isa debug information.
	if (Emulator::isa_debug && INST.gds)
	{
		Emulator::isa_debug << misc::fmt("t%d: GDS[%u]<=(0x%x) ", id, 
			addr.as_uint, data0.as_ushort[0]);
	}
	else
	{
		Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(0x%x) ", id, 
			addr.as_uint, data0.as_ushort[0]);
	}

}

// R = DS[A]; Dword read.
void WorkItem::ISA_DS_READ_B32_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data;

	assert(!INST.gds);

	// Load address from register.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;

	// Global data store not supported
	assert(!INST.gds);

	// Read Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Read(addr.as_uint, 4,
			(char *)&data.as_uint);
	}

	// Write results.
	WriteVReg(INST.vdst, data.as_uint);

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessRead;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 4;
	}

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x)(0x%x) ", id, 
			INST.vdst, addr.as_uint, data.as_uint);
	}

}

// R = DS[ADDR+offset0*4], R+1 = DS[ADDR+offset1*4]. Read 2 Dwords.
void WorkItem::ISA_DS_READ2_B32_Impl(Instruction *instruction)
{
	Instruction::Register addr0;
	Instruction::Register addr1;
	Instruction::Register data0;
	Instruction::Register data1;

	assert(!INST.gds);

	// Load address from register.
	addr0.as_uint = ReadVReg(INST.addr);
	addr1.as_uint = addr0.as_uint;

	assert(INST.offset0 != INST.offset1);

	addr0.as_uint += INST.offset0 * 4;
	addr1.as_uint += INST.offset1 * 4;

	// Global data store not supported
	assert(!INST.gds);


	lds->Read(
		addr0.as_uint, 4, (char *)&data0.as_uint);
	lds->Read(
		addr1.as_uint, 4, (char *)&data1.as_uint);

	// Write results.
	WriteVReg(INST.vdst, data0.as_uint);
	WriteVReg(INST.vdst+1, data1.as_uint);

	// Record last memory access for the detailed simulator.
	lds_access_count = 2;
	lds_access[0].type = MemoryAccessRead;
	lds_access[0].addr = addr0.as_uint;
	lds_access[0].size = 4;
	lds_access[1].type = MemoryAccessRead;
	lds_access[1].addr = addr1.as_uint;
	lds_access[1].size = 4;

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x)(0x%x) ", id, 
			INST.vdst, addr0.as_uint, data0.as_uint);
		Emulator::isa_debug << misc::fmt("V%u<=(0x%x)(0x%x) ",
			INST.vdst+1, addr1.as_uint, data1.as_uint);
	}
}

// R = signext(DS[A][7:0]}; signed byte read.
void WorkItem::ISA_DS_READ_I8_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data;

	assert(!INST.gds);

	// Load address from register.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;

	// Global data store not supported
	assert(!INST.gds);

	// Read Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Read(addr.as_uint, 1,
			&data.as_byte[0]);
	}

	// Extend the sign.
	data.as_int = (int) data.as_byte[0];

	// Write results.
	WriteVReg(INST.vdst, data.as_uint);

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessRead;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 1;
	}

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x)(%d) ", id, INST.vdst,
			addr.as_uint, data.as_int);
	}
}

// R = {24’h0,DS[A][7:0]}; unsigned byte read.
void WorkItem::ISA_DS_READ_U8_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data;

	assert(!INST.gds);

	// Load address from register.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;

	// Global data store not supported
	assert(!INST.gds);

	// Read Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Read(addr.as_uint, 1,
			(char *)&data.as_ubyte[0]);
	}

	// Make sure to use only bits [7:0].
	data.as_uint = (unsigned) data.as_ubyte[0];

	// Write results.
	WriteVReg(INST.vdst, data.as_uint);

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessRead;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 1;
	}

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x)(%u) ", id, INST.vdst,
			addr.as_uint, data.as_uint);
	}
}

// R = signext(DS[A][15:0]}; signed short read.
void WorkItem::ISA_DS_READ_I16_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data;

	assert(!INST.gds);

	// Load address from register.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;

	// Global data store not supported
	assert(!INST.gds);

	// Read Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Read(addr.as_uint, 2, (char *)&data.as_short[0]);
	}

	// Extend the sign.
	data.as_int = (int) data.as_short[0];

	// Write results.
	WriteVReg(INST.vdst, data.as_uint);

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessRead;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 2;
	}

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x)(%d) ", id, INST.vdst,
			addr.as_uint, data.as_int);
	}

}

// R = {16’h0,DS[A][15:0]}; unsigned short read.
void WorkItem::ISA_DS_READ_U16_Impl(Instruction *instruction)
{
	Instruction::Register addr;
	Instruction::Register data;

	assert(!INST.gds);

	// Load address from register.
	addr.as_uint = ReadVReg(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;

	// Global data store not supported
	assert(!INST.gds);

	// Read Dword.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds->Read(addr.as_uint, 2,
			(char *)&data.as_ushort[0]);
	}

	// Make sure to use only bits [15:0].
	data.as_uint = (unsigned) data.as_ushort[0];

	// Write results.
	WriteVReg(INST.vdst, data.as_uint);

	// Record last memory access for the detailed simulator.
	if (INST.gds)
	{
		assert(0);
	}
	else
	{
		lds_access_count = 1;
		lds_access[0].type = MemoryAccessRead;
		lds_access[0].addr = addr.as_uint;
		lds_access[0].size = 2;
	}

	// Print isa debug information.
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(0x%x)(%u) ", id, INST.vdst,
			addr.as_uint, data.as_uint);
	}
}

// Write.
void WorkItem::ISA_DS_WRITE_B64_Impl(Instruction *instruction)
{
	assert(!INST.gds);

	Instruction::Register addr;
	addr = Read_VSRC(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;
	
	Instruction::Register64 data;
	data = Read_VSRC_64(INST.data0);

	// TODO(rzl): ignore stores that go outside the LDS
	lds->Write(addr.as_uint, 8, data.as_byte);

	lds_access_count = 1;
	lds_access[0].type = MemoryAccessWrite;
	lds_access[0].addr = addr.as_uint;
	lds_access[0].size = 8;

	if (Emulator::isa_debug) {
		Emulator::isa_debug << misc::fmt("t%d: LDS[%x:+7]<=(0x%x 0x%x) ",
			id, addr.as_uint, data.as_uint[0], data.as_uint[1]);
	}
}

// DS[ADDR+offset0*8] = D0; DS[ADDR+offset1*8] = D1; write 2 Dwords.
void WorkItem::ISA_DS_WRITE2_B64_Impl(Instruction *instruction)
{
	assert(!INST.gds);

	Instruction::Register addr0, addr1;
	addr0 = Read_VSRC(INST.addr);
	addr1 = Read_VSRC(INST.addr);
	addr0.as_uint += INST.offset0 * 8;
	addr1.as_uint += INST.offset1 * 8;
	
	Instruction::Register64 data0, data1;
	data0 = Read_VSRC_64(INST.data0);
	data1 = Read_VSRC_64(INST.data1);

	// TODO(rzl): ignore stores that go outside the LDS
	lds->Write(addr0.as_uint, 8, data0.as_byte);
	lds->Write(addr1.as_uint, 8, data1.as_byte);

	lds_access_count = 2;
	lds_access[0].type = MemoryAccessWrite;
	lds_access[0].addr = addr0.as_uint;
	lds_access[0].size = 8;
	lds_access[1].type = MemoryAccessWrite;
	lds_access[1].addr = addr1.as_uint;
	lds_access[1].size = 8;

	if (Emulator::isa_debug) {
		Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(0x%x) ", id,
			addr0.as_uint + 0, data0.as_uint[0]);
		Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(0x%x) ", id,
			addr0.as_uint + 4, data0.as_uint[1]);
		Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(0x%x) ", id,
			addr1.as_uint + 0, data1.as_uint[0]);
		Emulator::isa_debug << misc::fmt("t%d: LDS[%u]<=(0x%x) ", id,
			addr1.as_uint + 4, data1.as_uint[1]);
	}
}

void WorkItem::ISA_DS_READ_B64_Impl(Instruction *instruction)
{
	assert(!INST.gds);
	Instruction::Register addr;
	addr = Read_VSRC(INST.addr);
	addr.as_uint += (INST.offset1 << 8) | INST.offset0;

	Instruction::Register64 data_in;
	// TODO(rzl): read 0 if outside the LDS
	lds->Read(addr.as_uint, 8, data_in.as_byte);
	lds_access_count = 1;
	lds_access[0].type = MemoryAccessRead;
	lds_access[0].addr = addr.as_uint;
	lds_access[0].size = 8;

	WriteVReg(INST.vdst, data_in.lo.as_uint);
	WriteVReg(INST.vdst + 1, data_in.hi.as_uint);

	if (Emulator::isa_debug) {
		Emulator::isa_debug << misc::fmt("t%d: V[%d:+1]<=(%x %x)", id,
			INST.vdst, data_in.lo.as_uint, data_in.hi.as_uint);
	}
}

void WorkItem::ISA_DS_READ2_B64_Impl(Instruction *instruction)
{
	assert(!INST.gds);
	Instruction::Register addr0, addr1;
	addr0 = Read_VSRC(INST.addr);
	addr0.as_uint += INST.offset0 * 8;
	addr1 = Read_VSRC(INST.addr);	
	addr0.as_uint += INST.offset1 * 8;

	Instruction::Register64 data0_in, data1_in;
	// TODO(rzl): read 0 if outside the LDS
	lds->Read(addr0.as_uint, 8, data0_in.as_byte);
	lds->Read(addr1.as_uint, 8, data1_in.as_byte);
	lds_access_count = 2;
	lds_access[0].type = MemoryAccessRead;
	lds_access[0].addr = addr0.as_uint;
	lds_access[0].size = 8;
	lds_access[1].type = MemoryAccessRead;
	lds_access[1].addr = addr1.as_uint;
	lds_access[1].size = 8;

	WriteVReg(INST.vdst, data0_in.lo.as_uint);
	WriteVReg(INST.vdst + 1, data0_in.hi.as_uint);
	WriteVReg(INST.vdst + 2, data1_in.lo.as_uint);
	WriteVReg(INST.vdst + 3, data1_in.hi.as_uint);

	if (Emulator::isa_debug) {
		Emulator::isa_debug << misc::fmt("t%d: V[%d:+3]<=(%x %x %x %x)", id,
			INST.vdst, data0_in.lo.as_uint, data0_in.hi.as_uint,
			data1_in.lo.as_uint, data1_in.hi.as_uint);
	}
}

} // namespace SI
