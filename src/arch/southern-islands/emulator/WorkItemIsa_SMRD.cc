#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->smrd

void WorkItem::ISA_S_BUFFER_LOAD_DWORD_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);
	int sbase = INST.sbase << 1;

	// sbase holds the first of 4 registers containing the buffer
	// resource descriptor
	BufferDescriptor buffer_descriptor;
	ReadBufferResource(sbase, buffer_descriptor);

	// Calculate effective address
	unsigned m_base = buffer_descriptor.base_addr;
	unsigned m_offset = INST.imm ? INST.offset * 4 : ReadSReg(INST.offset);
	unsigned addr = m_base + m_offset;

	// Read value from global memory
	Instruction::Register value;
	global_mem->Read(addr, 4, (char *)&value);

	// Store the data in the destination register
	WriteSReg(INST.sdst, value.as_uint);

	// Debug
	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("wf%d: S%u<=(%u)(%u,%gf)", 
				wavefront->getId(), INST.sdst, addr, 
				value.as_uint, value.as_float);

	// Record last memory access for timing simulation purposes
	global_memory_access_address = addr;
	global_memory_access_size = 4;
}

void WorkItem::ISA_S_BUFFER_LOAD_DWORDX2_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);
	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = (INST.imm) ? (INST.offset * 4) : ReadSReg(INST.offset);
	unsigned addr = m_base + m_offset;

	Instruction::Register value[2];
	for (int i = 0; i < 2; i++)
	{
		// Read value from global memory
		global_mem->Read(addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: ", wavefront->getId());
		for (int i = 0; i < 2; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u)(%u,%gf) ", INST.sdst + i, 
				addr + i * 4, value[i].as_uint, 
				value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = 4 * 2;
}


void WorkItem::ISA_S_BUFFER_LOAD_DWORDX4_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);
	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = (INST.imm) ? (INST.offset * 4) : ReadSReg(INST.offset);
	unsigned addr = m_base + m_offset;

	Instruction::Register value[4];
	for (int i = 0; i < 4; i++)
	{
		// Read value from global memory
		global_mem->Read(addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: ", wavefront->getId());
		for (int i = 0; i < 4; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u)(%u,%gf) ", INST.sdst + i, 
				addr + i*4, value[i].as_uint, 
				value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = 4 * 4;
}

void WorkItem::ISA_S_BUFFER_LOAD_DWORDX8_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);
	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = (INST.imm) ? (INST.offset * 4) : ReadSReg(INST.offset);
	unsigned addr = m_base + m_offset;

	Instruction::Register value[8];
	for (int i = 0; i < 8; i++)
	{
		// Read value from global memory
		global_mem->Read(addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: ", wavefront->getId());
		for (int i = 0; i < 8; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u)(%u,%gf) ", INST.sdst + i, 
				addr + i*4, value[i].as_uint, 
				value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = 4 * 8;
}

void WorkItem::ISA_S_BUFFER_LOAD_DWORDX16_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);
	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = (INST.imm) ? (INST.offset * 4) : ReadSReg(INST.offset);
	unsigned addr = m_base + m_offset;

	Instruction::Register value[16];
	for (int i = 0; i < 16; i++)
	{
		// Read value from global memory
		global_mem->Read(addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("wf%d: ", wavefront->getId());
		for (int i = 0; i < 16; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u)(%u,%gf) ", INST.sdst + i, 
				addr + i*4, value[i].as_uint, 
				value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = 4 * 16;
}

void WorkItem::ISA_S_LOAD_DWORD_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);

	assert(INST.imm);

	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = INST.offset * 4;
	unsigned m_addr = m_base + m_offset;

	assert(!(m_addr & 0x3));

	Instruction::Register value[1];
	for (int i = 0; i < 1; i++)
	{
		// Read value from global memory
		global_mem->Read(m_addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S%u <= (addr %u): ", INST.sdst, m_addr);
		for (int i = 0; i < 1; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u,%gf) ", INST.sdst + i,
				value[i].as_uint, value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = m_addr;
	global_memory_access_size = 4 * 1;
}

void WorkItem::ISA_S_LOAD_DWORDX2_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);

	assert(INST.imm);

	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = INST.offset * 4;
	unsigned m_addr = m_base + m_offset;

	assert(!(m_addr & 0x3));

	Instruction::Register value[2];
	for (int i = 0; i < 2; i++)
	{
		// Read value from global memory		
		global_mem->Read(m_addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S[%u,%u] <= (addr %u): ", INST.sdst, INST.sdst+1, 
			m_addr);
		for (int i = 0; i < 2; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u,%gf) ", INST.sdst + i,
				value[i].as_uint, value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = m_addr;
	global_memory_access_size = 4 * 2;
}

void WorkItem::ISA_S_LOAD_DWORDX4_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);

	assert(INST.imm);

	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = INST.offset * 4;
	unsigned m_addr = m_base + m_offset;

	assert(!(m_addr & 0x3));

	Instruction::Register value[4];
	for (int i = 0; i < 4; i++)
	{
		// Read value from global memory		
		global_mem->Read(m_addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S[%u,%u] <= (addr %u): ", INST.sdst, INST.sdst+3, 
			m_addr);
		for (int i = 0; i < 4; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u,%gf) ", INST.sdst + i,
				value[i].as_uint, value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = m_addr;
	global_memory_access_size = 4 * 4;
}

void WorkItem::ISA_S_LOAD_DWORDX8_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);

	assert(INST.imm);

	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = INST.offset * 4;
	unsigned m_addr = m_base + m_offset;

	assert(!(m_addr & 0x3));

	Instruction::Register value[8];
	for (int i = 0; i < 8; i++)
	{
		// Read value from global memory		
		global_mem->Read(m_addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S[%u,%u] <= (addr %u): ", INST.sdst, INST.sdst+3, 
			m_addr);
		for (int i = 0; i < 8; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u,%gf) ", INST.sdst + i,
				value[i].as_uint, value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = m_addr;
	global_memory_access_size = 4 * 8;
}

void WorkItem::ISA_S_LOAD_DWORDX16_Impl(Instruction *instruction)
{
	// Record access
	wavefront->setScalarMemoryRead(true);

	assert(INST.imm);

	int sbase = INST.sbase << 1;

	MemoryPointer memory_pointer;
	ReadMemPtr(sbase, memory_pointer);

	// Calculate effective address
	unsigned m_base = memory_pointer.addr;
	unsigned m_offset = INST.offset * 4;
	unsigned m_addr = m_base + m_offset;

	assert(!(m_addr & 0x3));

	Instruction::Register value[16];
	for (int i = 0; i < 16; i++)
	{
		// Read value from global memory		
		global_mem->Read(m_addr + i * 4, 4, (char *)&value[i]);
		// Store the data in the destination register
		WriteSReg(INST.sdst + i, value[i].as_uint);
	}

	// Debug
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("S[%u,%u] <= (addr %u): ", INST.sdst, INST.sdst+3, 
			m_addr);
		for (int i = 0; i < 8; i++)
		{
			Emulator::isa_debug << misc::fmt("S%u<=(%u,%gf) ", INST.sdst + i,
				value[i].as_uint, value[i].as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = m_addr;
	global_memory_access_size = 4 * 16;
}

} // namespace SI
