#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

// XXX(rzl): Is it worth it to merge these two functions?

uint32_t
WorkItem::CalculateBUFAddress(Instruction::BytesMUBUF INST)
{
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));
	assert(!INST.lds);
	assert(!INST.slc);
	assert(!INST.tfe);

	// srsrc is in units of 4 registers
	uint32_t srsrc = INST.srsrc << 2;
	BufferDescriptor buffer_descriptor;
	ReadBufferResource(srsrc, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned soffset = ReadSReg(INST.soffset);
	unsigned ioffset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

        uint32_t addr;
	if (INST.addr64)
	{
	        uint32_t addr_lsb = ReadVReg(INST.vaddr);
	        uint32_t addr_msb = ReadVReg(INST.vaddr+1);
	        uint64_t vaddr = addr_msb;
	        vaddr = (vaddr << 32) | addr_lsb;
	        vaddr += base + ioffset + soffset;
	        // FIXME(rzl): this requires 64-bit addressing in the memory
	        // emulation.
	        addr = (uint32_t)vaddr;
	        assert(addr == vaddr && "need 64-bit addressing support!");
	} else {
		unsigned off_vgpr = 0;
		unsigned idx_vgpr = 0;

		// Table 8.3 from SI ISA
		if (INST.idxen && INST.offen) {
			idx_vgpr = ReadVReg(INST.vaddr);
			off_vgpr = ReadVReg(INST.vaddr + 1);
		} else if (!INST.idxen && INST.offen) {
			off_vgpr = ReadVReg(INST.vaddr);
		} else if (INST.idxen && !INST.offen) {
			idx_vgpr = ReadVReg(INST.vaddr);
		}

		/* It wouldn't make sense to have a value for idxen without
		 * having a stride. */
		assert(!idx_vgpr || stride);
	        addr = base + soffset + ioffset + off_vgpr + 
		    stride * (idx_vgpr + id_in_wavefront);
	}

	return addr;
}

uint32_t
WorkItem::CalculateBUFAddress(Instruction::BytesMTBUF INST)
{
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));
	assert(!INST.slc);
	assert(!INST.tfe);

	// srsrc is in units of 4 registers
	uint32_t srsrc = INST.srsrc << 2;
	BufferDescriptor buffer_descriptor;
	ReadBufferResource(srsrc, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned soffset = ReadSReg(INST.soffset);
	unsigned ioffset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

        uint32_t addr;
	if (INST.addr64)
	{
	        uint32_t addr_lsb = ReadVReg(INST.vaddr);
	        uint32_t addr_msb = ReadVReg(INST.vaddr+1);
	        uint64_t vaddr = addr_msb;
	        vaddr = (vaddr << 32) | addr_lsb;
	        vaddr += base + ioffset + soffset;
	        // FIXME(rzl): this requires 64-bit addressing in the memory
	        // emulation.
	        addr = (uint32_t)vaddr;
	        assert(addr == vaddr && "need 64-bit addressing support!");
	} else {
		unsigned off_vgpr = 0;
		unsigned idx_vgpr = 0;

		// Table 8.3 from SI ISA
		if (INST.idxen && INST.offen) {
			idx_vgpr = ReadVReg(INST.vaddr);
			off_vgpr = ReadVReg(INST.vaddr + 1);
		} else if (!INST.idxen && INST.offen) {
			off_vgpr = ReadVReg(INST.vaddr);
		} else if (INST.idxen && !INST.offen) {
			idx_vgpr = ReadVReg(INST.vaddr);
		}

		/* It wouldn't make sense to have a value for idxen without
		 * having a stride. */
		assert(!idx_vgpr || stride);
	        addr = base + soffset + ioffset + off_vgpr + 
		    stride * (idx_vgpr + id_in_wavefront);
	}

	return addr;
}

/*
 * MUBUF
 */

#define INST instruction->getBytes()->mubuf

void WorkItem::ISA_BUFFER_LOAD_UBYTE_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	Instruction::Register value;
	const int bytes_to_read = 1;
	global_mem->Read(addr, bytes_to_read, &value.as_byte[0]);
	
	// Zero extend
	value.as_uint = value.as_byte[0];
	WriteVReg(INST.vdata, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(addr %x)(%u) ",
			id, INST.vdata, addr, value.as_uint);
}

