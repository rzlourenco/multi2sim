libarch_arm_src = [
  'disassembler/Disassembler.cc',
  'disassembler/Disassembler.h',
  'disassembler/Instruction.cc',
  'disassembler/Instruction.h',
  'disassembler/Instruction.inc.h',
  'disassembler/InstructionThumb.inc.h',
  'disassembler/InstructionThumb32.inc.h',
  'emulator/Context.cc',
  'emulator/Context.h',
  'emulator/ContextIsa.cc',
  'emulator/ContextIsaArm32.cc',
  'emulator/ContextIsaThumb16.cc',
  'emulator/ContextIsaThumb32.cc',
  'emulator/ContextLoader.cc',
  'emulator/ContextSignal.cc',
  'emulator/ContextSyscall.cc',
  'emulator/ContextSyscall.inc.h',
  'emulator/Emulator.cc',
  'emulator/Emulator.h',
  'emulator/Regs.cc',
  'emulator/Regs.h',
  'emulator/Signal.h',
]

libarch_arm_lib = static_library('arch-arm',
  sources: libarch_arm_src,
  include_directories: m2s_inc)
libarch_arm = declare_dependency(
  link_with: libarch_arm_lib,
  include_directories: m2s_inc)
