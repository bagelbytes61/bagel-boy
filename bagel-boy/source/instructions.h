#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct instruction
{
	const char* syntax;

	uint8_t length;

	uint8_t duration;

	bool update_pc;

	void(*instruction)(struct cpu* cpu, uint16_t);
};

void instruction_00(struct cpu* cpu, uint16_t reserved);	//nop
void instruction_01(struct cpu* cpu, uint16_t d16);			//ld bc, d16
void instruction_02(struct cpu* cpu, uint16_t reserved);	//ld (bc), a
void instruction_03(struct cpu* cpu, uint16_t reserved);	//inc bc
void instruction_04(struct cpu* cpu, uint16_t reserved);	//inc b
void instruction_05(struct cpu* cpu, uint16_t reserved);	//dec b
void instruction_06(struct cpu* cpu, uint16_t d8);			//ld b, d8
void instruction_07(struct cpu* cpu, uint16_t reserved);	//rlca
void instruction_08(struct cpu* cpu, uint16_t a16);			//ld a16, sp
void instruction_09(struct cpu* cpu, uint16_t reserved);	//add hl, bc
void instruction_0a(struct cpu* cpu, uint16_t reserved);	//ld a, (bc)
void instruction_0b(struct cpu* cpu, uint16_t reserved);	//dec bc
void instruction_0c(struct cpu* cpu, uint16_t reserved);	//inc c
void instruction_0d(struct cpu* cpu, uint16_t reserved);	//dec c
void instruction_0e(struct cpu* cpu, uint16_t d8);			//ld c, d8
void instruction_0f(struct cpu* cpu, uint16_t reserved);	//rrca
void instruction_10(struct cpu* cpu, uint16_t reserved);	//stop 0
void instruction_11(struct cpu* cpu, uint16_t d16);			//ld de, d16
void instruction_12(struct cpu* cpu, uint16_t reserved);	//ld (de), a
void instruction_13(struct cpu* cpu, uint16_t reserved);	//inc de
void instruction_14(struct cpu* cpu, uint16_t reserved);	//inc d
void instruction_15(struct cpu* cpu, uint16_t reserved);	//dec d
void instruction_16(struct cpu* cpu, uint16_t d8);			//ld d, d8
void instruction_17(struct cpu* cpu, uint16_t reserved);	//rla
void instruction_18(struct cpu* cpu, uint16_t r8);			//jr r8
void instruction_19(struct cpu* cpu, uint16_t reserved);	//add hl, de
void instruction_1a(struct cpu* cpu, uint16_t reserved);	//ld a, (de)
void instruction_1b(struct cpu* cpu, uint16_t reserved);	//dec de
void instruction_1c(struct cpu* cpu, uint16_t reserved);	//inc e
void instruction_1d(struct cpu* cpu, uint16_t reserved);	//dec e
void instruction_1e(struct cpu* cpu, uint16_t d8);			//ld e, d8
void instruction_1f(struct cpu* cpu, uint16_t reserved);	//rra
void instruction_20(struct cpu* cpu, uint16_t r8);			//jr nz, r8
void instruction_21(struct cpu* cpu, uint16_t d16);			//ld hl, d16
void instruction_22(struct cpu* cpu, uint16_t reserved);	//ld (hl+), a
void instruction_23(struct cpu* cpu, uint16_t reserved);	//inc hl
void instruction_24(struct cpu* cpu, uint16_t reserved);	//inc h
void instruction_25(struct cpu* cpu, uint16_t reserved);	//dec h
void instruction_26(struct cpu* cpu, uint16_t d8);			//ld h, d8
void instruction_27(struct cpu* cpu, uint16_t reserved);	//daa
void instruction_28(struct cpu* cpu, uint16_t r8);			//jr z, r8
void instruction_29(struct cpu* cpu, uint16_t reserved);	//add hl, hl
void instruction_2a(struct cpu* cpu, uint16_t reserved);	//ld a, (hl+)
void instruction_2b(struct cpu* cpu, uint16_t reserved);	//dec hl
void instruction_2c(struct cpu* cpu, uint16_t reserved);	//inc l
void instruction_2d(struct cpu* cpu, uint16_t reserved);	//dec l
void instruction_2e(struct cpu* cpu, uint16_t d8);			//ld l, d8
void instruction_2f(struct cpu* cpu, uint16_t reserved);	//cpl
void instruction_30(struct cpu* cpu, uint16_t r8);			//jr nc, r8
void instruction_31(struct cpu* cpu, uint16_t d16);			//ld sp, d16
void instruction_32(struct cpu* cpu, uint16_t reserved);	//ld (hl-), a
void instruction_33(struct cpu* cpu, uint16_t reserved);	//inc sp
void instruction_34(struct cpu* cpu, uint16_t reserved);	//inc (hl)
void instruction_35(struct cpu* cpu, uint16_t reserved);	//dec (hl)
void instruction_36(struct cpu* cpu, uint16_t d8);			//ld (hl), d8
void instruction_37(struct cpu* cpu, uint16_t reserved);	//scf
void instruction_38(struct cpu* cpu, uint16_t r8);			//jr c, r8
void instruction_39(struct cpu* cpu, uint16_t reserved);	//add hl, sp
void instruction_3a(struct cpu* cpu, uint16_t reserved);	//ld a, (hl-)
void instruction_3b(struct cpu* cpu, uint16_t reserved);	//dec sp
void instruction_3c(struct cpu* cpu, uint16_t reserved);	//inc a
void instruction_3d(struct cpu* cpu, uint16_t reserved);	//dec a
void instruction_3e(struct cpu* cpu, uint16_t d8);			//ld a, d8
void instruction_3f(struct cpu* cpu, uint16_t reserved);	//ccf
void instruction_40(struct cpu* cpu, uint16_t reserved);	//ld b, b
void instruction_41(struct cpu* cpu, uint16_t reserved);	//ld b, c
void instruction_42(struct cpu* cpu, uint16_t reserved);	//ld b, d
void instruction_43(struct cpu* cpu, uint16_t reserved);	//ld b, e
void instruction_44(struct cpu* cpu, uint16_t reserved);	//ld b, h
void instruction_45(struct cpu* cpu, uint16_t reserved);	//ld b, l
void instruction_46(struct cpu* cpu, uint16_t reserved);	//ld b, (hl)
void instruction_47(struct cpu* cpu, uint16_t reserved);	//ld b, a
void instruction_48(struct cpu* cpu, uint16_t reserved);	//ld c, b
void instruction_49(struct cpu* cpu, uint16_t reserved);	//ld c, c
void instruction_4a(struct cpu* cpu, uint16_t reserved);	//ld c, d
void instruction_4b(struct cpu* cpu, uint16_t reserved);	//ld c, e
void instruction_4c(struct cpu* cpu, uint16_t reserved);	//ld c, h
void instruction_4d(struct cpu* cpu, uint16_t reserved);	//ld c, l
void instruction_4e(struct cpu* cpu, uint16_t reserved);	//ld c, (hl)
void instruction_4f(struct cpu* cpu, uint16_t reserved);	//ld c, a
void instruction_50(struct cpu* cpu, uint16_t reserved);	//ld d, b
void instruction_51(struct cpu* cpu, uint16_t reserved);	//ld d, c
void instruction_52(struct cpu* cpu, uint16_t reserved);	//ld d, d
void instruction_53(struct cpu* cpu, uint16_t reserved);	//ld d, e
void instruction_54(struct cpu* cpu, uint16_t reserved);	//ld d, h
void instruction_55(struct cpu* cpu, uint16_t reserved);	//ld d, l
void instruction_56(struct cpu* cpu, uint16_t reserved);	//ld d, (hl)
void instruction_57(struct cpu* cpu, uint16_t reserved);	//ld d, a
void instruction_58(struct cpu* cpu, uint16_t reserved);	//ld e, b
void instruction_59(struct cpu* cpu, uint16_t reserved);	//ld e, c
void instruction_5a(struct cpu* cpu, uint16_t reserved);	//ld e, d
void instruction_5b(struct cpu* cpu, uint16_t reserved);	//ld e, e
void instruction_5c(struct cpu* cpu, uint16_t reserved);	//ld e, h
void instruction_5d(struct cpu* cpu, uint16_t reserved);	//ld e, l
void instruction_5e(struct cpu* cpu, uint16_t reserved);	//ld e, (hl)
void instruction_5f(struct cpu* cpu, uint16_t reserved);	//ld e, a
void instruction_60(struct cpu* cpu, uint16_t reserved);	//ld h, b
void instruction_61(struct cpu* cpu, uint16_t reserved);	//ld h, c
void instruction_62(struct cpu* cpu, uint16_t reserved);	//ld h, d
void instruction_63(struct cpu* cpu, uint16_t reserved);	//ld h, e
void instruction_64(struct cpu* cpu, uint16_t reserved);	//ld h, h
void instruction_65(struct cpu* cpu, uint16_t reserved);	//ld h, l
void instruction_66(struct cpu* cpu, uint16_t reserved);	//ld h, (hl)
void instruction_67(struct cpu* cpu, uint16_t reserved);	//ld h, a
void instruction_68(struct cpu* cpu, uint16_t reserved);	//ld l, b
void instruction_69(struct cpu* cpu, uint16_t reserved);	//ld l, c
void instruction_6a(struct cpu* cpu, uint16_t reserved);	//ld l, d
void instruction_6b(struct cpu* cpu, uint16_t reserved);	//ld l, e
void instruction_6c(struct cpu* cpu, uint16_t reserved);	//ld l, h
void instruction_6d(struct cpu* cpu, uint16_t reserved);	//ld l, l
void instruction_6e(struct cpu* cpu, uint16_t reserved);	//ld l, (hl)
void instruction_6f(struct cpu* cpu, uint16_t reserved);	//ld l, a
void instruction_70(struct cpu* cpu, uint16_t reserved);	//ld (hl), b
void instruction_71(struct cpu* cpu, uint16_t reserved);	//ld (hl), c
void instruction_72(struct cpu* cpu, uint16_t reserved);	//ld (hl), d
void instruction_73(struct cpu* cpu, uint16_t reserved);	//ld (hl), e
void instruction_74(struct cpu* cpu, uint16_t reserved);	//ld (hl), h
void instruction_75(struct cpu* cpu, uint16_t reserved);	//ld (hl) l
void instruction_76(struct cpu* cpu, uint16_t reserved);	//halt
void instruction_77(struct cpu* cpu, uint16_t reserved);	//ld (hl), a
void instruction_78(struct cpu* cpu, uint16_t reserved);	//ld a, b
void instruction_79(struct cpu* cpu, uint16_t reserved);	//ld a, c
void instruction_7a(struct cpu* cpu, uint16_t reserved);	//ld a, d
void instruction_7b(struct cpu* cpu, uint16_t reserved);	//ld a, e
void instruction_7c(struct cpu* cpu, uint16_t reserved);	//ld a, h
void instruction_7d(struct cpu* cpu, uint16_t reserved);	//ld a, l
void instruction_7e(struct cpu* cpu, uint16_t reserved);	//ld a, (hl)
void instruction_7f(struct cpu* cpu, uint16_t reserved);	//ld a, a
void instruction_80(struct cpu* cpu, uint16_t reserved);	//add a, b
void instruction_81(struct cpu* cpu, uint16_t reserved);	//add a, c
void instruction_82(struct cpu* cpu, uint16_t reserved);	//add a, d
void instruction_83(struct cpu* cpu, uint16_t reserved);	//add a, e
void instruction_84(struct cpu* cpu, uint16_t reserved);	//add a, h
void instruction_85(struct cpu* cpu, uint16_t reserved);	//add a, l
void instruction_86(struct cpu* cpu, uint16_t reserved);	//add a, (hl)
void instruction_87(struct cpu* cpu, uint16_t reserved);	//add a, a
void instruction_88(struct cpu* cpu, uint16_t reserved);	//adc a, b
void instruction_89(struct cpu* cpu, uint16_t reserved);	//adc a, c
void instruction_8a(struct cpu* cpu, uint16_t reserved);	//adc a, d
void instruction_8b(struct cpu* cpu, uint16_t reserved);	//adc a, e
void instruction_8c(struct cpu* cpu, uint16_t reserved);	//adc a, h
void instruction_8d(struct cpu* cpu, uint16_t reserved);	//adc a, l
void instruction_8e(struct cpu* cpu, uint16_t reserved);	//adc a, (hl)
void instruction_8f(struct cpu* cpu, uint16_t reserved);	//adc a, a
void instruction_90(struct cpu* cpu, uint16_t reserved);	//sub b, b
void instruction_91(struct cpu* cpu, uint16_t reserved);	//sub b, c
void instruction_92(struct cpu* cpu, uint16_t reserved);	//sub b, d
void instruction_93(struct cpu* cpu, uint16_t reserved);	//sub b, e
void instruction_94(struct cpu* cpu, uint16_t reserved);	//sub b, h
void instruction_95(struct cpu* cpu, uint16_t reserved);	//sub b, l
void instruction_96(struct cpu* cpu, uint16_t reserved);	//sub b, (hl)
void instruction_97(struct cpu* cpu, uint16_t reserved);	//sub b, a
void instruction_98(struct cpu* cpu, uint16_t reserved);	//sbc a, b
void instruction_99(struct cpu* cpu, uint16_t reserved);	//sbc a, c
void instruction_9a(struct cpu* cpu, uint16_t reserved);	//sbc a, d
void instruction_9b(struct cpu* cpu, uint16_t reserved);	//sbc a, e
void instruction_9c(struct cpu* cpu, uint16_t reserved);	//sbc a, h
void instruction_9d(struct cpu* cpu, uint16_t reserved);	//sbc a, l
void instruction_9e(struct cpu* cpu, uint16_t reserved);	//sbc a, (hl)
void instruction_9f(struct cpu* cpu, uint16_t reserved);	//sbs a, a
void instruction_a0(struct cpu* cpu, uint16_t reserved);	//and b
void instruction_a1(struct cpu* cpu, uint16_t reserved);	//and c
void instruction_a2(struct cpu* cpu, uint16_t reserved);	//and d
void instruction_a3(struct cpu* cpu, uint16_t reserved);	//and e
void instruction_a4(struct cpu* cpu, uint16_t reserved);	//and h
void instruction_a5(struct cpu* cpu, uint16_t reserved);	//and l
void instruction_a6(struct cpu* cpu, uint16_t reserved);	//and (hl)
void instruction_a7(struct cpu* cpu, uint16_t reserved);	//and a
void instruction_a8(struct cpu* cpu, uint16_t reserved);	//xor b
void instruction_a9(struct cpu* cpu, uint16_t reserved);	//xor c
void instruction_aa(struct cpu* cpu, uint16_t reserved);	//xor d
void instruction_ab(struct cpu* cpu, uint16_t reserved);	//xor e
void instruction_ac(struct cpu* cpu, uint16_t reserved);	//xor h
void instruction_ad(struct cpu* cpu, uint16_t reserved);	//xor l
void instruction_ae(struct cpu* cpu, uint16_t reserved);	//xor (hl)
void instruction_af(struct cpu* cpu, uint16_t reserved);	//xor a
void instruction_b0(struct cpu* cpu, uint16_t reserved);	//or b
void instruction_b1(struct cpu* cpu, uint16_t reserved);	//or c
void instruction_b2(struct cpu* cpu, uint16_t reserved);	//or d
void instruction_b3(struct cpu* cpu, uint16_t reserved);	//or e
void instruction_b4(struct cpu* cpu, uint16_t reserved);	//or h
void instruction_b5(struct cpu* cpu, uint16_t reserved);	//or l
void instruction_b6(struct cpu* cpu, uint16_t reserved);	//or (hl)
void instruction_b7(struct cpu* cpu, uint16_t reserved);	//or a
void instruction_b8(struct cpu* cpu, uint16_t reserved);	//cp b
void instruction_b9(struct cpu* cpu, uint16_t reserved);	//cp c
void instruction_ba(struct cpu* cpu, uint16_t reserved);	//cp d
void instruction_bb(struct cpu* cpu, uint16_t reserved);	//cp e
void instruction_bc(struct cpu* cpu, uint16_t reserved);	//cp h
void instruction_bd(struct cpu* cpu, uint16_t reserved);	//cp l
void instruction_be(struct cpu* cpu, uint16_t reserved);	//cp (hl)
void instruction_bf(struct cpu* cpu, uint16_t reserved);	//cp a
void instruction_c0(struct cpu* cpu, uint16_t reserved);	//ret nz
void instruction_c1(struct cpu* cpu, uint16_t reserved);	//pop bc
void instruction_c2(struct cpu* cpu, uint16_t a16);			//jp nz, a16
void instruction_c3(struct cpu* cpu, uint16_t a16);			//jp a16
void instruction_c4(struct cpu* cpu, uint16_t a16);			//call nz, a16
void instruction_c5(struct cpu* cpu, uint16_t reserved);	//push bc
void instruction_c6(struct cpu* cpu, uint16_t d8);			//add a, d8
void instruction_c7(struct cpu* cpu, uint16_t reserved);	//rst 00h
void instruction_c8(struct cpu* cpu, uint16_t reserved);	//ret 2
void instruction_c9(struct cpu* cpu, uint16_t reserved);	//ret
void instruction_ca(struct cpu* cpu, uint16_t a16);			//jp z, a16
void instruction_cb(struct cpu* cpu, uint16_t reserved);	//prefix cb
void instruction_cc(struct cpu* cpu, uint16_t a16);			//call z, a16
void instruction_cd(struct cpu* cpu, uint16_t a16);			//call a16
void instruction_ce(struct cpu* cpu, uint16_t d8);			//adc a, d8
void instruction_cf(struct cpu* cpu, uint16_t reserved);	//rst 08h
void instruction_d0(struct cpu* cpu, uint16_t reserved);	//ret nc
void instruction_d1(struct cpu* cpu, uint16_t reserved);	//pop de
void instruction_d2(struct cpu* cpu, uint16_t a16);			//jp nc, a16