void WorkItem::ISA_BUFFER_LOAD_SBYTE_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	Instruction::Register value;
	const int bytes_to_read = 1;
	global_mem->Read(addr, bytes_to_read, &value.as_byte[0]);
	
	// Sign extend
	value.as_int = (int) value.as_byte[0];
	WriteVReg(INST.vdata, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	if (Emulator::isa_debug) {
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(addr %x)(%d) ",
			id, INST.vdata, addr, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_LOAD_DWORD_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	const int words_to_read = 1;
	const int bytes_to_read = words_to_read * 4;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < words_to_read; ++i) {
		Instruction::Register value;
		global_mem->Read(addr + i * 4, 4, value.as_byte);
		WriteVReg(INST.vdata + i, value.as_uint);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(addr %x)(%d) ",
				INST.vdata + i, addr + i * 4, value.as_int);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_BUFFER_LOAD_DWORDX2_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	const int words_to_read = 2;
	const int bytes_to_read = words_to_read * 4;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < words_to_read; ++i) {
		Instruction::Register value;
		global_mem->Read(addr + i * 4, 4, value.as_byte);
		WriteVReg(INST.vdata + i, value.as_uint);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(addr %x)(%d) ",
				INST.vdata + i, addr + i * 4, value.as_int);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_BUFFER_LOAD_DWORDX4_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	const int words_to_read = 4;
	const int bytes_to_read = words_to_read * 4;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < words_to_read; ++i) {
		Instruction::Register value;
		global_mem->Read(addr + i * 4, 4, value.as_byte);
		WriteVReg(INST.vdata + i, value.as_uint);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(addr %x)(%d) ",
				INST.vdata + i, addr + i * 4, value.as_int);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_BUFFER_STORE_BYTE_Impl(Instruction *instruction)
{
	// XXX(rzl): Is this still relevant?
	if (0 && INST.glc)
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant

	uint32_t addr = CalculateBUFAddress(INST);

	Instruction::Register value;
	value.as_uint = ReadVReg(INST.vdata) & 0xFF;
	const int bytes_to_write = 1;
	global_mem->Write(addr, bytes_to_write, &value.as_byte[0]);
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(addr %x)(%u) ",
			id, INST.vdata, addr, value.as_uint);
}

void WorkItem::ISA_BUFFER_STORE_DWORD_Impl(Instruction *instruction)
{
	// XXX(rzl): Is this still relevant?
	if (0 && INST.glc)
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant

	uint32_t addr = CalculateBUFAddress(INST);

	const int words_to_write = 1;
	const int bytes_to_write = words_to_write * 4;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < words_to_write; ++i) {
		Instruction::Register value;
		value.as_uint = ReadVReg(INST.vdata + i);
		global_mem->Write(addr + i * 4, 4, &value.as_byte[0]);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("(addr %x)<=V%u(%u) ",
				addr + i * 4, INST.vdata + i, value.as_uint);
	}
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}

void WorkItem::ISA_BUFFER_STORE_DWORDX2_Impl(Instruction *instruction)
{
	// XXX(rzl): Is this still relevant?
	if (0 && INST.glc)
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant

	uint32_t addr = CalculateBUFAddress(INST);

	const int words_to_write = 2;
	const int bytes_to_write = words_to_write * 4;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < words_to_write; ++i) {
		Instruction::Register value;
		value.as_uint = ReadVReg(INST.vdata + i);
		global_mem->Write(addr + i * 4, 4, &value.as_byte[0]);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("(addr %x)<=V%u(%u) ",
				addr + i * 4, INST.vdata + i, value.as_uint);
	}
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}

void WorkItem::ISA_BUFFER_STORE_DWORDX4_Impl(Instruction *instruction)
{
	// XXX(rzl): Is this still relevant?
	if (0 && INST.glc)
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant

	uint32_t addr = CalculateBUFAddress(INST);

	const int words_to_write = 4;
	const int bytes_to_write = words_to_write * 4;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < words_to_write; ++i) {
		Instruction::Register value;
		value.as_uint = ReadVReg(INST.vdata + i);
		global_mem->Write(addr + i * 4, 4, &value.as_byte[0]);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("(addr %x)<=V%u(%u) ",
				addr + i * 4, INST.vdata + i, value.as_uint);
	}
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}

