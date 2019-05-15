#include "WorkItem.h"
#include "Emulator.h"

namespace SI {

#define INST instruction->getBytes()->sopp

void WorkItem::ISA_S_NOP_Impl(Instruction *instruction)
{
	// FIXME(rzl): shouldn't this be handled at the wavefront level?
}

// End the program.
void WorkItem::ISA_S_ENDPGM_Impl(Instruction *instruction)
{
	wavefront->setFinished(true);
	work_group->incWavefrontsCompletedEmu();
}

// PC = PC + signext(SIMM16 * 4) + 4
void WorkItem::ISA_S_BRANCH_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	// Load the short constant operand and sign extend into an integer.
	simm16 = INST.simm16;
	se_simm16 = simm16;

	// Relative jump
	wavefront->incPC(se_simm16 * 4 + 4 - instruction->getSize());
}

// if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
void WorkItem::ISA_S_CBRANCH_SCC0_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	if (!ReadSReg(Instruction::RegisterScc))
	{
		/* Load the short constant operand and sign extend into an
		 * integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		// Determine the program counter to branch to.
		wavefront->incPC(
			se_simm16 * 4 + 4 - instruction->getSize());
	}
}


// if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
void WorkItem::ISA_S_CBRANCH_SCC1_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	Instruction::Register scc;

	scc.as_uint = ReadSReg(Instruction::RegisterScc);

	if (scc.as_uint)
	{
		assert(ReadSReg(Instruction::RegisterScc) == 1);

		/* Load the short constant operand and sign extend into an
		 * integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		// Determine the program counter to branch to.
		wavefront->incPC(
			se_simm16 * 4 + 4 - instruction->getSize());

		// Print isa debug information.
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("wf%d: SCC=%u (taken)", 
				wavefront->getId(), scc.as_uint);
		}
	}
	else
	{
		// Print isa debug information.
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("wf%d: SCC=%u (not taken)", 
				wavefront->getId(), scc.as_uint);
		}
	}
}

// if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
void WorkItem::ISA_S_CBRANCH_VCCZ_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	if (ReadSReg(Instruction::RegisterVccz))
	{
		/* Load the short constant operand and sign extend into an
		 * integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		// Determine the program counter to branch to.
		wavefront->incPC(
			se_simm16 * 4 + 4 - instruction->getSize());
	}
}

// if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
void WorkItem::ISA_S_CBRANCH_VCCNZ_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	if (!ReadSReg(Instruction::RegisterVccz))
	{
		/* Load the short constant operand and sign extend into an
		 * integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		// Determine the program counter to branch to.
		wavefront->incPC(
			se_simm16 * 4 + 4 - instruction->getSize());
	}
}

// if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
void WorkItem::ISA_S_CBRANCH_EXECZ_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	Instruction::Register exec;
	Instruction::Register execz;

	exec.as_uint = ReadSReg(Instruction::RegisterExec);
	execz.as_uint = ReadSReg(Instruction::RegisterExecz);

	if (execz.as_uint)
	{
		/* Load the short constant operand and sign extend into an
		 * integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		// Determine the program counter to branch to.
		wavefront->incPC(
			se_simm16 * 4 + 4 - instruction->getSize());

		// Print isa debug information.
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("wf%d: EXEC=0x%x, EXECZ=%u (taken)", 
				wavefront->getId(), exec.as_uint, 
				execz.as_uint);
		}
	}
	else
	{
		// Print isa debug information.
		if (Emulator::isa_debug)
		{
			Emulator::isa_debug << misc::fmt("wf%d: EXEC=0x%x, EXECZ=%u " 
				"(not taken)", wavefront->getId(), 
				exec.as_uint, execz.as_uint);
		}
	}
}

// if(EXEC != 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop.
void WorkItem::ISA_S_CBRANCH_EXECNZ_Impl(Instruction *instruction)
{
	short simm16;
	int se_simm16;

	if (!ReadSReg(Instruction::RegisterExecz))
	{
		/* Load the short constant operand and sign extend into an
		 * integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		// Determine the program counter to branch to.
		wavefront->incPC(
			se_simm16 * 4 + 4 - instruction->getSize());
	}
}

/* Suspend current wavefront at the barrier. If all wavefronts in work-group
 * reached the barrier, wake them up */
void WorkItem::ISA_S_BARRIER_Impl(Instruction *instruction)
{
	// Suspend current wavefront at the barrier
	wavefront->setBarrierInstruction(true);
	wavefront->setAtBarrier(true);
	work_group->incWavefrontsAtBarrier();

	Emulator::isa_debug << misc::fmt("Group %d wavefront %d reached barrier "
		"(%d reached, %d left)\n",
		work_group->getId(), wavefront->getId(), 
		work_group->getWavefrontsAtBarrier(),
		work_group->getWavefrontsInWorkgroup() - 
		work_group->getWavefrontsAtBarrier());


	// If all wavefronts in work-group reached the barrier, wake them up
	if (work_group->getWavefrontsAtBarrier() == work_group->getWavefrontsInWorkgroup())
	{
		for (auto i = work_group->getWavefrontsBegin(),
				e = work_group->getWavefrontsEnd();
				i != e;
				++i)
			(*i)->setAtBarrier(false);

		work_group->setWavefrontsAtBarrier(0);

		Emulator::isa_debug << misc::fmt("Group %d completed barrier\n", work_group->getId());
	}
}

void WorkItem::ISA_S_WAITCNT_Impl(Instruction *instruction)
{
	// Nothing to do in emulation
	wavefront->setMemoryWait(true);
}

void WorkItem::ISA_PHI_Impl(Instruction *instruction)
{
	ISAUnimplemented(instruction);
}

} // namespace SI