void instruction_d4(struct cpu* cpu, uint16_t a16);			//call nc, a16
void instruction_d5(struct cpu* cpu, uint16_t reserved);	//push de
void instruction_d6(struct cpu* cpu, uint16_t d8);			//sub d8
void instruction_d7(struct cpu* cpu, uint16_t reserved);	//rst 10h
void instruction_d8(struct cpu* cpu, uint16_t reserved);	//ret c
void instruction_d9(struct cpu* cpu, uint16_t reserved);	//reti
void instruction_da(struct cpu* cpu, uint16_t a16);			//jp c, a16
void instruction_dc(struct cpu* cpu, uint16_t a16);			//call c, a16
void instruction_de(struct cpu* cpu, uint16_t d8);			//sbc a, d8
void instruction_df(struct cpu* cpu, uint16_t reserved);	//rst 18h
void instruction_e0(struct cpu* cpu, uint16_t a8);			//ldh (a8), a
void instruction_e1(struct cpu* cpu, uint16_t reserved);	//pop hl
void instruction_e2(struct cpu* cpu, uint16_t reserved);	//ld (c), a
void instruction_e5(struct cpu* cpu, uint16_t reserved);	//push hl
void instruction_e6(struct cpu* cpu, uint16_t d8);			//and d8
void instruction_e7(struct cpu* cpu, uint16_t reserved);	//rst 20h
void instruction_e8(struct cpu* cpu, uint16_t r8);			//add sp, r8
void instruction_e9(struct cpu* cpu, uint16_t reserved);	//jp (hl)
void instruction_ea(struct cpu* cpu, uint16_t a16);			//ld (a16), a
void instruction_ee(struct cpu* cpu, uint16_t d8);			//xor d8
void instruction_ef(struct cpu* cpu, uint16_t reserved);	//rst 28h
void instruction_f0(struct cpu* cpu, uint16_t a8);			//lda a, (a8)
void instruction_f1(struct cpu* cpu, uint16_t reserved);	//pop af
void instruction_f2(struct cpu* cpu, uint16_t reserved);	//ld a, (c)
void instruction_f3(struct cpu* cpu, uint16_t reserved);	//di
void instruction_f5(struct cpu* cpu, uint16_t reserved);	//push af
void instruction_f6(struct cpu* cpu, uint16_t reserved);	//or d8
void instruction_f7(struct cpu* cpu, uint16_t reserved);	//rst 30h
void instruction_f8(struct cpu* cpu, uint16_t r8);			//ld hl, sp + r8
void instruction_f9(struct cpu* cpu, uint16_t reserved);	//ld sp, hl
void instruction_fa(struct cpu* cpu, uint16_t a16);			//ld a, (a16)
void instruction_fb(struct cpu* cpu, uint16_t reserved);	//ei
void instruction_fe(struct cpu* cpu, uint16_t d8);			//cp d8
void instruction_ff(struct cpu* cpu, uint16_t reserved);	// rst 38h