void WorkItem::ISA_BUFFER_ATOMIC_ADD_Impl(Instruction *instruction)
{
	Instruction::Register value;
	Instruction::Register prev_value;

	int bytes_to_read = 4;
	int bytes_to_write = 4;

	if (INST.glc) {
		wavefront->setVectorMemoryGlobalCoherency(true);
	} else {
		/* NOTE Regardless of whether the glc bit is set by the AMD 
		 * compiler, for the NMOESI protocol correctness , the glc bit
		 * must be set. */
		wavefront->setVectorMemoryGlobalCoherency(true);
	}

	uint32_t addr = CalculateBUFAddress(INST);

	// Read existing value from global memory
	global_mem->Read(addr, bytes_to_read, prev_value.as_byte);
	// Read value to add to existing value from a register
	value.as_int = ReadVReg(INST.vdata);

	// Compute and store the updated value
	value.as_int += prev_value.as_int;
	global_mem->Write(addr, bytes_to_write, value.as_byte);

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: (addr %x)<=(%d) ",
			id, addr, value.as_int);
	
	// If glc bit set, return the previous value in a register
	if (INST.glc) {
		WriteVReg(INST.vdata, prev_value.as_uint);

		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(%d) ", id,
				INST.vdata, value.as_int);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

}

// Write back and invalidate the shader L1. Always returns ACK to shader.
void WorkItem::ISA_BUFFER_WBINVL1_Impl(Instruction *instruction)
{
	// XXX(rzl): There's no cache implementation so we don't need to do
	// anything?
}

/*
 * MTBUF
 */

#undef INST
#define INST instruction->getBytes()->mtbuf

void WorkItem::ISA_TBUFFER_LOAD_FORMAT_X_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	assert(num_elems == 1);
	assert(elem_size == 4);
	const int bytes_to_read = elem_size * num_elems;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < elem_size; ++i) {
		Instruction::Register value;
		global_mem->Read(addr + i * elem_size, elem_size, value.as_byte);
		WriteVReg(INST.vdata + i, value.as_uint);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(addr %x)(%u) ",
				INST.vdata, addr, value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_TBUFFER_LOAD_FORMAT_XY_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	assert(num_elems == 2);
	assert(elem_size == 4);
	const int bytes_to_read = elem_size * num_elems;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < elem_size; ++i) {
		Instruction::Register value;
		global_mem->Read(addr + i * elem_size, elem_size, value.as_byte);
		WriteVReg(INST.vdata + i, value.as_uint);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(addr %x)(%u) ",
				INST.vdata, addr, value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_TBUFFER_LOAD_FORMAT_XYZ_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

void WorkItem::ISA_TBUFFER_LOAD_FORMAT_XYZW_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	assert(num_elems == 4);
	assert(elem_size == 4);
	const int bytes_to_read = elem_size * num_elems;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < elem_size; ++i) {
		Instruction::Register value;
		global_mem->Read(addr + i * elem_size, elem_size, value.as_byte);
		WriteVReg(INST.vdata + i, value.as_uint);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("V%u<=(addr %x)(%u) ",
				INST.vdata, addr, value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_TBUFFER_STORE_FORMAT_X_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	assert(num_elems == 1);
	assert(elem_size == 4);
	const int bytes_to_write = elem_size * num_elems;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < elem_size; ++i) {
		Instruction::Register value;
		value.as_uint = ReadVReg(INST.vdata + i);
		global_mem->Write(addr + i * elem_size, elem_size, value.as_byte);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("(addr %x)<=V%u(%u) ",
				addr, INST.vdata, value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}

void WorkItem::ISA_TBUFFER_STORE_FORMAT_XY_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	assert(num_elems == 2);
	assert(elem_size == 4);
	const int bytes_to_write = elem_size * num_elems;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < elem_size; ++i) {
		Instruction::Register value;
		value.as_uint = ReadVReg(INST.vdata + i);
		global_mem->Write(addr + i * elem_size, elem_size, value.as_byte);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("(addr %x)<=V%u(%u) ",
				addr, INST.vdata, value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}

void WorkItem::ISA_TBUFFER_STORE_FORMAT_XYZW_Impl(Instruction *instruction)
{
	uint32_t addr = CalculateBUFAddress(INST);

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	assert(num_elems == 4);
	assert(elem_size == 4);
	const int bytes_to_write = elem_size * num_elems;

	if (Emulator::isa_debug)
		Emulator::isa_debug << misc::fmt("t%d: ", id);

	for (int i = 0; i < elem_size; ++i) {
		Instruction::Register value;
		value.as_uint = ReadVReg(INST.vdata + i);
		global_mem->Write(addr + i * elem_size, elem_size, value.as_byte);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("(addr %x)<=V%u(%u) ",
				addr, INST.vdata, value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}
#undef INST

} // namespace SI
