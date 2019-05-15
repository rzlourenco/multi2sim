#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->mubuf
void WorkItem::ISA_BUFFER_LOAD_SBYTE_Impl(Instruction *instruction)
{

	assert(!INST.addr64);
	assert(!INST.glc);
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_read = 1;

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("The buffer descriptor is probably invalid");

	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	
	global_mem->Read(addr, bytes_to_read, (char *)&value);
	
	// Sign extend
	value.as_int = (int) value.as_byte[0];

	WriteVReg(INST.vdata, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%d) ", id,
			INST.vdata, addr, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_LOAD_DWORD_Impl(Instruction *instruction)
{
	assert(!INST.glc);
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_read = 4;

	// srsrc is in units of 4 registers
	int srsrc = INST.srsrc << 2;
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
	}
	else
	{
	        // Table 8.3 from SI ISA
	        if (!INST.idxen && INST.offen)
	        {
	            off_vgpr = ReadVReg(INST.vaddr);
	        }
	        else if (INST.idxen && !INST.offen)
	        {
	            idx_vgpr = ReadVReg(INST.vaddr);
	        }
	        else if (INST.idxen && INST.offen)
	        {
	            idx_vgpr = ReadVReg(INST.vaddr);
	            off_vgpr = ReadVReg(INST.vaddr + 1);
	        }
	        addr = base + soffset + ioffset + off_vgpr + 
		    stride * (idx_vgpr + id_in_wavefront);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	global_mem->Read(addr, bytes_to_read, (char *)&value);

	WriteVReg(INST.vdata, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%d) ", id,
			INST.vdata, addr, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_LOAD_DWORDX2_Impl(Instruction *instruction)
{
	assert(!INST.glc);
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_read = 8;

	// srsrc is in units of 4 registers
	int srsrc = INST.srsrc << 2;
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
	}
	else
	{
	        // Table 8.3 from SI ISA
	        if (!INST.idxen && INST.offen)
	        {
	            off_vgpr = ReadVReg(INST.vaddr);
	        }
	        else if (INST.idxen && !INST.offen)
	        {
	            idx_vgpr = ReadVReg(INST.vaddr);
	        }
	        else if (INST.idxen && INST.offen)
	        {
	            idx_vgpr = ReadVReg(INST.vaddr);
	            off_vgpr = ReadVReg(INST.vaddr + 1);
	        }
	        addr = base + soffset + ioffset + off_vgpr + 
		    stride * (idx_vgpr + id_in_wavefront);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	global_mem->Read(addr, 4, (char *)&value);
	WriteVReg(INST.vdata, value.as_uint);
	global_mem->Read(addr + 4, 4, (char *)&value);
	WriteVReg(INST.vdata + 1, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	if (Emulator::isa_debug)
	{
		// FIXME(rzl): update trace format.
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%d) ", id,
			INST.vdata, addr, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_LOAD_DWORDX4_Impl(Instruction *instruction)
{
	assert(!INST.glc);
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_read = 16;

	// srsrc is in units of 4 registers
	int srsrc = INST.srsrc << 2;
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
	}
	else
	{
	        // Table 8.3 from SI ISA
	        if (!INST.idxen && INST.offen)
	        {
	            off_vgpr = ReadVReg(INST.vaddr);
	        }
	        else if (INST.idxen && !INST.offen)
	        {
	            idx_vgpr = ReadVReg(INST.vaddr);
	        }
	        else if (INST.idxen && INST.offen)
	        {
	            idx_vgpr = ReadVReg(INST.vaddr);
	            off_vgpr = ReadVReg(INST.vaddr + 1);
	        }
	        addr = base + soffset + ioffset + off_vgpr + 
		    stride * (idx_vgpr + id_in_wavefront);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	global_mem->Read(addr, 4, (char *)&value);
	WriteVReg(INST.vdata, value.as_uint);
	global_mem->Read(addr + 4, 4, (char *)&value);
	WriteVReg(INST.vdata + 1, value.as_uint);
	global_mem->Read(addr + 8, 4, (char *)&value);
	WriteVReg(INST.vdata + 2, value.as_uint);
	global_mem->Read(addr + 12, 4, (char *)&value);
	WriteVReg(INST.vdata + 3, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	if (Emulator::isa_debug)
	{
		// FIXME(rzl): update trace format.
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%d) ", id,
			INST.vdata, addr, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_STORE_BYTE_Impl(Instruction *instruction)
{

	assert(!INST.addr64);
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_write = 1;

	if (INST.glc)
	{
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant
	}

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	value.as_int = ReadVReg(INST.vdata);

	global_mem->Write(addr, bytes_to_write, (char *)&value);
	
	// Sign extend
	//value.as_int = (int) value.as_byte[0];

	WriteVReg(INST.vdata, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%d) ", id,
			INST.vdata, addr, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_STORE_DWORD_Impl(Instruction *instruction)
{
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_write = 4;

	if (INST.glc)
	{
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant
	}

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned soffset = ReadSReg(INST.soffset);
	unsigned ioffset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

        uint32_t addr;
	if (INST.addr64) {
	        uint32_t addr_lsb = ReadVReg(INST.vaddr);
	        uint32_t addr_msb = ReadVReg(INST.vaddr+1);
	        uint64_t vaddr = addr_msb;
	        vaddr = (vaddr << 32) | addr_lsb;
	        vaddr += base + ioffset + soffset;
	        // FIXME(rzl): this requires 64-bit addressing in the memory
	        // emulation.
	        addr = (uint32_t)vaddr;
	        assert(addr == vaddr && "need 64-bit addressing support!");
	}
	else
	{
	    // Table 8.3 from SI ISA
	    if (!INST.idxen && INST.offen)
	    {
		off_vgpr = ReadVReg(INST.vaddr);
	    }
	    else if (INST.idxen && !INST.offen)
	    {
		idx_vgpr = ReadVReg(INST.vaddr);
	    }
	    else if (INST.idxen && INST.offen)
	    {
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	    }

	    addr = base + soffset + ioffset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	value.as_int = ReadVReg(INST.vdata);

	global_mem->Write(addr, bytes_to_write, (char *)&value);
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: (%u)<=V%u(%d) ", id,
			addr, INST.vdata, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_STORE_DWORDX2_Impl(Instruction *instruction)
{
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_write = 4 * 2;

	if (INST.glc)
	{
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant
	}

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned soffset = ReadSReg(INST.soffset);
	unsigned ioffset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

        uint32_t addr;
	if (INST.addr64) {
	        uint32_t addr_lsb = ReadVReg(INST.vaddr);
	        uint32_t addr_msb = ReadVReg(INST.vaddr+1);
	        uint64_t vaddr = addr_msb;
	        vaddr = (vaddr << 32) | addr_lsb;
	        vaddr += base + ioffset + soffset;
	        // FIXME(rzl): this requires 64-bit addressing in the memory
	        // emulation.
	        addr = (uint32_t)vaddr;
	        assert(addr == vaddr && "need 64-bit addressing support!");
	}
	else
	{
	    // Table 8.3 from SI ISA
	    if (!INST.idxen && INST.offen)
	    {
		off_vgpr = ReadVReg(INST.vaddr);
	    }
	    else if (INST.idxen && !INST.offen)
	    {
		idx_vgpr = ReadVReg(INST.vaddr);
	    }
	    else if (INST.idxen && INST.offen)
	    {
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	    }

	    addr = base + soffset + ioffset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	value.as_int = ReadVReg(INST.vdata);
	global_mem->Write(addr, 4, (char *)&value);
	value.as_int = ReadVReg(INST.vdata + 1);
	global_mem->Write(addr + 4, 4, (char *)&value);
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	if (Emulator::isa_debug)
	{
		// TODO(rzl): fix debug output
		Emulator::isa_debug << misc::fmt("t%d: (%u)<=V%u(%d) ", id,
			addr, INST.vdata, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_STORE_DWORDX4_Impl(Instruction *instruction)
{
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);
	assert(!INST.addr64 || (INST.addr64 && !INST.offen));
	assert(!INST.addr64 || (INST.addr64 && !INST.idxen));

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_write = 4 * 4;

	if (INST.glc)
	{
		wavefront->setVectorMemoryGlobalCoherency(true); // FIXME redundant
	}

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned soffset = ReadSReg(INST.soffset);
	unsigned ioffset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

        uint32_t addr;
	if (INST.addr64) {
	        uint32_t addr_lsb = ReadVReg(INST.vaddr);
	        uint32_t addr_msb = ReadVReg(INST.vaddr+1);
	        uint64_t vaddr = addr_msb;
	        vaddr = (vaddr << 32) | addr_lsb;
	        vaddr += base + ioffset + soffset;
	        // FIXME(rzl): this requires 64-bit addressing in the memory
	        // emulation.
	        addr = (uint32_t)vaddr;
	        assert(addr == vaddr && "need 64-bit addressing support!");
	}
	else
	{
	    // Table 8.3 from SI ISA
	    if (!INST.idxen && INST.offen)
	    {
		off_vgpr = ReadVReg(INST.vaddr);
	    }
	    else if (INST.idxen && !INST.offen)
	    {
		idx_vgpr = ReadVReg(INST.vaddr);
	    }
	    else if (INST.idxen && INST.offen)
	    {
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	    }

	    addr = base + soffset + ioffset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	value.as_int = ReadVReg(INST.vdata);
	global_mem->Write(addr, 4, (char *)&value);
	value.as_int = ReadVReg(INST.vdata + 1);
	global_mem->Write(addr + 4, 4, (char *)&value);
	value.as_int = ReadVReg(INST.vdata + 2);
	global_mem->Write(addr + 8, 4, (char *)&value);
	value.as_int = ReadVReg(INST.vdata + 3);
	global_mem->Write(addr + 12, 4, (char *)&value);
	
	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	if (Emulator::isa_debug)
	{
		// TODO(rzl): fix debug output
		Emulator::isa_debug << misc::fmt("t%d: (%u)<=V%u(%d) ", id,
			addr, INST.vdata, value.as_int);
	}
}

void WorkItem::ISA_BUFFER_ATOMIC_ADD_Impl(Instruction *instruction)
{

	assert(!INST.addr64);
	assert(!INST.slc);
	assert(!INST.tfe);
	assert(!INST.lds);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;
	Instruction::Register prev_value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int bytes_to_read = 4;
	int bytes_to_write = 4;

	if (INST.glc)
	{
		wavefront->setVectorMemoryGlobalCoherency(true);
	}
	else
	{
		/* NOTE Regardless of whether the glc bit is set by the AMD 
		 * compiler, for the NMOESI protocol correctness , the glc bit
		 * must be set. */
		wavefront->setVectorMemoryGlobalCoherency(true);
	}

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	// Read existing value from global memory
	
	global_mem->Read(addr, bytes_to_read, prev_value.as_byte);

	// Read value to add to existing value from a register
	value.as_int = ReadVReg(INST.vdata);

	// Compute and store the updated value
	value.as_int += prev_value.as_int;
	global_mem->Write(addr, bytes_to_write, (char *)&value);
	
	// If glc bit set, return the previous value in a register
	if (INST.glc)
	{
		WriteVReg(INST.vdata, prev_value.as_uint);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%d) ", id,
			INST.vdata, addr, value.as_int);
	}
}
#undef INST

#define INST instruction->getBytes()->mtbuf
void WorkItem::ISA_TBUFFER_LOAD_FORMAT_X_Impl(Instruction *instruction)
{

	assert(!INST.addr64);
	assert(!INST.tfe);
	assert(!INST.slc);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	int bytes_to_read;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	bytes_to_read = elem_size * num_elems;

	assert(num_elems == 1);
	assert(elem_size == 4);

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	// Calculate the address
	// XXX Need to know when to enable id_in_wavefront
	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + 0/*work_item->id_in_wavefront*/);

	
	global_mem->Read(addr, bytes_to_read, (char *)&value);

	WriteVReg(INST.vdata, value.as_uint);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;

	// TODO Print value based on type
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%u,%gf) ", id,
			INST.vdata, addr, value.as_uint, value.as_float);
		if (INST.offen)
			Emulator::isa_debug << misc::fmt("offen ");
		if (INST.idxen)
			Emulator::isa_debug << misc::fmt("idxen ");
		Emulator::isa_debug << misc::fmt("%u,%u,%u,%u,%u,%u ", base, mem_offset, 
			inst_offset, off_vgpr, idx_vgpr, stride);
	}
}

void WorkItem::ISA_TBUFFER_LOAD_FORMAT_XY_Impl(Instruction *instruction)
{

	assert(!INST.addr64);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	int i;
	int bytes_to_read;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	bytes_to_read = elem_size * num_elems;

	assert(num_elems == 2);
	assert(elem_size == 4);

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	// Calculate the address
	// XXX Need to know when to enable id_in_wavefront
	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + 0/*work_item->id_in_wavefront*/);

	for (i = 0; i < 2; i++)
	{
		
		global_mem->Read(addr+4*i, 4, (char *)&value);

		WriteVReg(INST.vdata + i, value.as_uint);

		// TODO Print value based on type
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%u,%gf) ", id,
				INST.vdata + i, addr+4*i, value.as_uint,
				value.as_float);
		}
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

	assert(!INST.addr64);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	int i;
	int bytes_to_read;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;
	unsigned id_in_wavefront = 0;

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	bytes_to_read = elem_size * num_elems;

	assert(num_elems == 4);
	assert(elem_size == 4);

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	// XXX Need to know when to enable id_in_wavefront
	id_in_wavefront = buffer_descriptor.add_tid_enable ?  id_in_wavefront : 0;
	
	// Calculate the address
	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	for (i = 0; i < 4; i++)
	{
		
		global_mem->Read(addr+4*i, 4, (char *)&value);

		WriteVReg(INST.vdata + i, value.as_uint);

		// TODO Print value based on type
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("t%d: V%u<=(%u)(%u,%gf) ", id,
				INST.vdata + i, addr+4*i, value.as_uint,
				value.as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_read;
}

void WorkItem::ISA_TBUFFER_STORE_FORMAT_X_Impl(Instruction *instruction)
{

	assert(!INST.addr64);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	int bytes_to_write = elem_size * num_elems;

	assert(num_elems == 1);
	assert(elem_size == 4);

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	value.as_uint = ReadVReg(INST.vdata);

	global_mem->Write(addr, bytes_to_write, (char *)&value);

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;

	// TODO Print value based on type
	if (Emulator::isa_debug)
	{
		Emulator::isa_debug << misc::fmt("t%d: (%u)<=V%u(%u,%gf) ", id,
			addr, INST.vdata, value.as_uint,
			value.as_float);
	}
}

void WorkItem::ISA_TBUFFER_STORE_FORMAT_XY_Impl(Instruction *instruction)
{

	assert(!INST.addr64);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;


	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	int bytes_to_write = elem_size * num_elems;

	assert(num_elems == 2);
	assert(elem_size == 4);

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	for (unsigned i = 0; i < 2; i++)
	{
		value.as_uint = ReadVReg(INST.vdata + i);

		global_mem->Write(addr+4*i, 4, (char *)&value);

		// TODO Print value based on type
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("t%d: (%u)<=V%u(%u,%gf) ", id,
				addr, INST.vdata+i, value.as_uint,
				value.as_float);
		}
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}

void WorkItem::ISA_TBUFFER_STORE_FORMAT_XYZW_Impl(Instruction *instruction)
{

	assert(!INST.addr64);

	BufferDescriptor buffer_descriptor;
	Instruction::Register value;

	unsigned off_vgpr = 0;
	unsigned idx_vgpr = 0;

	int elem_size = ISAGetElemSize(INST.dfmt);
	int num_elems = ISAGetNumElems(INST.dfmt);
	int bytes_to_write = elem_size * num_elems;

	assert(num_elems == 4);
	assert(elem_size == 4);

	// srsrc is in units of 4 registers
	ReadBufferResource(INST.srsrc * 4, buffer_descriptor);

	// Figure 8.1 from SI ISA defines address calculation
	unsigned base = buffer_descriptor.base_addr;
	unsigned mem_offset = ReadSReg(INST.soffset);
	unsigned inst_offset = INST.offset;
	unsigned stride = buffer_descriptor.stride;

	// Table 8.3 from SI ISA
	if (!INST.idxen && INST.offen)
	{
		off_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && !INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
	}
	else if (INST.idxen && INST.offen)
	{
		idx_vgpr = ReadVReg(INST.vaddr);
		off_vgpr = ReadVReg(INST.vaddr + 1);
	}

	/* It wouldn't make sense to have a value for idxen without
	 * having a stride */
	if (idx_vgpr && !stride)
		throw misc::Panic("Probably invalid buffer descriptor");

	// Calculate the address
	unsigned addr = base + mem_offset + inst_offset + off_vgpr + 
		stride * (idx_vgpr + id_in_wavefront);

	for (unsigned i = 0; i < 4; i++)
	{
		value.as_uint = ReadVReg(INST.vdata + i);

		global_mem->Write(addr+4*i, 4, (char *)&value);

		// TODO Print value based on type
		if (Emulator::isa_debug)
			Emulator::isa_debug << misc::fmt("t%d: (%u)<=V%u(%u,%gf) ",
				id,
				addr,
				INST.vdata + i,
				value.as_uint,
				value.as_float);
	}

	// Record last memory access for the detailed simulator.
	global_memory_access_address = addr;
	global_memory_access_size = bytes_to_write;
}
#undef INST

} // namespace SI