void extended_instruction_00(struct cpu* cpu, uint16_t reserved);
void extended_instruction_01(struct cpu* cpu, uint16_t reserved);
void extended_instruction_02(struct cpu* cpu, uint16_t reserved);
void extended_instruction_03(struct cpu* cpu, uint16_t reserved);
void extended_instruction_04(struct cpu* cpu, uint16_t reserved);
void extended_instruction_05(struct cpu* cpu, uint16_t reserved);
void extended_instruction_06(struct cpu* cpu, uint16_t reserved);
void extended_instruction_07(struct cpu* cpu, uint16_t reserved);
void extended_instruction_08(struct cpu* cpu, uint16_t reserved);
void extended_instruction_09(struct cpu* cpu, uint16_t reserved);
void extended_instruction_0a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_0b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_0c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_0d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_0e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_0f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_10(struct cpu* cpu, uint16_t reserved);
void extended_instruction_11(struct cpu* cpu, uint16_t reserved);
void extended_instruction_12(struct cpu* cpu, uint16_t reserved);
void extended_instruction_13(struct cpu* cpu, uint16_t reserved);
void extended_instruction_14(struct cpu* cpu, uint16_t reserved);
void extended_instruction_15(struct cpu* cpu, uint16_t reserved);
void extended_instruction_16(struct cpu* cpu, uint16_t reserved);
void extended_instruction_17(struct cpu* cpu, uint16_t reserved);
void extended_instruction_18(struct cpu* cpu, uint16_t reserved);
void extended_instruction_19(struct cpu* cpu, uint16_t reserved);
void extended_instruction_1a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_1b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_1c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_1d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_1e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_1f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_20(struct cpu* cpu, uint16_t reserved);
void extended_instruction_21(struct cpu* cpu, uint16_t reserved);
void extended_instruction_22(struct cpu* cpu, uint16_t reserved);
void extended_instruction_23(struct cpu* cpu, uint16_t reserved);
void extended_instruction_24(struct cpu* cpu, uint16_t reserved);
void extended_instruction_25(struct cpu* cpu, uint16_t reserved);
void extended_instruction_26(struct cpu* cpu, uint16_t reserved);
void extended_instruction_27(struct cpu* cpu, uint16_t reserved);
void extended_instruction_28(struct cpu* cpu, uint16_t reserved);
void extended_instruction_29(struct cpu* cpu, uint16_t reserved);
void extended_instruction_2a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_2b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_2c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_2d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_2e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_2f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_30(struct cpu* cpu, uint16_t reserved);
void extended_instruction_31(struct cpu* cpu, uint16_t reserved);
void extended_instruction_32(struct cpu* cpu, uint16_t reserved);
void extended_instruction_33(struct cpu* cpu, uint16_t reserved);
void extended_instruction_34(struct cpu* cpu, uint16_t reserved);
void extended_instruction_35(struct cpu* cpu, uint16_t reserved);
void extended_instruction_36(struct cpu* cpu, uint16_t reserved);
void extended_instruction_37(struct cpu* cpu, uint16_t reserved);
void extended_instruction_38(struct cpu* cpu, uint16_t reserved);
void extended_instruction_39(struct cpu* cpu, uint16_t reserved);
void extended_instruction_3a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_3b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_3c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_3d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_3e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_3f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_40(struct cpu* cpu, uint16_t reserved);
void extended_instruction_41(struct cpu* cpu, uint16_t reserved);
void extended_instruction_42(struct cpu* cpu, uint16_t reserved);
void extended_instruction_43(struct cpu* cpu, uint16_t reserved);
void extended_instruction_44(struct cpu* cpu, uint16_t reserved);
void extended_instruction_45(struct cpu* cpu, uint16_t reserved);
void extended_instruction_46(struct cpu* cpu, uint16_t reserved);
void extended_instruction_47(struct cpu* cpu, uint16_t reserved);
void extended_instruction_48(struct cpu* cpu, uint16_t reserved);
void extended_instruction_49(struct cpu* cpu, uint16_t reserved);
void extended_instruction_4a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_4b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_4c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_4d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_4e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_4f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_50(struct cpu* cpu, uint16_t reserved);
void extended_instruction_51(struct cpu* cpu, uint16_t reserved);
void extended_instruction_52(struct cpu* cpu, uint16_t reserved);
void extended_instruction_53(struct cpu* cpu, uint16_t reserved);
void extended_instruction_54(struct cpu* cpu, uint16_t reserved);
void extended_instruction_55(struct cpu* cpu, uint16_t reserved);
void extended_instruction_56(struct cpu* cpu, uint16_t reserved);
void extended_instruction_57(struct cpu* cpu, uint16_t reserved);
void extended_instruction_58(struct cpu* cpu, uint16_t reserved);
void extended_instruction_59(struct cpu* cpu, uint16_t reserved);
void extended_instruction_5a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_5b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_5c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_5d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_5e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_5f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_60(struct cpu* cpu, uint16_t reserved);
void extended_instruction_61(struct cpu* cpu, uint16_t reserved);
void extended_instruction_62(struct cpu* cpu, uint16_t reserved);
void extended_instruction_63(struct cpu* cpu, uint16_t reserved);
void extended_instruction_64(struct cpu* cpu, uint16_t reserved);
void extended_instruction_65(struct cpu* cpu, uint16_t reserved);
void extended_instruction_66(struct cpu* cpu, uint16_t reserved);
void extended_instruction_67(struct cpu* cpu, uint16_t reserved);
void extended_instruction_68(struct cpu* cpu, uint16_t reserved);
void extended_instruction_69(struct cpu* cpu, uint16_t reserved);
void extended_instruction_6a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_6b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_6c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_6d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_6e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_6f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_70(struct cpu* cpu, uint16_t reserved);
void extended_instruction_71(struct cpu* cpu, uint16_t reserved);
void extended_instruction_72(struct cpu* cpu, uint16_t reserved);
void extended_instruction_73(struct cpu* cpu, uint16_t reserved);
void extended_instruction_74(struct cpu* cpu, uint16_t reserved);
void extended_instruction_75(struct cpu* cpu, uint16_t reserved);
void extended_instruction_76(struct cpu* cpu, uint16_t reserved);
void extended_instruction_77(struct cpu* cpu, uint16_t reserved);
void extended_instruction_78(struct cpu* cpu, uint16_t reserved);
void extended_instruction_79(struct cpu* cpu, uint16_t reserved);
void extended_instruction_7a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_7b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_7c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_7d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_7e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_7f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_80(struct cpu* cpu, uint16_t reserved);
void extended_instruction_81(struct cpu* cpu, uint16_t reserved);
void extended_instruction_82(struct cpu* cpu, uint16_t reserved);
void extended_instruction_83(struct cpu* cpu, uint16_t reserved);
void extended_instruction_84(struct cpu* cpu, uint16_t reserved);
void extended_instruction_85(struct cpu* cpu, uint16_t reserved);
void extended_instruction_86(struct cpu* cpu, uint16_t reserved);
void extended_instruction_87(struct cpu* cpu, uint16_t reserved);
void extended_instruction_88(struct cpu* cpu, uint16_t reserved);
void extended_instruction_89(struct cpu* cpu, uint16_t reserved);
void extended_instruction_8a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_8b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_8c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_8d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_8e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_8f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_90(struct cpu* cpu, uint16_t reserved);
void extended_instruction_91(struct cpu* cpu, uint16_t reserved);
void extended_instruction_92(struct cpu* cpu, uint16_t reserved);
void extended_instruction_93(struct cpu* cpu, uint16_t reserved);
void extended_instruction_94(struct cpu* cpu, uint16_t reserved);
void extended_instruction_95(struct cpu* cpu, uint16_t reserved);
void extended_instruction_96(struct cpu* cpu, uint16_t reserved);
void extended_instruction_97(struct cpu* cpu, uint16_t reserved);
void extended_instruction_98(struct cpu* cpu, uint16_t reserved);
void extended_instruction_99(struct cpu* cpu, uint16_t reserved);
void extended_instruction_9a(struct cpu* cpu, uint16_t reserved);
void extended_instruction_9b(struct cpu* cpu, uint16_t reserved);
void extended_instruction_9c(struct cpu* cpu, uint16_t reserved);
void extended_instruction_9d(struct cpu* cpu, uint16_t reserved);
void extended_instruction_9e(struct cpu* cpu, uint16_t reserved);
void extended_instruction_9f(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a0(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a1(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a2(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a3(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a4(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a5(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a6(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a7(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a8(struct cpu* cpu, uint16_t reserved);
void extended_instruction_a9(struct cpu* cpu, uint16_t reserved);
void extended_instruction_aa(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ab(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ac(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ad(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ae(struct cpu* cpu, uint16_t reserved);
void extended_instruction_af(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b0(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b1(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b2(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b3(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b4(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b5(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b6(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b7(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b8(struct cpu* cpu, uint16_t reserved);
void extended_instruction_b9(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ba(struct cpu* cpu, uint16_t reserved);
void extended_instruction_bb(struct cpu* cpu, uint16_t reserved);
void extended_instruction_bc(struct cpu* cpu, uint16_t reserved);
void extended_instruction_bd(struct cpu* cpu, uint16_t reserved);
void extended_instruction_be(struct cpu* cpu, uint16_t reserved);
void extended_instruction_bf(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c0(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c1(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c2(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c3(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c4(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c5(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c6(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c7(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c8(struct cpu* cpu, uint16_t reserved);
void extended_instruction_c9(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ca(struct cpu* cpu, uint16_t reserved);
void extended_instruction_cb(struct cpu* cpu, uint16_t reserved);
void extended_instruction_cc(struct cpu* cpu, uint16_t reserved);
void extended_instruction_cd(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ce(struct cpu* cpu, uint16_t reserved);
void extended_instruction_cf(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d0(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d1(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d2(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d3(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d4(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d5(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d6(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d7(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d8(struct cpu* cpu, uint16_t reserved);
void extended_instruction_d9(struct cpu* cpu, uint16_t reserved);
void extended_instruction_da(struct cpu* cpu, uint16_t reserved);
void extended_instruction_db(struct cpu* cpu, uint16_t reserved);
void extended_instruction_dc(struct cpu* cpu, uint16_t reserved);
void extended_instruction_dd(struct cpu* cpu, uint16_t reserved);
void extended_instruction_de(struct cpu* cpu, uint16_t reserved);
void extended_instruction_df(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e0(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e1(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e2(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e3(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e4(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e5(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e6(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e7(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e8(struct cpu* cpu, uint16_t reserved);
void extended_instruction_e9(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ea(struct cpu* cpu, uint16_t reserved);
void extended_instruction_eb(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ec(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ed(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ee(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ef(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f0(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f1(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f2(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f3(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f5(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f6(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f7(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f8(struct cpu* cpu, uint16_t reserved);
void extended_instruction_f9(struct cpu* cpu, uint16_t reserved);
void extended_instruction_fa(struct cpu* cpu, uint16_t reserved);
void extended_instruction_fb(struct cpu* cpu, uint16_t reserved);
void extended_instruction_fc(struct cpu* cpu, uint16_t reserved);
void extended_instruction_fd(struct cpu* cpu, uint16_t reserved);
void extended_instruction_fe(struct cpu* cpu, uint16_t reserved);
void extended_instruction_ff(struct cpu* cpu, uint16_t reserved);

#endif