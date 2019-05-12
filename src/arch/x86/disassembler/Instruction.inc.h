DEFINST(adc_al_imm8, 0x14, SKIP, SKIP, SKIP, IB, 0)
DEFINST(adc_ax_imm16, 0x15, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(adc_eax_imm32, 0x15, SKIP, SKIP, SKIP, ID, 0)
DEFINST(adc_rm8_imm8, 0x80, SKIP, SKIP, 2, IB, 0)
DEFINST(adc_rm16_imm16, 0x81, SKIP, SKIP, 2, IW, Instruction::PrefixOp)
DEFINST(adc_rm32_imm32, 0x81, SKIP, SKIP, 2, ID, 0)
DEFINST(adc_rm16_imm8, 0x83, SKIP, SKIP, 2, IB, Instruction::PrefixOp)
DEFINST(adc_rm32_imm8, 0x83, SKIP, SKIP, 2, IB, 0)
DEFINST(adc_rm8_r8, 0x10, SKIP, SKIP, REG, SKIP, 0)
DEFINST(adc_rm16_r16, 0x11, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(adc_rm32_r32, 0x11, SKIP, SKIP, REG, SKIP, 0)
DEFINST(adc_r8_rm8, 0x12, SKIP, SKIP, REG, SKIP, 0)
DEFINST(adc_r16_rm16, 0x13, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(adc_r32_rm32, 0x13, SKIP, SKIP, REG, SKIP, 0)

DEFINST(add_al_imm8, 0x04, SKIP, SKIP, SKIP, IB, 0)
DEFINST(add_ax_imm16, 0x05, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(add_eax_imm32, 0x05, SKIP, SKIP, SKIP, ID, 0)
DEFINST(add_rm8_imm8, 0x80, SKIP, SKIP, 0, IB, 0)
DEFINST(add_rm16_imm16, 0x81, SKIP, SKIP, 0, IW, Instruction::PrefixOp)
DEFINST(add_rm32_imm32, 0x81, SKIP, SKIP, 0, ID, 0)
DEFINST(add_rm16_imm8, 0x83, SKIP, SKIP, 0, IB, Instruction::PrefixOp)
DEFINST(add_rm32_imm8, 0x83, SKIP, SKIP, 0, IB, 0)
DEFINST(add_rm8_r8, 0x00, SKIP, SKIP, REG, SKIP, 0)
DEFINST(add_rm16_r16, 0x01, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(add_rm32_r32, 0x01, SKIP, SKIP, REG, SKIP, 0)
DEFINST(add_r8_rm8, 0x02, SKIP, SKIP, REG, SKIP, 0)
DEFINST(add_r16_rm16, 0x03, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(add_r32_rm32, 0x03, SKIP, SKIP, REG, SKIP, 0)

DEFINST(addpd_xmm_xmmm128, 0x0f, 0x58, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(addps_xmm_xmmm128, 0x0f, 0x58, SKIP, REG, SKIP, 0)

DEFINST(addsd_xmm_xmmm64, 0x0f, 0x58, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(addss_xmm_xmmm32, 0x0f, 0x58, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(and_al_imm8, 0x24, SKIP, SKIP, SKIP, IB, 0)
DEFINST(and_ax_imm16, 0x25, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(and_eax_imm32, 0x25, SKIP, SKIP, SKIP, ID, 0)
DEFINST(and_rm8_imm8, 0x80, SKIP, SKIP, 4, IB, 0)
DEFINST(and_rm16_imm16, 0x81, SKIP, SKIP, 4, IW, Instruction::PrefixOp)
DEFINST(and_rm32_imm32, 0x81, SKIP, SKIP, 4, ID, 0)
DEFINST(and_rm16_imm8, 0x83, SKIP, SKIP, 4, IB, Instruction::PrefixOp)
DEFINST(and_rm32_imm8, 0x83, SKIP, SKIP, 4, IB, 0)
DEFINST(and_rm8_r8, 0x20, SKIP, SKIP, REG, SKIP, 0)
DEFINST(and_rm16_r16, 0x21, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(and_rm32_r32, 0x21, SKIP, SKIP, REG, SKIP, 0)
DEFINST(and_r8_rm8, 0x22, SKIP, SKIP, REG, SKIP, 0)
DEFINST(and_r16_rm16, 0x23, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(and_r32_rm32, 0x23, SKIP, SKIP, REG, SKIP, 0)

DEFINST(andpd_xmm_xmmm128, 0x0f, 0x54, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(andps_xmm_xmmm128, 0x0f, 0x54, SKIP, REG, SKIP, 0)

DEFINST(bound_r16_rm32, 0x62, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(bound_r32_rm64, 0x62, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(bsf_r32_rm32, 0x0f, 0xbc, SKIP, REG, SKIP, 0)

DEFINST(bsr_r32_rm32, 0x0f, 0xbd, SKIP, REG, SKIP, 0)

DEFINST(bswap_ir32, 0x0f, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(bt_rm32_r32, 0x0f, 0xa3, SKIP, REG, SKIP, 0)
DEFINST(bt_rm32_imm8, 0x0f, 0xba, SKIP, 4, IB, 0)

DEFINST(bts_rm32_imm8, 0x0f, 0xba, SKIP, 5, IB, 0)

DEFINST(call_rel32, 0xe8, SKIP, SKIP, SKIP, ID, 0)
DEFINST(call_rm32, 0xff, SKIP, SKIP, 2, SKIP, 0)

DEFINST(cbw, 0x98, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixOp)
DEFINST(cwde, 0x98, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(cdq, 0x99, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(cld, 0xfc, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(cmova_r16_rm16, 0x0f, 0x47, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmova_r32_rm32, 0x0f, 0x47, SKIP, REG, SKIP, 0)
DEFINST(cmovae_r16_rm16, 0x0f, 0x43, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovae_r32_rm32, 0x0f, 0x43, SKIP, REG, SKIP, 0)
DEFINST(cmovb_r16_rm16, 0x0f, 0x42, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovb_r32_rm32, 0x0f, 0x42, SKIP, REG, SKIP, 0)
DEFINST(cmovbe_r16_rm16, 0x0f, 0x46, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovbe_r32_rm32, 0x0f, 0x46, SKIP, REG, SKIP, 0)
DEFINST(cmove_r16_rm16, 0x0f, 0x44, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmove_r32_rm32, 0x0f, 0x44, SKIP, REG, SKIP, 0)
DEFINST(cmovg_r16_rm16, 0x0f, 0x4f, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovg_r32_rm32, 0x0f, 0x4f, SKIP, REG, SKIP, 0)
DEFINST(cmovge_r16_rm16, 0x0f, 0x4d, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovge_r32_rm32, 0x0f, 0x4d, SKIP, REG, SKIP, 0)
DEFINST(cmovl_r16_rm16, 0x0f, 0x4c, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovl_r32_rm32, 0x0f, 0x4c, SKIP, REG, SKIP, 0)
DEFINST(cmovle_r16_rm16, 0x0f, 0x4e, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovle_r32_rm32, 0x0f, 0x4e, SKIP, REG, SKIP, 0)
DEFINST(cmovne_r16_rm16, 0x0f, 0x45, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovne_r32_rm32, 0x0f, 0x45, SKIP, REG, SKIP, 0)
DEFINST(cmovno_r16_rm16, 0x0f, 0x41, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovno_r32_rm32, 0x0f, 0x41, SKIP, REG, SKIP, 0)
DEFINST(cmovnp_r16_rm16, 0x0f, 0x4b, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovnp_r32_rm32, 0x0f, 0x4b, SKIP, REG, SKIP, 0)
DEFINST(cmovns_r16_rm16, 0x0f, 0x49, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovns_r32_rm32, 0x0f, 0x49, SKIP, REG, SKIP, 0)
DEFINST(cmovo_r16_rm16, 0x0f, 0x40, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovo_r32_rm32, 0x0f, 0x40, SKIP, REG, SKIP, 0)
DEFINST(cmovp_r16_rm16, 0x0f, 0x4a, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovp_r32_rm32, 0x0f, 0x4a, SKIP, REG, SKIP, 0)
DEFINST(cmovs_r16_rm16, 0x0f, 0x48, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmovs_r32_rm32, 0x0f, 0x48, SKIP, REG, SKIP, 0)

DEFINST(cmp_al_imm8, 0x3c, SKIP, SKIP, SKIP, IB, 0)
DEFINST(cmp_ax_imm16, 0x3d, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(cmp_eax_imm32, 0x3d, SKIP, SKIP, SKIP, ID, 0)
DEFINST(cmp_rm8_imm8, 0x80, SKIP, SKIP, 7, IB, 0)
DEFINST(cmp_rm16_imm16, 0x81, SKIP, SKIP, 7, IW, Instruction::PrefixOp)
DEFINST(cmp_rm32_imm32, 0x81, SKIP, SKIP, 7, ID, 0)
DEFINST(cmp_rm16_imm8, 0x83, SKIP, SKIP, 7, IB, Instruction::PrefixOp)
DEFINST(cmp_rm32_imm8, 0x83, SKIP, SKIP, 7, IB, 0)
DEFINST(cmp_rm8_r8, 0x38, SKIP, SKIP, REG, SKIP, 0)
DEFINST(cmp_rm16_r16, 0x39, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmp_rm32_r32, 0x39, SKIP, SKIP, REG, SKIP, 0)
DEFINST(cmp_r8_rm8, 0x3a, SKIP, SKIP, REG, SKIP, 0)
DEFINST(cmp_r16_rm16, 0x3b, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cmp_r32_rm32, 0x3b, SKIP, SKIP, REG, SKIP, 0)

DEFINST(cmppd_xmm_xmmm128_imm8, 0x0f, 0xc2, SKIP, REG, IB, Instruction::PrefixOp)
DEFINST(cmpps_xmm_xmmm128_imm8, 0x0f, 0xc2, SKIP, REG, IB, 0)

DEFINST(cmpsb, 0xa6, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(cmpsd, 0xa7, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(cmpxchg_rm32_r32, 0x0f, 0xb1, SKIP, REG, SKIP, 0)

DEFINST(cmpxchg8b_m64, 0x0f, 0xc7, SKIP, MEM|1, SKIP, 0)

DEFINST(cpuid, 0x0f, 0xa2, SKIP, SKIP, SKIP, 0)

DEFINST(cvtdq2pd_xmm_xmmm64, 0x0f, 0xe6, SKIP, REG, SKIP, Instruction::PrefixRep)
DEFINST(cvtdq2ps_xmm_xmmm128, 0x0f, 0x5b, SKIP, REG, SKIP, 0)

DEFINST(cvtsd2si_r32_xmmm64, 0x0f, 0x2d, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(cvtsd2ss_xmm_xmmm64, 0x0f, 0x5a, SKIP, REG, SKIP, Instruction::PrefixRepnz)

DEFINST(cvtpd2ps_xmm_xmmm128, 0x0f, 0x5a, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cvtps2pd_xmm_xmmm64, 0x0f, 0x5a, SKIP, REG, SKIP, 0)

DEFINST(cvtsi2sd_xmm_rm32, 0x0f, 0x2a, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(cvtsi2ss_xmm_rm32, 0x0f, 0x2a, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(cvtss2sd_xmm_xmmm32, 0x0f, 0x5a, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(cvttpd2dq_xmm_xmmm128, 0x0f, 0xe6, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(cvttps2dq_xmm_xmmm128, 0x0f, 0x5b, SKIP, REG, SKIP, Instruction::PrefixRep)
DEFINST(cvttsd2si_r32_xmmm64, 0x0f, 0x2c, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(cvttss2si_r32_xmmm32, 0x0f, 0x2c, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(dec_rm8, 0xfe, SKIP, SKIP, 1, SKIP, 0)
DEFINST(dec_rm16, 0xff, SKIP, SKIP, 1, SKIP, Instruction::PrefixOp)
DEFINST(dec_rm32, 0xff, SKIP, SKIP, 1, SKIP, 0)
DEFINST(dec_ir16, 0x48|INDEX, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixOp)
DEFINST(dec_ir32, 0x48|INDEX, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(div_rm8, 0xf6, SKIP, SKIP, 6, SKIP, 0)
DEFINST(div_rm32, 0xf7, SKIP, SKIP, 6, SKIP, 0)

DEFINST(divpd_xmm_xmmm128, 0x0f, 0x5e, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(divps_xmm_xmmm128, 0x0f, 0x5e, SKIP, REG, SKIP, 0)

DEFINST(divsd_xmm_xmmm64, 0x0f, 0x5e, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(divss_xmm_xmmm32, 0x0f, 0x5e, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(endbr32, 0x0f, 0x1e, 0xfb, SKIP, SKIP, Instruction::PrefixRepz)

DEFINST(f2xm1, 0xd9, 0xf0, SKIP, SKIP, SKIP, 0)

DEFINST(fabs, 0xd9, 0xe1, SKIP, SKIP, SKIP, 0)

DEFINST(fadd_m32, 0xd8, SKIP, SKIP, MEM|0, SKIP, 0)
DEFINST(fadd_m64, 0xdc, SKIP, SKIP, MEM|0, SKIP, 0)
DEFINST(fadd_st0_sti, 0xd8, 0xc0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fadd_sti_st0, 0xdc, 0xc0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(faddp_sti_st0, 0xde, 0xc0|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fchs, 0xd9, 0xe0, SKIP, SKIP, SKIP, 0)

DEFINST(fcmovb_st0_sti, 0xda, 0xc0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmove_st0_sti, 0xda, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmovbe_st0_sti, 0xda, 0xd0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmovu_st0_sti, 0xda, 0xd8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmovnb_st0_sti, 0xdb, 0xc0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmovne_st0_sti, 0xdb, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmovnbe_st0_sti, 0xdb, 0xd0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcmovnu_st0_sti, 0xdb, 0xd8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fcom_m32, 0xd8, SKIP, SKIP, MEM|2, SKIP, 0)
DEFINST(fcom_m64, 0xdc, SKIP, SKIP, MEM|2, SKIP, 0)
DEFINST(fcom_sti, 0xd8, 0xd0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcomp_m32, 0xd8, SKIP, SKIP, MEM|3, SKIP, 0)
DEFINST(fcomp_m64, 0xdc, SKIP, SKIP, MEM|3, SKIP, 0)
DEFINST(fcomp_sti, 0xd8, 0xd8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcompp, 0xde, 0xd9, SKIP, SKIP, SKIP, 0)

DEFINST(fcomi_st0_sti, 0xdb, 0xf0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fcomip_st0_sti, 0xdf, 0xf0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fucomi_st0_sti, 0xdb, 0xe8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fucomip_st0_sti, 0xdf, 0xe8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fcos, 0xd9, 0xff, SKIP, SKIP, SKIP, 0)

DEFINST(fdiv_m32, 0xd8, SKIP, SKIP, MEM|6, SKIP, 0)
DEFINST(fdiv_m64, 0xdc, SKIP, SKIP, MEM|6, SKIP, 0)
DEFINST(fdiv_st0_sti, 0xd8, 0xf0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fdiv_sti_st0, 0xdc, 0xf8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fdivp_sti_st0, 0xde, 0xf8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fdivr_m32, 0xd8, SKIP, SKIP, MEM|7, SKIP, 0)
DEFINST(fdivr_m64, 0xdc, SKIP, SKIP, MEM|7, SKIP, 0)
DEFINST(fdivr_st0_sti, 0xd8, 0xf8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fdivr_sti_st0, 0xdc, 0xf0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fdivrp_sti_st0, 0xde, 0xf0|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fild_m16, 0xdf, SKIP, SKIP, MEM|0, SKIP, 0)
DEFINST(fild_m32, 0xdb, SKIP, SKIP, MEM|0, SKIP, 0)
DEFINST(fild_m64, 0xdf, SKIP, SKIP, MEM|5, SKIP, 0)

DEFINST(fist_m16, 0xdf, SKIP, SKIP, MEM|2, SKIP, 0)
DEFINST(fist_m32, 0xdb, SKIP, SKIP, MEM|2, SKIP, 0)
DEFINST(fistp_m16, 0xdf, SKIP, SKIP, MEM|3, SKIP, 0)
DEFINST(fistp_m32, 0xdb, SKIP, SKIP, MEM|3, SKIP, 0)
DEFINST(fistp_m64, 0xdf, SKIP, SKIP, MEM|7, SKIP, 0)

DEFINST(fld1, 0xd9, 0xe8, SKIP, SKIP, SKIP, 0)
DEFINST(fldl2t, 0xd9, 0xe9, SKIP, SKIP, SKIP, 0)
DEFINST(fldl2e, 0xd9, 0xea, SKIP, SKIP, SKIP, 0)
DEFINST(fldpi, 0xd9, 0xeb, SKIP, SKIP, SKIP, 0)
DEFINST(fldlg2, 0xd9, 0xec, SKIP, SKIP, SKIP, 0)
DEFINST(fldln2, 0xd9, 0xed, SKIP, SKIP, SKIP, 0)
DEFINST(fldz, 0xd9, 0xee, SKIP, SKIP, SKIP, 0)

DEFINST(fld_m32, 0xd9, SKIP, SKIP, MEM|0, SKIP, 0)
DEFINST(fld_m64, 0xdd, SKIP, SKIP, MEM|0, SKIP, 0)
DEFINST(fld_m80, 0xdb, SKIP, SKIP, MEM|5, SKIP, 0)
DEFINST(fld_sti, 0xd9, 0xc0|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fldcw_m16, 0xd9, SKIP, SKIP, MEM|5, SKIP, 0)

DEFINST(fmul_m32, 0xd8, SKIP, SKIP, MEM|1, SKIP, 0)
DEFINST(fmul_m64, 0xdc, SKIP, SKIP, MEM|1, SKIP, 0)
DEFINST(fmul_st0_sti, 0xd8, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fmul_sti_st0, 0xdc, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fmulp_sti_st0, 0xde, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fpatan, 0xd9, 0xf3, SKIP, SKIP, SKIP, 0)

DEFINST(fptan, 0xd9, 0xf2, SKIP, SKIP, SKIP, 0)

DEFINST(frndint, 0xd9, 0xfc, SKIP, SKIP, SKIP, 0)

DEFINST(fprem, 0xd9, 0xf8, SKIP, SKIP, SKIP, 0)

DEFINST(fprem1, 0xd9, 0xf5, SKIP, SKIP, SKIP, 0)

DEFINST(fscale, 0xd9, 0xfd, SKIP, SKIP, SKIP, 0)

DEFINST(fsin, 0xd9, 0xfe, SKIP, SKIP, SKIP, 0)

DEFINST(fsincos, 0xd9, 0xfb, SKIP, SKIP, SKIP, 0)

DEFINST(fsqrt, 0xd9, 0xfa, SKIP, SKIP, SKIP, 0)

DEFINST(fstcw_m16, 0x9b, 0xd9, SKIP, MEM|7, SKIP, 0)
DEFINST(fnstcw_m16, 0xd9, SKIP, SKIP, MEM|7, SKIP, 0)

DEFINST(fst_m32, 0xd9, SKIP, SKIP, MEM|2, SKIP, 0)
DEFINST(fst_m64, 0xdd, SKIP, SKIP, MEM|2, SKIP, 0)
DEFINST(fst_sti, 0xdd, 0xd0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fstp_m32, 0xd9, SKIP, SKIP, MEM|3, SKIP, 0)
DEFINST(fstp_m64, 0xdd, SKIP, SKIP, MEM|3, SKIP, 0)
DEFINST(fstp_m80, 0xdb, SKIP, SKIP, MEM|7, SKIP, 0)
DEFINST(fstp_sti, 0xdd, 0xd8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fstsw_ax, 0x9b, 0xdf, 0xe0, SKIP, SKIP, 0)
DEFINST(fnstsw_ax, 0xdf, 0xe0, SKIP, SKIP, SKIP, 0)

DEFINST(fsub_m32, 0xd8, SKIP, SKIP, MEM|4, SKIP, 0)
DEFINST(fsub_m64, 0xdc, SKIP, SKIP, MEM|4, SKIP, 0)
DEFINST(fsub_st0_sti, 0xd8, 0xe0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fsub_sti_st0, 0xdc, 0xe8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fsubp_sti_st0, 0xde, 0xe8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fsubr_m32, 0xd8, SKIP, SKIP, MEM|5, SKIP, 0)
DEFINST(fsubr_m64, 0xdc, SKIP, SKIP, MEM|5, SKIP, 0)
DEFINST(fsubr_st0_sti, 0xd8, 0xe8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fsubr_sti_st0, 0xdc, 0xe0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fsubrp_sti_st0, 0xde, 0xe0|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(ftst, 0xd9, 0xe4, SKIP, SKIP, SKIP, 0)

DEFINST(fucom_sti, 0xdd, 0xe0|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fucomp_sti, 0xdd, 0xe8|INDEX, SKIP, SKIP, SKIP, 0)
DEFINST(fucompp, 0xda, 0xe9, SKIP, SKIP, SKIP, 0)

DEFINST(fxam, 0xd9, 0xe5, SKIP, SKIP, SKIP, 0)

DEFINST(fxch_sti, 0xd9, 0xc8|INDEX, SKIP, SKIP, SKIP, 0)

DEFINST(fyl2x, 0xd9, 0xf1, SKIP, SKIP, SKIP, 0)

DEFINST(fyl2xp1, 0xd9, 0xf9, SKIP, SKIP, SKIP, 0)

DEFINST(hlt, 0xf4, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(idiv_rm32, 0xf7, SKIP, SKIP, 7, SKIP, 0)

DEFINST(imul_rm32, 0xf7, SKIP, SKIP, 5, SKIP, 0)
DEFINST(imul_r16_rm16, 0x0f, 0xaf, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(imul_r32_rm32, 0x0f, 0xaf, SKIP, REG, SKIP, 0)
DEFINST(imul_r32_rm32_imm8, 0x6b, SKIP, SKIP, REG, IB, 0)
DEFINST(imul_r32_rm32_imm32, 0x69, SKIP, SKIP, REG, ID, 0)

DEFINST(inc_rm8, 0xfe, SKIP, SKIP, 0, SKIP, 0)
DEFINST(inc_rm16, 0xff, SKIP, SKIP, 0, SKIP, Instruction::PrefixOp)
DEFINST(inc_rm32, 0xff, SKIP, SKIP, 0, SKIP, 0)
DEFINST(inc_ir16, 0x40|INDEX, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixOp)
DEFINST(inc_ir32, 0x40|INDEX, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(insb, 0x6c, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(insd, 0x6d, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(int_3, 0xcc, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(int_imm8, 0xcd, SKIP, SKIP, SKIP, IB, 0)
DEFINST(into, 0xce, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(ja_rel8, 0x77, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jae_rel8, 0x73, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jb_rel8, 0x72, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jbe_rel8, 0x76, SKIP, SKIP, SKIP, IB, 0)
DEFINST(je_rel8, 0x74, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jcxz_rel8, 0xe3, SKIP, SKIP, SKIP, IB, Instruction::PrefixOp)
DEFINST(jecxz_rel8, 0xe3, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jg_rel8, 0x7f, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jge_rel8, 0x7d, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jl_rel8, 0x7c, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jle_rel8, 0x7e, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jne_rel8, 0x75, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jno_rel8, 0x71, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jnp_rel8, 0x7b, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jns_rel8, 0x79, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jo_rel8, 0x70, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jp_rel8, 0x7a, SKIP, SKIP, SKIP, IB, 0)
DEFINST(js_rel8, 0x78, SKIP, SKIP, SKIP, IB, 0)
DEFINST(ja_rel32, 0x0f, 0x87, SKIP, SKIP, ID, 0)
DEFINST(jae_rel32, 0x0f, 0x83, SKIP, SKIP, ID, 0)
DEFINST(jb_rel32, 0x0f, 0x82, SKIP, SKIP, ID, 0)
DEFINST(jbe_rel32, 0x0f, 0x86, SKIP, SKIP, ID, 0)
DEFINST(je_rel32, 0x0f, 0x84, SKIP, SKIP, ID, 0)
DEFINST(jg_rel32, 0x0f, 0x8f, SKIP, SKIP, ID, 0)
DEFINST(jge_rel32, 0x0f, 0x8d, SKIP, SKIP, ID, 0)
DEFINST(jl_rel32, 0x0f, 0x8c, SKIP, SKIP, ID, 0)
DEFINST(jle_rel32, 0x0f, 0x8e, SKIP, SKIP, ID, 0)
DEFINST(jne_rel32, 0x0f, 0x85, SKIP, SKIP, ID, 0)
DEFINST(jno_rel32, 0x0f, 0x81, SKIP, SKIP, ID, 0)
DEFINST(jnp_rel32, 0x0f, 0x8b, SKIP, SKIP, ID, 0)
DEFINST(jns_rel32, 0x0f, 0x89, SKIP, SKIP, ID, 0)
DEFINST(jo_rel32, 0x0f, 0x80, SKIP, SKIP, ID, 0)
DEFINST(jp_rel32, 0x0f, 0x8a, SKIP, SKIP, ID, 0)
DEFINST(js_rel32, 0x0f, 0x88, SKIP, SKIP, ID, 0)

DEFINST(jmp_rel8, 0xeb, SKIP, SKIP, SKIP, IB, 0)
DEFINST(jmp_rel32, 0xe9, SKIP, SKIP, SKIP, ID, 0)
DEFINST(jmp_rm32, 0xff, SKIP, SKIP, 4, SKIP, 0)

DEFINST(ldmxcsr_m32, 0x0f, 0xae, SKIP, 2, SKIP, 0)

DEFINST(lea_r32_m, 0x8d, SKIP, SKIP, MEM|REG, SKIP, 0)

DEFINST(leave, 0xc9, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(lfence, 0x0f, 0xae, SKIP, 5, SKIP, 0)

DEFINST(lodsb, 0xac, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(lodsd, 0xad, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(maxpd_xmm_xmmm128, 0x0f, 0x5f, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(maxps_xmm_xmmm128, 0x0f, 0x5f, SKIP, REG, SKIP, 0)
DEFINST(maxsd_xmm_xmmm64, 0x0f, 0x5f, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(maxss_xmm_xmmm32, 0x0f, 0x5f, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(minpd_xmm_xmmm128, 0x0f, 0x5d, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(minps_xmm_xmmm128, 0x0f, 0x5d, SKIP, REG, SKIP, 0)
DEFINST(minsd_xmm_xmmm64, 0x0f, 0x5d, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(minss_xmm_xmmm32, 0x0f, 0x5d, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(mov_rm8_r8, 0x88, SKIP, SKIP, REG, SKIP, 0)
DEFINST(mov_rm16_r16, 0x89, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(mov_rm32_r32, 0x89, SKIP, SKIP, REG, SKIP, 0)
DEFINST(mov_r8_rm8, 0x8a, SKIP, SKIP, REG, SKIP, 0)
DEFINST(mov_r16_rm16, 0x8b, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(mov_r32_rm32, 0x8b, SKIP, SKIP, REG, SKIP, 0)
DEFINST(mov_al_moffs8, 0xa0, SKIP, SKIP, SKIP, ID, 0)
DEFINST(mov_ax_moffs16, 0xa1, SKIP, SKIP, SKIP, ID, Instruction::PrefixOp)
DEFINST(mov_eax_moffs32, 0xa1, SKIP, SKIP, SKIP, ID, 0)
DEFINST(mov_moffs8_al, 0xa2, SKIP, SKIP, SKIP, ID, 0)
DEFINST(mov_moffs16_ax, 0xa3, SKIP, SKIP, SKIP, ID, Instruction::PrefixOp)
DEFINST(mov_moffs32_eax, 0xa3, SKIP, SKIP, SKIP, ID, 0)
DEFINST(mov_ir8_imm8, 0xb0|INDEX, SKIP, SKIP, SKIP, IB, 0)
DEFINST(mov_ir16_imm16, 0xb8|INDEX, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(mov_ir32_imm32, 0xb8|INDEX, SKIP, SKIP, SKIP, ID, 0)
DEFINST(mov_rm8_imm8, 0xc6, SKIP, SKIP, 0, IB, 0)
DEFINST(mov_rm16_imm16, 0xc7, SKIP, SKIP, 0, IW, Instruction::PrefixOp)
DEFINST(mov_rm32_imm32, 0xc7, SKIP, SKIP, 0, ID, 0)
DEFINST(mov_sreg_rm16, 0x8e, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(mov_sreg_rm32, 0x8e, SKIP, SKIP, REG, SKIP, 0)
DEFINST(mov_rm16_sreg, 0x8c, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(mov_rm32_sreg, 0x8c, SKIP, SKIP, REG, SKIP, 0)

DEFINST(movapd_xmm_xmmm128, 0x0f, 0x28, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movapd_xmmm128_xmm, 0x0f, 0x29, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movaps_xmm_xmmm128, 0x0f, 0x28, SKIP, REG, SKIP, 0)
DEFINST(movaps_xmmm128_xmm, 0x0f, 0x29, SKIP, REG, SKIP, 0)

DEFINST(movd_xmm_rm32, 0x0f, 0x6e, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movd_rm32_xmm, 0x0f, 0x7e, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movdqa_xmm_xmmm128, 0x0f, 0x6f, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movdqa_xmmm128_xmm, 0x0f, 0x7f, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movdqu_xmm_xmmm128, 0x0f, 0x6f, SKIP, REG, SKIP, Instruction::PrefixRep)
DEFINST(movdqu_xmmm128_xmm, 0x0f, 0x7f, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(movhlps_xmm_xmmm128, 0x0f, 0x12, SKIP, REG, SKIP, 0)

DEFINST(movhpd_xmm_m64, 0x0f, 0x16, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movhpd_m64_xmm, 0x0f, 0x17, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movlhps_xmm_xmmm128, 0x0f, 0x16, SKIP, REG, SKIP, 0)

DEFINST(movlps_xmm_m64, 0x0f, 0x12, SKIP, REG, SKIP, 0)
DEFINST(movlpd_xmm_m64, 0x0f, 0x12, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movlps_m64_xmm, 0x0f, 0x13, SKIP, REG, SKIP, 0)
DEFINST(movlpd_m64_xmm, 0x0f, 0x13, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movmskpd_r32_xmmm128, 0x0f, 0x50, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movmskps_r32_xmmm128, 0x0f, 0x50, SKIP, REG, SKIP, 0)

DEFINST(movntdq_m128_xmm, 0x0f, 0xe7, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movq_xmm_xmmm64, 0x0f, 0x7e, SKIP, REG, SKIP, Instruction::PrefixRep)
DEFINST(movq_xmmm64_xmm, 0x0f, 0xd6, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(movsb, 0xa4, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(movsw, 0xa5, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixOp)
DEFINST(movsd, 0xa5, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(movsd_xmm_xmmm64, 0x0f, 0x10, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(movsd_xmmm64_xmm, 0x0f, 0x11, SKIP, REG, SKIP, Instruction::PrefixRepnz)

DEFINST(movss_xmm_xmmm32, 0x0f, 0x10, SKIP, REG, SKIP, Instruction::PrefixRep)
DEFINST(movss_xmmm32_xmm, 0x0f, 0x11, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(movsx_r16_rm8, 0x0f, 0xbe, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movsx_r32_rm8, 0x0f, 0xbe, SKIP, REG, SKIP, 0)
DEFINST(movsx_r32_rm16, 0x0f, 0xbf, SKIP, REG, SKIP, 0)

DEFINST(movups_xmm_xmmm128, 0x0f, 0x10, SKIP, REG, SKIP, 0)
DEFINST(movups_xmmm128_xmm, 0x0f, 0x11, SKIP, REG, SKIP, 0)

DEFINST(movzx_r16_rm8, 0x0f, 0xb6, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(movzx_r32_rm8, 0x0f, 0xb6, SKIP, REG, SKIP, 0)
DEFINST(movzx_r32_rm16, 0x0f, 0xb7, SKIP, REG, SKIP, 0)

DEFINST(mul_rm32, 0xf7, SKIP, SKIP, 4, SKIP, 0)

DEFINST(mulpd_xmm_xmmm128, 0x0f, 0x59, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(mulps_xmm_xmmm128, 0x0f, 0x59, SKIP, REG, SKIP, 0)

DEFINST(mulsd_xmm_xmmm64, 0x0f, 0x59, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(mulss_xmm_xmmm32, 0x0f, 0x59, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(neg_rm8, 0xf6, SKIP, SKIP, 3, SKIP, 0)
DEFINST(neg_rm32, 0xf7, SKIP, SKIP, 3, SKIP, 0)

DEFINST(nop, 0x90, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(nop_rm16, 0x0f, 0x1f, SKIP, 0, SKIP, Instruction::PrefixOp)
DEFINST(nop_rm32, 0x0f, 0x1f, SKIP, 0, SKIP, 0)

DEFINST(not_rm8, 0xf6, SKIP, SKIP, 2, SKIP, 0)
DEFINST(not_rm16, 0xf7, SKIP, SKIP, 2, SKIP, Instruction::PrefixOp)
DEFINST(not_rm32, 0xf7, SKIP, SKIP, 2, SKIP, 0)

DEFINST(or_al_imm8, 0x0c, SKIP, SKIP, SKIP, IB, 0)
DEFINST(or_ax_imm16, 0x0d, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(or_eax_imm32, 0x0d, SKIP, SKIP, SKIP, ID, 0)
DEFINST(or_rm8_imm8, 0x80, SKIP, SKIP, 1, IB, 0)
DEFINST(or_rm16_imm16, 0x81, SKIP, SKIP, 1, IW, Instruction::PrefixOp)
DEFINST(or_rm32_imm32, 0x81, SKIP, SKIP, 1, ID, 0)
DEFINST(or_rm16_imm8, 0x83, SKIP, SKIP, 1, IB, Instruction::PrefixOp)
DEFINST(or_rm32_imm8, 0x83, SKIP, SKIP, 1, IB, 0)
DEFINST(or_rm8_r8, 0x08, SKIP, SKIP, REG, SKIP, 0)
DEFINST(or_rm16_r16, 0x09, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(or_rm32_r32, 0x09, SKIP, SKIP, REG, SKIP, 0)
DEFINST(or_r8_rm8, 0x0a, SKIP, SKIP, REG, SKIP, 0)
DEFINST(or_r16_rm16, 0x0b, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(or_r32_rm32, 0x0b, SKIP, SKIP, REG, SKIP, 0)

DEFINST(orpd_xmm_xmmm128, 0x0f, 0x56, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(orps_xmm_xmmm128, 0x0f, 0x56, SKIP, REG, SKIP, 0)

DEFINST(out_imm8_al, 0xe6, SKIP, SKIP, SKIP, IB, 0)
DEFINST(out_imm8_ax, 0xe7, SKIP, SKIP, SKIP, IB, Instruction::PrefixOp)
DEFINST(out_imm8_eax, 0xe7, SKIP, SKIP, SKIP, IB, 0)
DEFINST(out_dx_al, 0xee, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(out_dx_ax, 0xef, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixOp)
DEFINST(out_dx_eax, 0xef, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(outsb, 0x6e, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(outsd, 0x6f, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(paddb_xmm_xmmm128, 0x0f, 0xfc, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(paddw_xmm_xmmm128, 0x0f, 0xfd, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(paddd_xmm_xmmm128, 0x0f, 0xfe, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(paddq_xmm_xmmm128, 0x0f, 0xd4, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(palignr_xmm_xmmm128_imm8, 0x0f, 0x3a, 0x0f, REG, IB, Instruction::PrefixOp)

DEFINST(pand_xmm_xmmm128, 0x0f, 0xdb, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(pandn_xmm_xmmm128, 0x0f, 0xdf, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(pause, 0x90, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)

DEFINST(pcmpeqb_xmm_xmmm128, 0x0f, 0x74, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(pcmpeqw_xmm_xmmm128, 0x0f, 0x75, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(pcmpeqd_xmm_xmmm128, 0x0f, 0x76, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(pcmpeqq_xmm_xmmm128, 0x0f, 0x38, 0x29, REG, SKIP, Instruction::PrefixOp)

DEFINST(pcmpgtb_xmm_xmmm128, 0x0f, 0x64, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(pcmpgtw_xmm_xmmm128, 0x0f, 0x65, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(pcmpgtd_xmm_xmmm128, 0x0f, 0x66, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(pcmpistri_xmm_xmmm128_imm8, 0x0f, 0x3a, 0x63, REG, IB, Instruction::PrefixOp)

DEFINST(pextrw_r32_xmmm128_imm8, 0x0f, 0xc5, SKIP, REG, IB, Instruction::PrefixOp)

DEFINST(pinsrb_xmm_r32m8_imm8, 0x0f, 0x3a, 0x20, REG, IB, Instruction::PrefixOp)
DEFINST(pinsrd_xmm_rm32_imm8, 0x0f, 0x3a, 0x22, REG, IB, Instruction::PrefixOp)

DEFINST(pinsrw_xmm_r32m16_imm8, 0x0f, 0xc4, SKIP, REG, IB, Instruction::PrefixOp)

DEFINST(pmovmskb_r32_xmmm128, 0x0f, 0xd7, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(pop_rm32, 0x8f, SKIP, SKIP, 0, SKIP, 0)
DEFINST(pop_ir32, 0x58|INDEX, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(popf, 0x9d, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(por_xmm_xmmm128, 0x0f, 0xeb, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(prefetcht0_m8, 0x0f, 0x18, SKIP, MEM|1, SKIP, 0)
DEFINST(prefetcht1, 0x0f, 0x18, SKIP, MEM|2, SKIP, 0)
DEFINST(prefetcht2, 0x0f, 0x18, SKIP, MEM|3, SKIP, 0)
DEFINST(prefetchnta, 0x0f, 0x18, SKIP, MEM|0, SKIP, 0)

DEFINST(pshufb_xmm_xmmm128, 0x0f, 0x38, 0x00, REG, SKIP, Instruction::PrefixOp)

DEFINST(pshufd_xmm_xmmm128_imm8, 0x0f, 0x70, SKIP, REG, IB, Instruction::PrefixOp)

DEFINST(psllw_xmm_xmmm128, 0x0f, 0xf1, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psllw_xmmm128_imm8, 0x0f, 0x71, SKIP, 6, IB, Instruction::PrefixOp)
DEFINST(pslld_xmm_xmmm128, 0x0f, 0xf2, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(pslld_xmmm128_imm8, 0x0f, 0x72, SKIP, 6, IB, Instruction::PrefixOp)
DEFINST(psllq_xmm_xmmm128, 0x0f, 0xf3, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psllq_xmmm128_imm8, 0x0f, 0x73, SKIP, 6, IB, Instruction::PrefixOp)

DEFINST(pslldq_xmmm128_imm8, 0x0f, 0x73, SKIP, 7, IB, Instruction::PrefixOp)

DEFINST(psraw_xmm_xmmm128, 0x0f, 0xe1, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psraw_xmmm128_imm8, 0x0f, 0x71, SKIP, 4, IB, Instruction::PrefixOp)
DEFINST(psrad_xmm_xmmm128, 0x0f, 0xe2, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psrad_xmmm128_imm8, 0x0f, 0x72, SKIP, 4, IB, Instruction::PrefixOp)

DEFINST(psrlw_xmm_xmmm128, 0x0f, 0xd1, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psrlw_xmmm128_imm8, 0x0f, 0x71, SKIP, 2, IB, Instruction::PrefixOp)
DEFINST(psrld_xmm_xmmm128, 0x0f, 0xd2, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psrld_xmmm128_imm8, 0x0f, 0x72, SKIP, 2, IB, Instruction::PrefixOp)
DEFINST(psrlq_xmm_xmmm128, 0x0f, 0xd3, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psrlq_xmmm128_imm8, 0x0f, 0x73, SKIP, 2, IB, Instruction::PrefixOp)

DEFINST(psrldq_xmmm128_imm8, 0x0f, 0x73, SKIP, 3, IB, Instruction::PrefixOp)

DEFINST(psubb_xmm_xmmm128, 0x0f, 0xf8, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psubw_xmm_xmmm128, 0x0f, 0xf9, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psubd_xmm_xmmm128, 0x0f, 0xfa, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(psubq_xmm_xmmm128, 0x0f, 0xfb, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(ptest_xmm_xmmm128, 0x0f, 0x38, 0x17, REG, SKIP, Instruction::PrefixOp)

DEFINST(punpcklbw_xmm_xmmm128, 0x0f, 0x60, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(punpcklwd_xmm_xmmm128, 0x0f, 0x61, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(punpckldq_xmm_xmmm128, 0x0f, 0x62, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(punpcklqdq_xmm_xmmm128, 0x0f, 0x6c, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(push_rm32, 0xff, SKIP, SKIP, 6, SKIP, 0)
DEFINST(push_imm8, 0x6a, SKIP, SKIP, SKIP, IB, 0)
DEFINST(push_imm32, 0x68, SKIP, SKIP, SKIP, ID, 0)
DEFINST(push_ir32, 0x50|INDEX, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(pushf, 0x9c, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(pxor_xmm_xmmm128, 0x0f, 0xef, SKIP, REG, SKIP, Instruction::PrefixOp)

DEFINST(rcl_rm8_1, 0xd0, SKIP, SKIP, 2, SKIP, 0)
DEFINST(rcl_rm8_cl, 0xd2, SKIP, SKIP, 2, SKIP, 0)
DEFINST(rcl_rm8_imm8, 0xc0, SKIP, SKIP, 2, IB, 0)
DEFINST(rcl_rm16_1, 0xd1, SKIP, SKIP, 2, SKIP, Instruction::PrefixOp)
DEFINST(rcl_rm16_cl, 0xd3, SKIP, SKIP, 2, SKIP, Instruction::PrefixOp)
DEFINST(rcl_rm16_imm8, 0xc1, SKIP, SKIP, 2, IB, Instruction::PrefixOp)
DEFINST(rcl_rm32_1, 0xd1, SKIP, SKIP, 2, SKIP, 0)
DEFINST(rcl_rm32_cl, 0xd3, SKIP, SKIP, 2, SKIP, 0)
DEFINST(rcl_rm32_imm8, 0xc1, SKIP, SKIP, 2, IB, 0)
DEFINST(rcr_rm8_1, 0xd0, SKIP, SKIP, 3, SKIP, 0)
DEFINST(rcr_rm8_cl, 0xd2, SKIP, SKIP, 3, SKIP, 0)
DEFINST(rcr_rm8_imm8, 0xc0, SKIP, SKIP, 3, IB, 0)
DEFINST(rcr_rm16_1, 0xd1, SKIP, SKIP, 3, SKIP, Instruction::PrefixOp)
DEFINST(rcr_rm16_cl, 0xd3, SKIP, SKIP, 3, SKIP, Instruction::PrefixOp)
DEFINST(rcr_rm16_imm8, 0xc1, SKIP, SKIP, 3, IB, Instruction::PrefixOp)
DEFINST(rcr_rm32_1, 0xd1, SKIP, SKIP, 3, SKIP, 0)
DEFINST(rcr_rm32_cl, 0xd3, SKIP, SKIP, 3, SKIP, 0)
DEFINST(rcr_rm32_imm8, 0xc1, SKIP, SKIP, 3, IB, 0)
DEFINST(rol_rm8_1, 0xd0, SKIP, SKIP, 0, SKIP, 0)
DEFINST(rol_rm8_cl, 0xd2, SKIP, SKIP, 0, SKIP, 0)
DEFINST(rol_rm8_imm8, 0xc0, SKIP, SKIP, 0, IB, 0)
DEFINST(rol_rm16_1, 0xd1, SKIP, SKIP, 0, SKIP, Instruction::PrefixOp)
DEFINST(rol_rm16_cl, 0xd3, SKIP, SKIP, 0, SKIP, Instruction::PrefixOp)
DEFINST(rol_rm16_imm8, 0xc1, SKIP, SKIP, 0, IB, Instruction::PrefixOp)
DEFINST(rol_rm32_1, 0xd1, SKIP, SKIP, 0, SKIP, 0)
DEFINST(rol_rm32_cl, 0xd3, SKIP, SKIP, 0, SKIP, 0)
DEFINST(rol_rm32_imm8, 0xc1, SKIP, SKIP, 0, IB, 0)
DEFINST(ror_rm8_1, 0xd0, SKIP, SKIP, 1, SKIP, 0)
DEFINST(ror_rm8_cl, 0xd2, SKIP, SKIP, 1, SKIP, 0)
DEFINST(ror_rm8_imm8, 0xc0, SKIP, SKIP, 1, IB, 0)
DEFINST(ror_rm16_1, 0xd1, SKIP, SKIP, 1, SKIP, Instruction::PrefixOp)
DEFINST(ror_rm16_cl, 0xd3, SKIP, SKIP, 1, SKIP, Instruction::PrefixOp)
DEFINST(ror_rm16_imm8, 0xc1, SKIP, SKIP, 1, IB, Instruction::PrefixOp)
DEFINST(ror_rm32_1, 0xd1, SKIP, SKIP, 1, SKIP, 0)
DEFINST(ror_rm32_cl, 0xd3, SKIP, SKIP, 1, SKIP, 0)
DEFINST(ror_rm32_imm8, 0xc1, SKIP, SKIP, 1, IB, 0)

DEFINST(rep_insb, 0x6c, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_insd, 0x6d, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_movsb, 0xa4, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_movsd, 0xa5, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_outsb, 0x6e, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_outsd, 0x6f, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_lodsb, 0xac, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_lodsd, 0xad, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_stosb, 0xaa, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(rep_stosd, 0xab, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRep)
DEFINST(repz_cmpsb, 0xa6, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepz)
DEFINST(repz_cmpsd, 0xa7, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepz)
DEFINST(repz_ret, 0xc3, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepz)
DEFINST(repz_scasb, 0xae, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepz)
DEFINST(repz_scasd, 0xaf, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepz)
DEFINST(repnz_cmpsb, 0xa6, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepnz)
DEFINST(repnz_cmpsd, 0xa7, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepnz)
DEFINST(repnz_scasb, 0xae, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepnz)
DEFINST(repnz_scasd, 0xaf, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixRepnz)

DEFINST(ret, 0xc3, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(ret_imm16, 0xc2, SKIP, SKIP, SKIP, IW, 0)

DEFINST(rdssp_rm32, 0x0f, 0x1e, SKIP, 1, SKIP, Instruction::PrefixRepz)
DEFINST(rdtsc, 0x0f, 0x31, SKIP, SKIP, SKIP, 0)

DEFINST(sahf, 0x9e, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(seta_rm8, 0x0f, 0x97, SKIP, REG, SKIP, 0)
DEFINST(setae_rm8, 0x0f, 0x93, SKIP, REG, SKIP, 0)
DEFINST(setb_rm8, 0x0f, 0x92, SKIP, REG, SKIP, 0)
DEFINST(setbe_rm8, 0x0f, 0x96, SKIP, REG, SKIP, 0)
DEFINST(sete_rm8, 0x0f, 0x94, SKIP, REG, SKIP, 0)
DEFINST(setg_rm8, 0x0f, 0x9f, SKIP, REG, SKIP, 0)
DEFINST(setge_rm8, 0x0f, 0x9d, SKIP, REG, SKIP, 0)
DEFINST(setl_rm8, 0x0f, 0x9c, SKIP, REG, SKIP, 0)
DEFINST(setle_rm8, 0x0f, 0x9e, SKIP, REG, SKIP, 0)
DEFINST(setne_rm8, 0x0f, 0x95, SKIP, REG, SKIP, 0)
DEFINST(setno_rm8, 0x0f, 0x91, SKIP, REG, SKIP, 0)
DEFINST(setnp_rm8, 0x0f, 0x9b, SKIP, REG, SKIP, 0)
DEFINST(setns_rm8, 0x0f, 0x99, SKIP, REG, SKIP, 0)
DEFINST(seto_rm8, 0x0f, 0x90, SKIP, REG, SKIP, 0)
DEFINST(setp_rm8, 0x0f, 0x9a, SKIP, REG, SKIP, 0)
DEFINST(sets_rm8, 0x0f, 0x98, SKIP, REG, SKIP, 0)

DEFINST(sar_rm8_1, 0xd0, SKIP, SKIP, 7, SKIP, 0)
DEFINST(sar_rm8_cl, 0xd2, SKIP, SKIP, 7, SKIP, 0)
DEFINST(sar_rm8_imm8, 0xc0, SKIP, SKIP, 7, IB, 0)
DEFINST(sar_rm16_1, 0xd1, SKIP, SKIP, 7, SKIP, Instruction::PrefixOp)
DEFINST(sar_rm16_cl, 0xd3, SKIP, SKIP, 7, SKIP, Instruction::PrefixOp)
DEFINST(sar_rm16_imm8, 0xc1, SKIP, SKIP, 7, IB, Instruction::PrefixOp)
DEFINST(sar_rm32_1, 0xd1, SKIP, SKIP, 7, SKIP, 0)
DEFINST(sar_rm32_cl, 0xd3, SKIP, SKIP, 7, SKIP, 0)
DEFINST(sar_rm32_imm8, 0xc1, SKIP, SKIP, 7, IB, 0)
DEFINST(shl_rm8_1, 0xd0, SKIP, SKIP, 4, SKIP, 0)
DEFINST(shl_rm8_cl, 0xd2, SKIP, SKIP, 4, SKIP, 0)
DEFINST(shl_rm8_imm8, 0xc0, SKIP, SKIP, 4, IB, 0)
DEFINST(shl_rm16_1, 0xd1, SKIP, SKIP, 4, SKIP, Instruction::PrefixOp)
DEFINST(shl_rm16_cl, 0xd3, SKIP, SKIP, 4, SKIP, Instruction::PrefixOp)
DEFINST(shl_rm16_imm8, 0xc1, SKIP, SKIP, 4, IB, Instruction::PrefixOp)
DEFINST(shl_rm32_1, 0xd1, SKIP, SKIP, 4, SKIP, 0)
DEFINST(shl_rm32_cl, 0xd3, SKIP, SKIP, 4, SKIP, 0)
DEFINST(shl_rm32_imm8, 0xc1, SKIP, SKIP, 4, IB, 0)
DEFINST(shr_rm8_1, 0xd0, SKIP, SKIP, 5, SKIP, 0)
DEFINST(shr_rm8_cl, 0xd2, SKIP, SKIP, 5, SKIP, 0)
DEFINST(shr_rm8_imm8, 0xc0, SKIP, SKIP, 5, IB, 0)
DEFINST(shr_rm16_1, 0xd1, SKIP, SKIP, 5, SKIP, Instruction::PrefixOp)
DEFINST(shr_rm16_cl, 0xd3, SKIP, SKIP, 5, SKIP, Instruction::PrefixOp)
DEFINST(shr_rm16_imm8, 0xc1, SKIP, SKIP, 5, IB, Instruction::PrefixOp)
DEFINST(shr_rm32_1, 0xd1, SKIP, SKIP, 5, SKIP, 0)
DEFINST(shr_rm32_cl, 0xd3, SKIP, SKIP, 5, SKIP, 0)
DEFINST(shr_rm32_imm8, 0xc1, SKIP, SKIP, 5, IB, 0)

DEFINST(sbb_al_imm8, 0x1c, SKIP, SKIP, SKIP, IB, 0)
DEFINST(sbb_ax_imm16, 0x1d, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(sbb_eax_imm32, 0x1d, SKIP, SKIP, SKIP, ID, 0)
DEFINST(sbb_rm8_imm8, 0x80, SKIP, SKIP, 3, IB, 0)
DEFINST(sbb_rm16_imm16, 0x81, SKIP, SKIP, 3, IW, Instruction::PrefixOp)
DEFINST(sbb_rm32_imm32, 0x81, SKIP, SKIP, 3, ID, 0)
DEFINST(sbb_rm16_imm8, 0x83, SKIP, SKIP, 3, IB, Instruction::PrefixOp)
DEFINST(sbb_rm32_imm8, 0x83, SKIP, SKIP, 3, IB, 0)
DEFINST(sbb_rm8_r8, 0x18, SKIP, SKIP, REG, SKIP, 0)
DEFINST(sbb_rm16_r16, 0x19, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(sbb_rm32_r32, 0x19, SKIP, SKIP, REG, SKIP, 0)
DEFINST(sbb_r8_rm8, 0x1a, SKIP, SKIP, REG, SKIP, 0)
DEFINST(sbb_r16_rm16, 0x1b, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(sbb_r32_rm32, 0x1b, SKIP, SKIP, REG, SKIP, 0)

DEFINST(scasb, 0xae, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(scasd, 0xaf, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(sfence, 0x0f, 0xae, SKIP, 7, SKIP, 0)

DEFINST(shld_rm16_r16_imm8, 0x0f, 0xa4, SKIP, REG, IB, Instruction::PrefixOp)
DEFINST(shld_rm16_r16_cl, 0x0f, 0xa5, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(shld_rm32_r32_imm8, 0x0f, 0xa4, SKIP, REG, IB, 0)
DEFINST(shld_rm32_r32_cl, 0x0f, 0xa5, SKIP, REG, SKIP, 0)

DEFINST(shufps_xmm_xmmm128_imm8, 0x0f, 0xc6, SKIP, REG, IB, 0)

DEFINST(shrd_rm32_r32_imm8, 0x0f, 0xac, SKIP, REG, IB, 0)
DEFINST(shrd_rm32_r32_cl, 0x0f, 0xad, SKIP, REG, SKIP, 0)

DEFINST(sqrtpd_xmm_xmmm128, 0x0f, 0x51, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(sqrtps_xmm_xmmm128, 0x0f, 0x51, SKIP, REG, SKIP, 0)

DEFINST(sqrtsd_xmm_xmmm64, 0x0f, 0x51, SKIP, REG, SKIP, Instruction::PrefixRepnz)
DEFINST(sqrtss_xmm_xmmm32, 0x0f, 0x51, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(std, 0xfd, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(stmxcsr_m32, 0x0f, 0xae, SKIP, 3, SKIP, 0)

DEFINST(stosb, 0xaa, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(stosd, 0xab, SKIP, SKIP, SKIP, SKIP, 0)

DEFINST(sub_al_imm8, 0x2c, SKIP, SKIP, SKIP, IB, 0)
DEFINST(sub_ax_imm16, 0x2d, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(sub_eax_imm32, 0x2d, SKIP, SKIP, SKIP, ID, 0)
DEFINST(sub_rm8_imm8, 0x80, SKIP, SKIP, 5, IB, 0)
DEFINST(sub_rm16_imm16, 0x81, SKIP, SKIP, 5, IW, Instruction::PrefixOp)
DEFINST(sub_rm32_imm32, 0x81, SKIP, SKIP, 5, ID, 0)
DEFINST(sub_rm16_imm8, 0x83, SKIP, SKIP, 5, IB, Instruction::PrefixOp)
DEFINST(sub_rm32_imm8, 0x83, SKIP, SKIP, 5, IB, 0)
DEFINST(sub_rm8_r8, 0x28, SKIP, SKIP, REG, SKIP, 0)
DEFINST(sub_rm16_r16, 0x29, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(sub_rm32_r32, 0x29, SKIP, SKIP, REG, SKIP, 0)
DEFINST(sub_r8_rm8, 0x2a, SKIP, SKIP, REG, SKIP, 0)
DEFINST(sub_r16_rm16, 0x2b, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(sub_r32_rm32, 0x2b, SKIP, SKIP, REG, SKIP, 0)

DEFINST(subsd_xmm_xmmm64, 0x0f, 0x5c, SKIP, REG, SKIP, Instruction::PrefixRepnz)

DEFINST(subpd_xmm_xmmm128, 0x0f, 0x5c, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(subps_xmm_xmmm128, 0x0f, 0x5c, SKIP, REG, SKIP, 0)

DEFINST(subss_xmm_xmmm32, 0x0f, 0x5c, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(test_al_imm8, 0xa8, SKIP, SKIP, SKIP, IB, 0)
DEFINST(test_ax_imm16, 0xa9, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(test_eax_imm32, 0xa9, SKIP, SKIP, SKIP, ID, 0)
DEFINST(test_rm8_imm8, 0xf6, SKIP, SKIP, 0, IB, 0)
DEFINST(test_rm16_imm16, 0xf7, SKIP, SKIP, 0, IW, Instruction::PrefixOp)
DEFINST(test_rm32_imm32, 0xf7, SKIP, SKIP, 0, ID, 0)
DEFINST(test_rm8_r8, 0x84, SKIP, SKIP, REG, SKIP, 0)
DEFINST(test_rm16_r16, 0x85, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(test_rm32_r32, 0x85, SKIP, SKIP, REG, SKIP, 0)

DEFINST(tzcnt_r32_rm32, 0x0f, 0xbc, SKIP, REG, SKIP, Instruction::PrefixRep)

DEFINST(ucomisd_xmm_xmmm64, 0x0f, 0x2e, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(ucomiss_xmm_xmmm32, 0x0f, 0x2e, SKIP, REG, SKIP, 0)

DEFINST(unpckhpd_xmm_xmmm128, 0x0f, 0x15, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(unpckhps_xmm_xmmm128, 0x0f, 0x15, SKIP, REG, SKIP, 0)

DEFINST(unpcklpd_xmm_xmmm128, 0x0f, 0x14, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(unpcklps_xmm_xmmm128, 0x0f, 0x14, SKIP, REG, SKIP, 0)

DEFINST(xadd_rm8_r8, 0x0f, 0xc0, SKIP, REG, SKIP, 0)
DEFINST(xadd_rm32_r32, 0x0f, 0xc1, SKIP, REG, SKIP, 0)

DEFINST(xor_al_imm8, 0x34, SKIP, SKIP, SKIP, IB, 0)
DEFINST(xor_ax_imm16, 0x35, SKIP, SKIP, SKIP, IW, Instruction::PrefixOp)
DEFINST(xor_eax_imm32, 0x35, SKIP, SKIP, SKIP, ID, 0)
DEFINST(xor_rm8_imm8, 0x80, SKIP, SKIP, 6, IB, 0)
DEFINST(xor_rm16_imm16, 0x81, SKIP, SKIP, 6, IW, Instruction::PrefixOp)
DEFINST(xor_rm32_imm32, 0x81, SKIP, SKIP, 6, ID, 0)
DEFINST(xor_rm16_imm8, 0x83, SKIP, SKIP, 6, IB, Instruction::PrefixOp)
DEFINST(xor_rm32_imm8, 0x83, SKIP, SKIP, 6, IB, 0)
DEFINST(xor_rm8_r8, 0x30, SKIP, SKIP, REG, SKIP, 0)
DEFINST(xor_rm16_r16, 0x31, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(xor_rm32_r32, 0x31, SKIP, SKIP, REG, SKIP, 0)
DEFINST(xor_r8_rm8, 0x32, SKIP, SKIP, REG, SKIP, 0)
DEFINST(xor_r16_rm16, 0x33, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(xor_r32_rm32, 0x33, SKIP, SKIP, REG, SKIP, 0)

DEFINST(xorpd_xmm_xmmm128, 0x0f, 0x57, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(xorps_xmm_xmmm128, 0x0f, 0x57, SKIP, REG, SKIP, 0)

DEFINST(xchg_ir16_ax, 0x90|INDEX, SKIP, SKIP, SKIP, SKIP, Instruction::PrefixOp)
DEFINST(xchg_ir32_eax, 0x90|INDEX, SKIP, SKIP, SKIP, SKIP, 0)
DEFINST(xchg_rm8_r8, 0x86, SKIP, SKIP, REG, SKIP, 0)
DEFINST(xchg_rm16_r16, 0x87, SKIP, SKIP, REG, SKIP, Instruction::PrefixOp)
DEFINST(xchg_rm32_r32, 0x87, SKIP, SKIP, REG, SKIP, 0)

