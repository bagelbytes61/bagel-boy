#include "instructions.h"

#include "bus.h"
#include "cpu.h"
#include "instructions.h"
#include "instruction_sets.h"
#include "utilities.h"

#include <stdio.h>

static void inc(struct cpu* cpu, uint8_t* n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(*n + 1) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, (*n & 0x0f) == 0x0f);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);

	(*n)++;
}

static void dec(struct cpu* cpu, uint8_t* n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(*n - 1) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_SET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, (*n & 0x0f) > 0x0);

	(*n)--;
}

static void add(struct cpu* cpu, uint8_t n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(cpu->a + n) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, (((cpu->a & 0x0f) + (n & 0x0f)) & 0x10) == 0x10);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     cpu->a + n > 0xff);

	cpu->a += n;
}

static void adc(struct cpu* cpu, uint8_t n)
{
	uint8_t carry_flag = FLAG_STATE(cpu, FLAG_CARRY);

	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(cpu->a + n + carry_flag) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, (cpu->a & 0x0f) + ((n & 0x0f) + carry_flag) > 0x0f);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     cpu->a + n + carry_flag > 0xff);

	cpu->a += n + carry_flag;
}

static void sub(struct cpu* cpu, uint8_t n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(cpu->a - n) == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_SET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, ((cpu->a & 0x0f) - (n & 0x0f)) >= 0x0);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     !(~cpu->a & n));

	cpu->a -= n;
}

static void sbc(struct cpu* cpu, uint8_t n)
{
	uint8_t carry_flag = FLAG_STATE(cpu, FLAG_CARRY);

	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(cpu->a - (n + carry_flag)) == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_SET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, ((cpu->a & 0x0F) - ((n & 0x0F) + carry_flag)) >= 0x0);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     !(~cpu->a & (n + carry_flag)));

	cpu->a -= n + carry_flag;
}

static void and_bb(struct cpu* cpu, uint8_t n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (cpu->a & n) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_SET);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     FLAG_STATE_RESET);

	cpu->a &= n;
}

static void or_bb(struct cpu* cpu, uint8_t n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (cpu->a | n) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     FLAG_STATE_RESET);

	cpu->a |= n;
}

static void xor_bb(struct cpu* cpu, uint8_t n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (cpu->a ^ n) == 0x0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     FLAG_STATE_RESET);

	cpu->a ^= n;
}

static void cp(struct cpu* cpu, uint8_t n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,      (uint8_t)(cpu->a - n) == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_SET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, (cpu->a & 0x0F) - (n & 0x0F) >= 0x0);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     cpu->a < n);
}

static void rlc(struct cpu* cpu, uint8_t* n)
{
	uint8_t old_bit_7 = BIT_SET(*n, 7);

	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(*n, 7));

	*n <<= 1;
	*n |= old_bit_7;

	SET_FLAG_STATE(cpu, FLAG_ZERO, *n == 0);
}

static void rrc(struct cpu* cpu, uint8_t* n)
{
	uint8_t old_bit_0 = BIT_SET(*n, 0);

	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(*n, 0));

	*n >>= 1;
	*n |= old_bit_0 << 7;

	SET_FLAG_STATE(cpu, FLAG_ZERO, *n == 0);
}

static void rl(struct cpu* cpu, uint8_t* n)
{
	uint8_t old_carry_flag = FLAG_STATE(cpu, FLAG_CARRY);

	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(*n, 7));

	*n <<= 1;
	*n |= old_carry_flag;

	SET_FLAG_STATE(cpu, FLAG_ZERO, *n == 0);
}

static void rr(struct cpu* cpu, uint8_t* n)
{
	uint8_t old_carry_flag = FLAG_STATE(cpu, FLAG_CARRY);

	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(*n, 0));

	*n >>= 1;
	*n |= old_carry_flag << 7;

	SET_FLAG_STATE(cpu, FLAG_ZERO, *n == 0);
}

static void sla(struct cpu* cpu, uint8_t* n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO, *n << 1 == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->f, 0));

	*n <<= 1;
}

static void sra(struct cpu* cpu, uint8_t* n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO, *n >> 1 == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->f, 7));

	*n = (*n >> 1) | (*n & 0x80);
}

static void srl(struct cpu* cpu, uint8_t* n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO, *n >> 1 == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->f, 7));

	*n >>= 1;
}

static void swap(struct cpu* cpu, uint8_t* n)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO, (((*n << 4) & 0xF0) | ((*n >> 4) & 0x0F)) == 0);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, FLAG_STATE_RESET);

	*n = (((*n << 4) & 0xF0) | ((*n >> 4) & 0x0F));
}

static void bit(struct cpu* cpu, uint8_t b, uint8_t r)
{
	SET_FLAG_STATE(cpu, FLAG_ZERO,     !BIT_SET(r, b));
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,  FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY,     FLAG_STATE_RESET);
}

static void set(struct cpu* cpu, uint8_t b, uint8_t* r)
{
	*r |= 1 << b;
}

static void res(struct cpu* cpu, uint8_t b, uint8_t* r)
{
	*r &= ~(1 << b);
}


void instruction_00(struct cpu* cpu, uint16_t operand)		//nop
{

}

void instruction_01(struct cpu* cpu, uint16_t d16)			//ld bc, d16
{
	cpu->b = HIGH_BYTE(d16);
	cpu->c = LOW_BYTE(d16);
}

void instruction_02(struct cpu* cpu, uint16_t reserved)		//ld (bc), a
{
	bus_write(cpu->bus, REGISTER_BC(cpu), cpu->a);
}

void instruction_03(struct cpu* cpu, uint16_t reserved)		//inc bc
{
	uint16_t bc = REGISTER_BC(cpu) + 1;

	cpu->b = HIGH_BYTE(bc);
	cpu->c = LOW_BYTE(bc);
}

void instruction_04(struct cpu* cpu, uint16_t reserved)		//inc b
{
	inc(cpu, &cpu->b);
}

void instruction_05(struct cpu* cpu, uint16_t reserved)		//dec b
{
	dec(cpu, &cpu->b);
}

void instruction_06(struct cpu* cpu, uint16_t d8)			//ld b, d8
{
	cpu->b = (uint8_t)d8;
}

void instruction_07(struct cpu* cpu, uint16_t reserved)		//rlca
{
	uint8_t old_bit_7 = BIT_SET(cpu->a, 7);

	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->a, 7));

	cpu->a <<= 1;
	cpu->a |= old_bit_7;

	SET_FLAG_STATE(cpu, FLAG_ZERO, cpu->a == 0);
}

void instruction_08(struct cpu* cpu, uint16_t a16)			//ld a16, sp
{
	bus_write(cpu->bus, a16 + 0, LOW_BYTE(cpu->sp));
	bus_write(cpu->bus, a16 + 1, HIGH_BYTE(cpu->sp));
}

void instruction_09(struct cpu* cpu, uint16_t reserved)		//add hl, bc
{
	uint16_t hl = REGISTER_HL(cpu) + REGISTER_BC(cpu);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_0a(struct cpu* cpu, uint16_t reserved)		//ld a, (bc)
{
	cpu->a = bus_read(cpu->bus, REGISTER_BC(cpu));
}

void instruction_0b(struct cpu* cpu, uint16_t reserved)		//dec bc
{
	uint16_t bc = REGISTER_BC(cpu) - 1;

	cpu->b = HIGH_BYTE(bc);
	cpu->c = LOW_BYTE(bc);
}

void instruction_0c(struct cpu* cpu, uint16_t reserved)		//inc c
{
	inc(cpu, &cpu->c);
}

void instruction_0d(struct cpu* cpu, uint16_t reserved)		//dec c
{
	dec(cpu, &cpu->c);
}

void instruction_0e(struct cpu* cpu, uint16_t d8)			//ld c, d8
{
	cpu->c = (uint8_t)d8;
}

void instruction_0f(struct cpu* cpu, uint16_t reserved)		//rrca
{
	uint8_t old_bit_0 = BIT_SET(cpu->a, 0);

	SET_FLAG_STATE(cpu, FLAG_ZERO, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->a, 0));

	cpu->a >>= 1;
	cpu->a |= old_bit_0 << 7;
}

void instruction_10(struct cpu* cpu, uint16_t reserved)		//stop 0
{

}

void instruction_11(struct cpu* cpu, uint16_t d16)			//ld de, d16
{
	cpu->d = HIGH_BYTE(d16);
	cpu->e = LOW_BYTE(d16);
}

void instruction_12(struct cpu* cpu, uint16_t reserved)		//ld (de), a
{
	bus_write(cpu->bus, REGISTER_DE(cpu), cpu->a);
}

void instruction_13(struct cpu* cpu, uint16_t reserved)		//inc de
{
	uint16_t de = REGISTER_DE(cpu) + 1;

	cpu->d = HIGH_BYTE(de);
	cpu->e = LOW_BYTE(de);
}

void instruction_14(struct cpu* cpu, uint16_t reserved)		//inc d
{
	inc(cpu, &cpu->d);
}

void instruction_15(struct cpu* cpu, uint16_t reserved)		//dec d
{
	dec(cpu, &cpu->d);
}

void instruction_16(struct cpu* cpu, uint16_t d8)			//ld d, d8
{
	cpu->d = (uint8_t)d8;
}

void instruction_17(struct cpu* cpu, uint16_t reserved)		//rla
{
	uint8_t old_carry_flag = FLAG_STATE(cpu, FLAG_CARRY);

	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->a, 7));

	cpu->a <<= 1;
	cpu->a |= old_carry_flag;

	SET_FLAG_STATE(cpu, FLAG_ZERO, cpu->a == 0);
}

void instruction_18(struct cpu* cpu, uint16_t r8)			//jr r8
{
	cpu->pc += (int8_t)r8;
}

void instruction_19(struct cpu* cpu, uint16_t reserved)		//add hl, de
{
	uint16_t hl = REGISTER_HL(cpu) + REGISTER_DE(cpu);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_1a(struct cpu* cpu, uint16_t reserved)		//ld a, (de)
{
	cpu->a = bus_read(cpu->bus, REGISTER_DE(cpu));
}

void instruction_1b(struct cpu* cpu, uint16_t reserved)		//dec de
{
	uint16_t de = REGISTER_DE(cpu) - 1;

	cpu->d = HIGH_BYTE(de);
	cpu->e = LOW_BYTE(de);
}

void instruction_1c(struct cpu* cpu, uint16_t reserved)		//inc e
{
	inc(cpu, &cpu->e);
}

void instruction_1d(struct cpu* cpu, uint16_t reserved)		//dec e
{
	dec(cpu, &cpu->e);
}

void instruction_1e(struct cpu* cpu, uint16_t d8)			//ld e, d8
{
	cpu->e = (uint8_t)d8;
}

void instruction_1f(struct cpu* cpu, uint16_t reserved)		//rra
{
	uint8_t old_carry_flag = FLAG_STATE(cpu, FLAG_CARRY);

	SET_FLAG_STATE(cpu, FLAG_ZERO, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, BIT_SET(cpu->a, 0));

	cpu->a <<= 1;
	cpu->a |= old_carry_flag << 7;
}

#define SET_BREAKPOINT(condition) if(condition) { __debugbreak(); }


void instruction_20(struct cpu* cpu, uint16_t r8)			//jr nz, r8
{
    SET_BREAKPOINT(r8 == 0xFe);

	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_RESET)
	{
		cpu->pc += (int8_t)r8;
	}
}

void instruction_21(struct cpu* cpu, uint16_t d16)			//ld hl, d16
{
	cpu->h = HIGH_BYTE(d16);
	cpu->l = LOW_BYTE(d16);
}

void instruction_22(struct cpu* cpu, uint16_t reserved)		//ld (hl+), a
{
	uint16_t hl = REGISTER_HL(cpu);

	bus_write(cpu->bus, hl++, cpu->a);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_23(struct cpu* cpu, uint16_t reserved)		//inc hl
{
	uint16_t hl = REGISTER_HL(cpu) + 1;

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_24(struct cpu* cpu, uint16_t reserved)		//inc h
{
	inc(cpu, &cpu->h);
}

void instruction_25(struct cpu* cpu, uint16_t reserved)		//dec h
{
	dec(cpu, &cpu->h);
}

void instruction_26(struct cpu* cpu, uint16_t d8)			//ld h, d8
{
	cpu->h = (uint8_t)d8;
}

void instruction_27(struct cpu* cpu, uint16_t reserved)		//daa
{

}

void instruction_28(struct cpu* cpu, uint16_t r8)			//jr z, r8
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_SET)
	{
		cpu->pc += (int8_t)r8;
	}
}

void instruction_29(struct cpu* cpu, uint16_t reserved)		//add hl, hl
{
	uint16_t hl = REGISTER_HL(cpu) + REGISTER_HL(cpu);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_2a(struct cpu* cpu, uint16_t reserved)		//ld a, (hl+)
{
	uint16_t hl = REGISTER_HL(cpu);

	cpu->a = bus_read(cpu->bus, hl++);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_2b(struct cpu* cpu, uint16_t reserved)		//dec hl
{
	uint16_t hl = REGISTER_HL(cpu) - 1;

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_2c(struct cpu* cpu, uint16_t reserved)		//inc l
{
	inc(cpu, &cpu->l);
}

void instruction_2d(struct cpu* cpu, uint16_t reserved)		//dec l
{
	dec(cpu, &cpu->l);
}

void instruction_2e(struct cpu* cpu, uint16_t d8)			//ld l, d8
{
	cpu->l = (uint8_t)d8;
}

void instruction_2f(struct cpu* cpu, uint16_t reserved)		//cpl
{
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,	FLAG_STATE_SET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_SET);

	cpu->a = ~cpu->a;
}

void instruction_30(struct cpu* cpu, uint16_t r8)			//jr nc, r8
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_RESET)
	{
		cpu->pc += (int8_t)r8;
	}
}

void instruction_31(struct cpu* cpu, uint16_t d16)			//ld sp, d16
{
	cpu->sp = d16;
}

void instruction_32(struct cpu* cpu, uint16_t reserved)		//ld (hl-), a
{
	uint16_t hl = REGISTER_HL(cpu);

	bus_write(cpu->bus, hl--, cpu->a);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_33(struct cpu* cpu, uint16_t reserved)		//inc sp
{
	cpu->sp++;
}

void instruction_34(struct cpu* cpu, uint16_t reserved)		//inc (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	hl_address_value++;

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void instruction_35(struct cpu* cpu, uint16_t reserved)		//dec (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	hl_address_value--;

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void instruction_36(struct cpu* cpu, uint16_t d8)			//ld (hl), d8
{
	bus_write(cpu->bus, REGISTER_HL(cpu), (uint8_t)d8);
}

void instruction_37(struct cpu* cpu, uint16_t reserved)		//scf
{
	//RESET_FLAG(cpu, FLAG_SUBTRACT);
	//RESET_FLAG(cpu, FLAG_HALFCARRY);

	//SET_FLAG(cpu, FLAG_CARRY);
}

void instruction_38(struct cpu* cpu, uint16_t r8)			//jr c, r8
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_SET)
	{
		cpu->pc += (int8_t)r8;
	}
}

void instruction_39(struct cpu* cpu, uint16_t reserved)		//add hl, sp
{
	uint16_t hl = REGISTER_HL(cpu) + cpu->sp;

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_3a(struct cpu* cpu, uint16_t reserved)		//ld a, (hl-)
{
	uint16_t hl = REGISTER_HL(cpu);

	cpu->a = bus_read(cpu->bus, hl--);

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);
}

void instruction_3b(struct cpu* cpu, uint16_t reserved)		//dec sp
{
	cpu->sp--;
}

void instruction_3c(struct cpu* cpu, uint16_t reserved)		//inc a
{
	inc(cpu, &cpu->a);
}

void instruction_3d(struct cpu* cpu, uint16_t reserved)		//dec a
{
	dec(cpu, &cpu->a);
}

void instruction_3e(struct cpu* cpu, uint16_t d8)			//ld a, d8
{
	cpu->a = (uint8_t)d8;
}

void instruction_3f(struct cpu* cpu, uint16_t reserved)		//ccf
{
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_CARRY, FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_RESET);
}

void instruction_40(struct cpu* cpu, uint16_t reserved)		//ld b, b
{
	cpu->b = cpu->b;
}

void instruction_41(struct cpu* cpu, uint16_t reserved)		//ld b, c
{
	cpu->b = cpu->c;
}

void instruction_42(struct cpu* cpu, uint16_t reserved)		//ld b, d
{
	cpu->b = cpu->d;
}

void instruction_43(struct cpu* cpu, uint16_t reserved)		//ld b, e
{
	cpu->b = cpu->e;
}

void instruction_44(struct cpu* cpu, uint16_t reserved)		//ld b, h
{
	cpu->b = cpu->h;
}

void instruction_45(struct cpu* cpu, uint16_t reserved)		//ld b, l
{
	cpu->b = cpu->l;
}

void instruction_46(struct cpu* cpu, uint16_t reserved)		//ld b, (hl)
{
	cpu->b = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_47(struct cpu* cpu, uint16_t reserved)		//ld b, a
{
	cpu->b = cpu->a;
}

void instruction_48(struct cpu* cpu, uint16_t reserved)		//ld c, b
{
	cpu->c = cpu->b;
}

void instruction_49(struct cpu* cpu, uint16_t reserved)		//ld c, c
{
	cpu->c = cpu->c;
}

void instruction_4a(struct cpu* cpu, uint16_t reserved)		//ld c, d
{
	cpu->c = cpu->d;
}

void instruction_4b(struct cpu* cpu, uint16_t reserved)		//ld c, e
{
	cpu->c = cpu->e;
}

void instruction_4c(struct cpu* cpu, uint16_t reserved)		//ld c, h
{
	cpu->c = cpu->h;
}

void instruction_4d(struct cpu* cpu, uint16_t reserved)		//ld c, l
{
	cpu->c = cpu->l;
}

void instruction_4e(struct cpu* cpu, uint16_t reserved)		//ld c, (hl)
{
	cpu->c = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_4f(struct cpu* cpu, uint16_t reserved)		//ld c, a
{
	cpu->c = cpu->a;
}

void instruction_50(struct cpu* cpu, uint16_t reserved)		//ld d, b
{
	cpu->d = cpu->b;
}

void instruction_51(struct cpu* cpu, uint16_t reserved)		//ld d, c
{
	cpu->d = cpu->c;
}

void instruction_52(struct cpu* cpu, uint16_t reserved)		//ld d, d
{
	cpu->d = cpu->d;
}

void instruction_53(struct cpu* cpu, uint16_t reserved)		//ld d, e
{
	cpu->d = cpu->e;
}

void instruction_54(struct cpu* cpu, uint16_t reserved)		//ld d, h
{
	cpu->d = cpu->h;
}

void instruction_55(struct cpu* cpu, uint16_t reserved)	//ld d, l
{
	cpu->d = cpu->l;
}

void instruction_56(struct cpu* cpu, uint16_t reserved)	//ld d, (hl)
{
	cpu->d = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_57(struct cpu* cpu, uint16_t reserved)		//ld d, a
{
	cpu->d = cpu->a;
}

void instruction_58(struct cpu* cpu, uint16_t reserved)		//ld e, b
{
	cpu->e = cpu->b;
}

void instruction_59(struct cpu* cpu, uint16_t reserved)		//ld e, c
{
	cpu->e = cpu->c;
}

void instruction_5a(struct cpu* cpu, uint16_t reserved)		//ld e, d
{
	cpu->e = cpu->d;
}

void instruction_5b(struct cpu* cpu, uint16_t reserved)		//ld e, e
{
	cpu->e = cpu->e;
}

void instruction_5c(struct cpu* cpu, uint16_t reserved)		//ld e, h
{
	cpu->e = cpu->h;
}

void instruction_5d(struct cpu* cpu, uint16_t reserved)		//ld e, l
{
	cpu->e = cpu->l;
}

void instruction_5e(struct cpu* cpu, uint16_t reserved)		//ld e, (hl)
{
	cpu->e = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_5f(struct cpu* cpu, uint16_t reserved)		//ld e, a
{
	cpu->e = cpu->a;
}

void instruction_60(struct cpu* cpu, uint16_t reserved)		//ld h, b
{
	cpu->h = cpu->b;
}

void instruction_61(struct cpu* cpu, uint16_t reserved)		//ld h, c
{
	cpu->h = cpu->c;
}

void instruction_62(struct cpu* cpu, uint16_t reserved)		//ld h, d
{
	cpu->h = cpu->d;
}

void instruction_63(struct cpu* cpu, uint16_t reserved)		//ld h, e
{
	cpu->h = cpu->e;
}

void instruction_64(struct cpu* cpu, uint16_t reserved)		//ld h, h
{
	cpu->h = cpu->h;
}

void instruction_65(struct cpu* cpu, uint16_t reserved)		//ld h, l
{
	cpu->h = cpu->l;
}

void instruction_66(struct cpu* cpu, uint16_t reserved)		//ld h, (hl)
{
	cpu->h = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_67(struct cpu* cpu, uint16_t reserved)		//ld h, a
{
	cpu->h = cpu->a;
}

void instruction_68(struct cpu* cpu, uint16_t reserved)		//ld l, b
{
	cpu->l = cpu->b;
}

void instruction_69(struct cpu* cpu, uint16_t reserved)		//ld l, c
{
	cpu->l = cpu->c;
}

void instruction_6a(struct cpu* cpu, uint16_t reserved)		//ld l, d
{
	cpu->l = cpu->d;
}

void instruction_6b(struct cpu* cpu, uint16_t reserved)		//ld l, e
{
	cpu->l = cpu->e;
}

void instruction_6c(struct cpu* cpu, uint16_t reserved)		//ld l, h
{
	cpu->l = cpu->h;
}

void instruction_6d(struct cpu* cpu, uint16_t reserved)		//ld l, l
{
	cpu->l = cpu->l;
}

void instruction_6e(struct cpu* cpu, uint16_t reserved)		//ld l, (hl)
{
	cpu->l = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_6f(struct cpu* cpu, uint16_t reserved)		//ld l, a
{
	cpu->l = cpu->a;
}

void instruction_70(struct cpu* cpu, uint16_t reserved)		//ld (hl), b
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->b);
}

void instruction_71(struct cpu* cpu, uint16_t reserved)		//ld (hl), c
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->c);
}

void instruction_72(struct cpu* cpu, uint16_t reserved)		//ld (hl), d
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->d);
}

void instruction_73(struct cpu* cpu, uint16_t reserved)		//ld (hl), e
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->e);
}

void instruction_74(struct cpu* cpu, uint16_t reserved)		//ld (hl), h
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->h);
}

void instruction_75(struct cpu* cpu, uint16_t reserved)		//ld (hl) l
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->l);
}

void instruction_76(struct cpu* cpu, uint16_t reserved)		//halt
{
	cpu->halted = true;
}

void instruction_77(struct cpu* cpu, uint16_t reserved)		//ld (hl), a
{
	bus_write(cpu->bus, REGISTER_HL(cpu), cpu->a);
}

void instruction_78(struct cpu* cpu, uint16_t reserved)		//ld a, b
{
	cpu->a = cpu->b;
}

void instruction_79(struct cpu* cpu, uint16_t reserved)		//ld a, c
{
	cpu->a = cpu->c;
}

void instruction_7a(struct cpu* cpu, uint16_t reserved)		//ld a, d
{
	cpu->a = cpu->d;
}

void instruction_7b(struct cpu* cpu, uint16_t reserved)		//ld a, e
{
	cpu->a = cpu->e;
}

void instruction_7c(struct cpu* cpu, uint16_t reserved)		//ld a, h
{
	cpu->a = cpu->h;
}

void instruction_7d(struct cpu* cpu, uint16_t reserved)		//ld a, l
{
	cpu->a = cpu->l;
}

void instruction_7e(struct cpu* cpu, uint16_t reserved)		//ld a, (hl)
{
	cpu->a = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_7f(struct cpu* cpu, uint16_t reserved)		//ld a, a
{
	cpu->a = cpu->a;
}

void instruction_80(struct cpu* cpu, uint16_t reserved)		//add a, b
{
	add(cpu, cpu->b);
}

void instruction_81(struct cpu* cpu, uint16_t reserved)		//add a, c
{
	add(cpu, cpu->c);
}

void instruction_82(struct cpu* cpu, uint16_t reserved)		//add a, d
{
	add(cpu, cpu->d);
}

void instruction_83(struct cpu* cpu, uint16_t reserved)		//add a, e
{
	add(cpu, cpu->e);
}

void instruction_84(struct cpu* cpu, uint16_t reserved)		//add a, h
{
	add(cpu, cpu->h);
}

void instruction_85(struct cpu* cpu, uint16_t reserved)		//add a, l
{
	add(cpu, cpu->l);
}

void instruction_86(struct cpu* cpu, uint16_t reserved)		//add a, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	add(cpu, hl_address_value);
}

void instruction_87(struct cpu* cpu, uint16_t reserved)		//add a, a
{
	add(cpu, cpu->a);
}

void instruction_88(struct cpu* cpu, uint16_t reserved)		//adc a, b
{
	adc(cpu, cpu->b);
}

void instruction_89(struct cpu* cpu, uint16_t reserved)		//adc a, c
{
	adc(cpu, cpu->c);
}

void instruction_8a(struct cpu* cpu, uint16_t reserved)		//adc a, d
{
	adc(cpu, cpu->d);
}

void instruction_8b(struct cpu* cpu, uint16_t reserved)		//adc a, e
{
	adc(cpu, cpu->e);
}

void instruction_8c(struct cpu* cpu, uint16_t reserved)		//adc a, h
{
	adc(cpu, cpu->h);
}

void instruction_8d(struct cpu* cpu, uint16_t reserved)		//adc a, l
{
	adc(cpu, cpu->l);
}

void instruction_8e(struct cpu* cpu, uint16_t reserved)		//adc a, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	adc(cpu, hl_address_value);
}

void instruction_8f(struct cpu* cpu, uint16_t reserved)		//adc a, a
{
	adc(cpu, cpu->a);
}

void instruction_90(struct cpu* cpu, uint16_t reserved)		//sub b
{
	sub(cpu, cpu->b);
}

void instruction_91(struct cpu* cpu, uint16_t reserved)		//sub c
{
	sub(cpu, cpu->c);
}

void instruction_92(struct cpu* cpu, uint16_t reserved)		//sub d
{
	sub(cpu, cpu->d);
}

void instruction_93(struct cpu* cpu, uint16_t reserved)		//sub e
{
	sub(cpu, cpu->e);
}

void instruction_94(struct cpu* cpu, uint16_t reserved)		//sub h
{
	sub(cpu, cpu->h);
}

void instruction_95(struct cpu* cpu, uint16_t reserved)		//sub l
{
	sub(cpu, cpu->l);
}

void instruction_96(struct cpu* cpu, uint16_t reserved)		//sub (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	sub(cpu, hl_address_value);
}

void instruction_97(struct cpu* cpu, uint16_t reserved)		//sub a
{
	sub(cpu, cpu->a);
}

void instruction_98(struct cpu* cpu, uint16_t reserved)		//sbc a, b
{
	sbc(cpu, cpu->b);
}

void instruction_99(struct cpu* cpu, uint16_t reserved)		//sbc a, c
{
	sbc(cpu, cpu->c);
}

void instruction_9a(struct cpu* cpu, uint16_t reserved)		//sbc a, d
{
	sbc(cpu, cpu->d);
}

void instruction_9b(struct cpu* cpu, uint16_t reserved)		//sbc a, e
{
	sbc(cpu, cpu->e);
}

void instruction_9c(struct cpu* cpu, uint16_t reserved)		//sbc a, h
{
	sbc(cpu, cpu->h);
}

void instruction_9d(struct cpu* cpu, uint16_t reserved)		//sbc a, l
{
	sbc(cpu, cpu->l);
}

void instruction_9e(struct cpu* cpu, uint16_t reserved)		//sbc a, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	sbc(cpu, hl_address_value);
}

void instruction_9f(struct cpu* cpu, uint16_t reserved)		//sbc a, a
{
	sbc(cpu, cpu->a);
}

void instruction_a0(struct cpu* cpu, uint16_t reserved)		//and b
{
	and_bb(cpu, cpu->b);
}

void instruction_a1(struct cpu* cpu, uint16_t reserved)		//and c
{
	and_bb(cpu, cpu->c);
}

void instruction_a2(struct cpu* cpu, uint16_t reserved)		//and d
{
	and_bb(cpu, cpu->d);
}

void instruction_a3(struct cpu* cpu, uint16_t reserved)		//and e
{
	and_bb(cpu, cpu->e);
}

void instruction_a4(struct cpu* cpu, uint16_t reserved)		//and h
{
	and_bb(cpu, cpu->h);
}

void instruction_a5(struct cpu* cpu, uint16_t reserved)		//and l
{
	and_bb(cpu, cpu->l);
}

void instruction_a6(struct cpu* cpu, uint16_t reserved)		//and (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	and_bb(cpu, hl_address_value);
}

void instruction_a7(struct cpu* cpu, uint16_t reserved)		//and a
{
	and_bb(cpu, cpu->a);
}

void instruction_a8(struct cpu* cpu, uint16_t reserved)		//xor b
{
	xor_bb(cpu, cpu->b);
}

void instruction_a9(struct cpu* cpu, uint16_t reserved)		//xor c
{
	xor_bb(cpu, cpu->c);
}

void instruction_aa(struct cpu* cpu, uint16_t reserved)		//xor d
{
	xor_bb(cpu, cpu->d);
}

void instruction_ab(struct cpu* cpu, uint16_t reserved)		//xor e
{
	xor_bb(cpu, cpu->e);
}

void instruction_ac(struct cpu* cpu, uint16_t reserved)		//xor h
{
	xor_bb(cpu, cpu->c);
}

void instruction_ad(struct cpu* cpu, uint16_t reserved)		//xor l
{
	xor_bb(cpu, cpu->l);
}

void instruction_ae(struct cpu* cpu, uint16_t reserved)		//xor (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	xor_bb(cpu, hl_address_value);
}

void instruction_af(struct cpu* cpu, uint16_t reserved)		//xor a
{
	xor_bb(cpu, cpu->a);
}

void instruction_b0(struct cpu* cpu, uint16_t reserved)		//or b
{
	or_bb(cpu, cpu->b);
}

void instruction_b1(struct cpu* cpu, uint16_t reserved)		//or c
{
	or_bb(cpu, cpu->c);
}

void instruction_b2(struct cpu* cpu, uint16_t reserved)		//or d
{
	or_bb(cpu, cpu->d);
}

void instruction_b3(struct cpu* cpu, uint16_t reserved)		//or e
{
	or_bb(cpu, cpu->e);
}

void instruction_b4(struct cpu* cpu, uint16_t reserved)		//or h
{
	or_bb(cpu, cpu->h);
}

void instruction_b5(struct cpu* cpu, uint16_t reserved)		//or l
{
	or_bb(cpu, cpu->l);
}

void instruction_b6(struct cpu* cpu, uint16_t reserved)		//or (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	or_bb(cpu, hl_address_value);
}

void instruction_b7(struct cpu* cpu, uint16_t reserved)		//or a
{
	or_bb(cpu, cpu->c);
}

void instruction_b8(struct cpu* cpu, uint16_t reserved)		//cp b
{
	cp(cpu, cpu->b);
}

void instruction_b9(struct cpu* cpu, uint16_t reserved)		//cp c
{
	cp(cpu, cpu->c);
}

void instruction_ba(struct cpu* cpu, uint16_t reserved)		//cp d
{
	cp(cpu, cpu->d);
}

void instruction_bb(struct cpu* cpu, uint16_t reserved)		//cp e
{
	cp(cpu, cpu->e);
}

void instruction_bc(struct cpu* cpu, uint16_t reserved)		//cp h
{
	cp(cpu, cpu->h);
}

void instruction_bd(struct cpu* cpu, uint16_t reserved)		//cp l
{
	cp(cpu, cpu->l);
}

void instruction_be(struct cpu* cpu, uint16_t reserved)		//cp (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	//SET_BREAKPOINT(1);
	printf("register hl: %u", hl_address_value);

	cp(cpu, hl_address_value);
}

void instruction_bf(struct cpu* cpu, uint16_t reserved)		//cp a
{
	cp(cpu, cpu->a);
}

void instruction_c0(struct cpu* cpu, uint16_t reserved)		//ret nz
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_RESET)
	{
		cpu->pc = MAKE_SHORT(bus_read(cpu->bus, cpu->sp + 1), bus_read(cpu->bus, cpu->sp + 0));

		cpu->sp += 2;
	}
	else
	{
		cpu->pc += 1;
	}
}

void instruction_c1(struct cpu* cpu, uint16_t reserved)		//pop bc
{
	cpu->b = bus_read(cpu->bus, cpu->sp + 1);
	cpu->c = bus_read(cpu->bus, cpu->sp + 0);

	cpu->sp += 2;
}

void instruction_c2(struct cpu* cpu, uint16_t a16)			//jp nz, a16
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_RESET)
	{
		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_c3(struct cpu* cpu, uint16_t a16)			//jp a16
{
	cpu->pc = a16;
}

void instruction_c4(struct cpu* cpu, uint16_t a16)			//call nz, a16
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_RESET)
	{
		bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc));
		bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc));

		cpu->sp -= 2;

		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_c5(struct cpu* cpu, uint16_t reserved)		//push bc
{
	bus_write(cpu->bus, cpu->sp - 1, cpu->b);
	bus_write(cpu->bus, cpu->sp - 2, cpu->c);

	cpu->sp -= 2;
}

void instruction_c6(struct cpu* cpu, uint16_t d8)			//add a, d8
{
	add(cpu, (uint8_t)d8);
}

void instruction_c7(struct cpu* cpu, uint16_t reserved)		//rst 00h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x00;
}

void instruction_c8(struct cpu* cpu, uint16_t reserved)		//ret z
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_SET)
	{
		cpu->pc = MAKE_SHORT(bus_read(cpu->bus, cpu->sp + 1), bus_read(cpu->bus, cpu->sp + 0));

		cpu->sp += 2;
	}
	else
	{
		cpu->pc += 1;
	}
}

void instruction_c9(struct cpu* cpu, uint16_t reserved)		//ret
{
	cpu->pc = MAKE_SHORT(bus_read(cpu->bus, cpu->sp + 1), bus_read(cpu->bus, cpu->sp + 0));

	cpu->sp += 2;
}

void instruction_ca(struct cpu* cpu, uint16_t a16)			//jp z, a16
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_SET)
	{
		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_cb(struct cpu* cpu, uint16_t reserved)		//prefix cb
{
	const struct instruction* instruction = &extended_instruction_set[bus_read(cpu->bus, cpu->pc + 1)];
	uint16_t operand = 0;

	//printf(instruction->syntax, operand);
	//printf("\n");

	instruction->instruction(cpu, operand);

	cpu->pc += instruction->length;
}

void instruction_cc(struct cpu* cpu, uint16_t a16)			//call z, a16
{
	if (FLAG_STATE(cpu, FLAG_ZERO) == FLAG_STATE_SET)
	{
		bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 3));
		bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 3));

		cpu->sp -= 2;

		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_cd(struct cpu* cpu, uint16_t a16)			//call a16
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 3));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 3));

	cpu->sp -= 2;

	cpu->pc = a16;
}

void instruction_ce(struct cpu* cpu, uint16_t d8)			//adc a, d8
{
	adc(cpu, (uint8_t)d8);
}

void instruction_cf(struct cpu* cpu, uint16_t reserved)		//rst 08h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x08;
}

void instruction_d0(struct cpu* cpu, uint16_t reserved)		//ret nc
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_RESET)
	{
		cpu->pc = MAKE_SHORT(bus_read(cpu->bus, cpu->sp + 1), bus_read(cpu->bus, cpu->sp + 0));

		cpu->sp += 2;
	}
	else
	{
		cpu->pc += 1;
	}
}

void instruction_d1(struct cpu* cpu, uint16_t reserved)		//pop de
{
	cpu->d = bus_read(cpu->bus, cpu->sp + 1);
	cpu->e = bus_read(cpu->bus, cpu->sp + 0);

	cpu->sp += 2;
}

void instruction_d2(struct cpu* cpu, uint16_t a16)			//jp nc, a16
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_RESET)
	{
		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_d4(struct cpu* cpu, uint16_t a16)			//call nc, a16
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_RESET)
	{
		bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 3));
		bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 3));

		cpu->sp -= 2;

		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_d5(struct cpu* cpu, uint16_t reserved)		//push de
{
	bus_write(cpu->bus, cpu->sp - 1, cpu->d);
	bus_write(cpu->bus, cpu->sp - 2, cpu->e);

	cpu->sp -= 2;
}

void instruction_d6(struct cpu* cpu, uint16_t d8)			//sub d8
{
	sub(cpu, (uint8_t)d8);
}

void instruction_d7(struct cpu* cpu, uint16_t reserved)		//rst 10h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x10;
}

void instruction_d8(struct cpu* cpu, uint16_t reserved)		//ret c
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_SET)
	{
		cpu->pc = MAKE_SHORT(bus_read(cpu->bus, cpu->sp + 1), bus_read(cpu->bus, cpu->sp + 0));

		cpu->sp += 2;
	}
	else
	{
		cpu->pc += 1;
	}
}

void instruction_d9(struct cpu* cpu, uint16_t reserved)		//reti
{
	cpu->pc = MAKE_SHORT(bus_read(cpu->bus, cpu->sp + 1), bus_read(cpu->bus, cpu->sp + 0));

	cpu->sp += 2;

	//cpu->ie = 0x01;
}

void instruction_da(struct cpu* cpu, uint16_t a16)			//jp c, a16
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_SET)
	{
		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_dc(struct cpu* cpu, uint16_t a16)			//call c, a16
{
	if (FLAG_STATE(cpu, FLAG_CARRY) == FLAG_STATE_SET)
	{
		bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 3));
		bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 3));

		cpu->sp -= 2;

		cpu->pc = a16;
	}
	else
	{
		cpu->pc += 3;
	}
}

void instruction_de(struct cpu* cpu, uint16_t d8)			//sbc a, d8
{
	sbc(cpu, (uint8_t)d8);
}

void instruction_df(struct cpu* cpu, uint16_t reserved)		//rst 18h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x18;
}

void instruction_e0(struct cpu* cpu, uint16_t a8)			//ldh (a8), a
{
	bus_write(cpu->bus, 0xFF00 + (uint8_t)a8, cpu->a);
}

void instruction_e1(struct cpu* cpu, uint16_t reserved)		//pop hl
{
	cpu->h = bus_read(cpu->bus, cpu->sp + 1);
	cpu->l = bus_read(cpu->bus, cpu->sp + 0);

	cpu->sp += 2;
}

void instruction_e2(struct cpu* cpu, uint16_t reserved)		//ld (c), a
{
	bus_write(cpu->bus, 0xFF00 + cpu->c, cpu->a);
}

void instruction_e5(struct cpu* cpu, uint16_t reserved)		//push hl
{
	bus_write(cpu->bus, cpu->sp - 1, cpu->h);
	bus_write(cpu->bus, cpu->sp - 2, cpu->l);

	cpu->sp -= 2;
}

void instruction_e6(struct cpu* cpu, uint16_t d8)			//and d8
{
	and_bb(cpu, (uint8_t)d8);
}

void instruction_e7(struct cpu* cpu, uint16_t reserved)		//rst 20h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x20;
}

void instruction_e8(struct cpu* cpu, uint16_t r8)			//add sp, r8
{
	cpu->sp += r8;

	SET_FLAG_STATE(cpu, FLAG_ZERO,		FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,	FLAG_STATE_RESET);
}

void instruction_e9(struct cpu* cpu, uint16_t reserved)		//jp (hl)
{
	cpu->pc = bus_read(cpu->bus, REGISTER_HL(cpu));
}

void instruction_ea(struct cpu* cpu, uint16_t a16)			//ld (a16), a
{
	bus_write(cpu->bus, a16, cpu->a);
}

void instruction_ee(struct cpu* cpu, uint16_t d8)			//xor d8
{
	xor_bb(cpu, (uint8_t)d8);
}

void instruction_ef(struct cpu* cpu, uint16_t reserved)		//rst 28h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x28;
}

void instruction_f0(struct cpu* cpu, uint16_t a8)			//ldh a, (a8)
{
	cpu->a = bus_read(cpu->bus, 0xFF00 + a8);
}

void instruction_f1(struct cpu* cpu, uint16_t reserved)		//pop af
{
	cpu->a = bus_read(cpu->bus, cpu->sp - 1);
	cpu->f = bus_read(cpu->bus, cpu->sp - 0);

	cpu->sp += 2;
}

void instruction_f2(struct cpu* cpu, uint16_t reserved)		//ld a, (c)
{
	cpu->a = bus_read(cpu->bus, cpu->c);
}

void instruction_f3(struct cpu* cpu, uint16_t reserved)		//di
{
	//cpu->ime = 0x0;
}

void instruction_f5(struct cpu* cpu, uint16_t reserved)		//push af
{
	bus_write(cpu->bus, cpu->sp - 1, cpu->a);
	bus_write(cpu->bus, cpu->sp - 2, cpu->f);

	cpu->sp -= 2;
}

void instruction_f6(struct cpu* cpu, uint16_t d8)			//or d8
{
	or_bb(cpu, (uint8_t)d8);
}

void instruction_f7(struct cpu* cpu, uint16_t reserved)		//rst 30h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x30;
}

void instruction_f8(struct cpu* cpu, uint16_t r8)			//ld hl, sp + r8
{
	uint16_t hl = REGISTER_HL(cpu) + (int8_t)r8;

	cpu->h = HIGH_BYTE(hl);
	cpu->l = LOW_BYTE(hl);

	SET_FLAG_STATE(cpu, FLAG_ZERO,		FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_SUBTRACT,	FLAG_STATE_RESET);
	SET_FLAG_STATE(cpu, FLAG_HALFCARRY, (cpu->sp & 0xF) + (int8_t)r8 > 0xF);
	SET_FLAG_STATE(cpu, FLAG_CARRY,		(cpu->sp + (int8_t)r8) & 0xFFFF);
}

void instruction_f9(struct cpu* cpu, uint16_t reserved)		//ld sp, hl
{
	cpu->sp = REGISTER_HL(cpu);
}

void instruction_fa(struct cpu* cpu, uint16_t a16)			//ld a, (a16)
{
	cpu->a = bus_read(cpu->bus, a16);
}

void instruction_fb(struct cpu* cpu, uint16_t reserved)	//ei
{
	//cpu->ime = 0x01;
}

void instruction_fe(struct cpu* cpu, uint16_t d8)			//cp d8
{
	cp(cpu, (uint8_t)d8);
}

void instruction_ff(struct cpu* cpu, uint16_t reserved)		//rst 38h
{
	bus_write(cpu->bus, cpu->sp - 1, HIGH_BYTE(cpu->pc + 1));
	bus_write(cpu->bus, cpu->sp - 2, LOW_BYTE(cpu->pc + 1));

	cpu->sp -= 2;

	cpu->pc = 0x38;
}

void extended_instruction_00(struct cpu* cpu, uint16_t reserved)				//rlc b
{
	rlc(cpu, &cpu->b);
}

void extended_instruction_01(struct cpu* cpu, uint16_t reserved)				//rlc c
{
	rlc(cpu, &cpu->c);
}

void extended_instruction_02(struct cpu* cpu, uint16_t reserved)				//rlc d
{
	rlc(cpu, &cpu->d);
}

void extended_instruction_03(struct cpu* cpu, uint16_t reserved)				//rlc e
{
	rlc(cpu, &cpu->e);
}

void extended_instruction_04(struct cpu* cpu, uint16_t reserved)				//rlc h
{
	rlc(cpu, &cpu->h);
}

void extended_instruction_05(struct cpu* cpu, uint16_t reserved)				//rlc l
{
	rlc(cpu, &cpu->l);
}

void extended_instruction_06(struct cpu* cpu, uint16_t reserved)				//rlc (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	rlc(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_07(struct cpu* cpu, uint16_t reserved)				//rlc a
{
	rlc(cpu, &cpu->a);
}

void extended_instruction_08(struct cpu* cpu, uint16_t reserved)				//rrc b
{
	rrc(cpu, &cpu->b);
}

void extended_instruction_09(struct cpu* cpu, uint16_t reserved)				//rrc c
{
	rrc(cpu, &cpu->c);
}

void extended_instruction_0a(struct cpu* cpu, uint16_t reserved)				//rrc d
{
	rrc(cpu, &cpu->d);
}

void extended_instruction_0b(struct cpu* cpu, uint16_t reserved)				//rrc e
{
	rrc(cpu, &cpu->e);
}

void extended_instruction_0c(struct cpu* cpu, uint16_t reserved)				//rrc h
{
	rrc(cpu, &cpu->h);
}

void extended_instruction_0d(struct cpu* cpu, uint16_t reserved)				//rrc l
{
	rrc(cpu, &cpu->l);
}

void extended_instruction_0e(struct cpu* cpu, uint16_t reserved)				//rrc (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	rrc(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_0f(struct cpu* cpu, uint16_t reserved)				//rrc a
{
	rrc(cpu, &cpu->a);
}

void extended_instruction_10(struct cpu* cpu, uint16_t reserved)				//rl b
{
	rl(cpu, &cpu->b);
}

void extended_instruction_11(struct cpu* cpu, uint16_t reserved)				//rl c
{
	rl(cpu, &cpu->c);
}

void extended_instruction_12(struct cpu* cpu, uint16_t reserved)				//rl d
{
	rl(cpu, &cpu->d);
}

void extended_instruction_13(struct cpu* cpu, uint16_t reserved)				//rl e
{
	rl(cpu, &cpu->e);
}

void extended_instruction_14(struct cpu* cpu, uint16_t reserved)				//rl h
{
	rl(cpu, &cpu->h);
}

void extended_instruction_15(struct cpu* cpu, uint16_t reserved)				//rl l
{
	rl(cpu, &cpu->l);
}

void extended_instruction_16(struct cpu* cpu, uint16_t reserved)				//rl (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	rl(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_17(struct cpu* cpu, uint16_t reserved)				//rl a
{
	rl(cpu, &cpu->a);
}

void extended_instruction_18(struct cpu* cpu, uint16_t reserved)				//rr b
{
	rr(cpu, &cpu->b);
}

void extended_instruction_19(struct cpu* cpu, uint16_t reserved)				//rr c
{
	rr(cpu, &cpu->c);
}

void extended_instruction_1a(struct cpu* cpu, uint16_t reserved)				//rr d
{
	rr(cpu, &cpu->d);
}

void extended_instruction_1b(struct cpu* cpu, uint16_t reserved)				//rr e
{
	rr(cpu, &cpu->e);
}

void extended_instruction_1c(struct cpu* cpu, uint16_t reserved)				//rr h
{
	rr(cpu, &cpu->h);
}

void extended_instruction_1d(struct cpu* cpu, uint16_t reserved)				//rr l
{
	rr(cpu, &cpu->l);
}

void extended_instruction_1e(struct cpu* cpu, uint16_t reserved)				//rr (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	rr(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_1f(struct cpu* cpu, uint16_t reserved)				//rr a
{
	rr(cpu, &cpu->a);
}

void extended_instruction_20(struct cpu* cpu, uint16_t reserved)				//sla b
{
	sla(cpu, &cpu->b);
}

void extended_instruction_21(struct cpu* cpu, uint16_t reserved)				//sla c
{
	sla(cpu, &cpu->c);
}

void extended_instruction_22(struct cpu* cpu, uint16_t reserved)				//sla d
{
	sla(cpu, &cpu->d);
}

void extended_instruction_23(struct cpu* cpu, uint16_t reserved)				//sla e
{
	sla(cpu, &cpu->e);
}

void extended_instruction_24(struct cpu* cpu, uint16_t reserved)				//sla h
{
	sla(cpu, &cpu->h);
}

void extended_instruction_25(struct cpu* cpu, uint16_t reserved)				//sla l
{
	sla(cpu, &cpu->l);
}

void extended_instruction_26(struct cpu* cpu, uint16_t reserved)				//sla (hl)
{

}

void extended_instruction_27(struct cpu* cpu, uint16_t reserved)				//sla a
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	sla(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_28(struct cpu* cpu, uint16_t reserved)				//sra b
{
	sra(cpu, &cpu->b);
}

void extended_instruction_29(struct cpu* cpu, uint16_t reserved)				//sra c
{
	sra(cpu, &cpu->c);
}

void extended_instruction_2a(struct cpu* cpu, uint16_t reserved)				//sra d
{
	sra(cpu, &cpu->d);
}

void extended_instruction_2b(struct cpu* cpu, uint16_t reserved)				//sra e
{
	sra(cpu, &cpu->e);
}

void extended_instruction_2c(struct cpu* cpu, uint16_t reserved)				//sra h
{
	sra(cpu, &cpu->h);
}

void extended_instruction_2d(struct cpu* cpu, uint16_t reserved)				//sra l
{
	sra(cpu, &cpu->l);
}

void extended_instruction_2e(struct cpu* cpu, uint16_t reserved)				//sra (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	sra(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_2f(struct cpu* cpu, uint16_t reserved)				//sra a
{
	sra(cpu, &cpu->a);
}

void extended_instruction_30(struct cpu* cpu, uint16_t reserved)				//swap b
{
	swap(cpu, &cpu->b);
}

void extended_instruction_31(struct cpu* cpu, uint16_t reserved)				//swap c
{
	swap(cpu, &cpu->c);
}

void extended_instruction_32(struct cpu* cpu, uint16_t reserved)				//swap d
{
	swap(cpu, &cpu->d);
}

void extended_instruction_33(struct cpu* cpu, uint16_t reserved)				//swap e
{
	swap(cpu, &cpu->e);
}

void extended_instruction_34(struct cpu* cpu, uint16_t reserved)				//swap h
{
	swap(cpu, &cpu->h);
}

void extended_instruction_35(struct cpu* cpu, uint16_t reserved)				//swap l
{
	swap(cpu, &cpu->l);
}

void extended_instruction_36(struct cpu* cpu, uint16_t reserved)				//swap (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	swap(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_37(struct cpu* cpu, uint16_t reserved)				//swap a
{
	swap(cpu, &cpu->a);
}

void extended_instruction_38(struct cpu* cpu, uint16_t reserved)				//srl b
{

}

void extended_instruction_39(struct cpu* cpu, uint16_t reserved)				//srl c
{

}

void extended_instruction_3a(struct cpu* cpu, uint16_t reserved)				//srl d
{

}

void extended_instruction_3b(struct cpu* cpu, uint16_t reserved)				//srl e
{
	srl(cpu, &cpu->e);
}

void extended_instruction_3c(struct cpu* cpu, uint16_t reserved)				//srl h
{
	srl(cpu, &cpu->h);
}

void extended_instruction_3d(struct cpu* cpu, uint16_t reserved)				//srl l
{
	srl(cpu, &cpu->l);
}

void extended_instruction_3e(struct cpu* cpu, uint16_t reserved)				//srl (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	srl(cpu, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_3f(struct cpu* cpu, uint16_t reserved)				//srl a
{
	srl(cpu, &cpu->a);
}

void extended_instruction_40(struct cpu* cpu, uint16_t reserved)				//bit 0, b
{
	bit(cpu, 0, cpu->b);
}

void extended_instruction_41(struct cpu* cpu, uint16_t reserved)				//bit 0, c
{
	bit(cpu, 0, cpu->c);
}

void extended_instruction_42(struct cpu* cpu, uint16_t reserved)				//bit 0, d
{
	bit(cpu, 0, cpu->d);
}

void extended_instruction_43(struct cpu* cpu, uint16_t reserved)				//bit 0, e
{
	bit(cpu, 0, cpu->e);
}

void extended_instruction_44(struct cpu* cpu, uint16_t reserved)				//bit 0, h
{
	bit(cpu, 0, cpu->h);
}

void extended_instruction_45(struct cpu* cpu, uint16_t reserved)				//bit 0, l
{
	bit(cpu, 0, cpu->l);
}

void extended_instruction_46(struct cpu* cpu, uint16_t reserved)				//bit 0, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 0, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_47(struct cpu* cpu, uint16_t reserved)				//bit 0, a   
{
	bit(cpu, 0, cpu->a);
}

void extended_instruction_48(struct cpu* cpu, uint16_t reserved)				//bit 1, b
{
	bit(cpu, 1, cpu->b);
}

void extended_instruction_49(struct cpu* cpu, uint16_t reserved)				//bit 1, c
{
	bit(cpu, 1, cpu->c);
}

void extended_instruction_4a(struct cpu* cpu, uint16_t reserved)				//bit 1, d
{
	bit(cpu, 1, cpu->d);
}

void extended_instruction_4b(struct cpu* cpu, uint16_t reserved)				//bit 1, e
{
	bit(cpu, 1, cpu->e);
}

void extended_instruction_4c(struct cpu* cpu, uint16_t reserved)				//bit 1, h
{
	bit(cpu, 1, cpu->h);
}

void extended_instruction_4d(struct cpu* cpu, uint16_t reserved)				//bit 1, l
{
	bit(cpu, 1, cpu->l);
}

void extended_instruction_4e(struct cpu* cpu, uint16_t reserved)				//bit 1, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 1, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_4f(struct cpu* cpu, uint16_t reserved)				//bit 1, a   
{
	bit(cpu, 1, cpu->a);
}

void extended_instruction_50(struct cpu* cpu, uint16_t reserved)				//bit 2, b
{
	bit(cpu, 2, cpu->b);
}

void extended_instruction_51(struct cpu* cpu, uint16_t reserved)				//bit 2, c
{
	bit(cpu, 2, cpu->c);
}

void extended_instruction_52(struct cpu* cpu, uint16_t reserved)				//bit 2, d
{
	bit(cpu, 2, cpu->d);
}

void extended_instruction_53(struct cpu* cpu, uint16_t reserved)				//bit 2, e
{
	bit(cpu, 2, cpu->e);
}

void extended_instruction_54(struct cpu* cpu, uint16_t reserved)				//bit 2, h
{
	bit(cpu, 2, cpu->h);
}

void extended_instruction_55(struct cpu* cpu, uint16_t reserved)				//bit 2, l
{
	bit(cpu, 2, cpu->l);
}

void extended_instruction_56(struct cpu* cpu, uint16_t reserved)				//bit 2, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 2, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_57(struct cpu* cpu, uint16_t reserved)				//bit 2, a   
{
	bit(cpu, 2, cpu->a);
}

void extended_instruction_58(struct cpu* cpu, uint16_t reserved)				//bit 3, b
{
	bit(cpu, 3, cpu->b);
}

void extended_instruction_59(struct cpu* cpu, uint16_t reserved)				//bit 3, c
{
	bit(cpu, 3, cpu->c);
}

void extended_instruction_5a(struct cpu* cpu, uint16_t reserved)				//bit 3, d
{
	bit(cpu, 3, cpu->d);
}

void extended_instruction_5b(struct cpu* cpu, uint16_t reserved)				//bit 3, e
{
	bit(cpu, 3, cpu->e);
}

void extended_instruction_5c(struct cpu* cpu, uint16_t reserved)				//bit 3, h
{
	bit(cpu, 3, cpu->h);
}

void extended_instruction_5d(struct cpu* cpu, uint16_t reserved)				//bit 3, l
{
	bit(cpu, 3, cpu->l);
}

void extended_instruction_5e(struct cpu* cpu, uint16_t reserved)				//bit 3, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 3, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_5f(struct cpu* cpu, uint16_t reserved)				//bit 3, a   
{
	bit(cpu, 3, cpu->a);
}

void extended_instruction_60(struct cpu* cpu, uint16_t reserved)				//bit 4, b
{
	bit(cpu, 4, cpu->b);
}

void extended_instruction_61(struct cpu* cpu, uint16_t reserved)				//bit 4, c
{
	bit(cpu, 4, cpu->c);
}

void extended_instruction_62(struct cpu* cpu, uint16_t reserved)				//bit 4, d
{
	bit(cpu, 4, cpu->d);
}

void extended_instruction_63(struct cpu* cpu, uint16_t reserved)				//bit 4, e
{
	bit(cpu, 4, cpu->e);
}

void extended_instruction_64(struct cpu* cpu, uint16_t reserved)				//bit 4, h
{
	bit(cpu, 4, cpu->h);
}

void extended_instruction_65(struct cpu* cpu, uint16_t reserved)				//bit 4, l
{
	bit(cpu, 4, cpu->l);
}

void extended_instruction_66(struct cpu* cpu, uint16_t reserved)				//bit 4, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 4, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_67(struct cpu* cpu, uint16_t reserved)				//bit 4, a   
{
	bit(cpu, 4, cpu->a);
}

void extended_instruction_68(struct cpu* cpu, uint16_t reserved)				//bit 5, b
{
	bit(cpu, 5, cpu->b);
}

void extended_instruction_69(struct cpu* cpu, uint16_t reserved)				//bit 5, c
{
	bit(cpu, 5, cpu->c);
}

void extended_instruction_6a(struct cpu* cpu, uint16_t reserved)				//bit 5, d
{
	bit(cpu, 5, cpu->d);
}

void extended_instruction_6b(struct cpu* cpu, uint16_t reserved)				//bit 5, e
{
	bit(cpu, 5, cpu->e);
}

void extended_instruction_6c(struct cpu* cpu, uint16_t reserved)				//bit 5, h
{
	bit(cpu, 5, cpu->h);
}

void extended_instruction_6d(struct cpu* cpu, uint16_t reserved)				//bit 5, l
{
	bit(cpu, 5, cpu->l);
}

void extended_instruction_6e(struct cpu* cpu, uint16_t reserved)				//bit 5, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 5, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_6f(struct cpu* cpu, uint16_t reserved)				//bit 5, a   
{
	bit(cpu, 5, cpu->a);
}

void extended_instruction_70(struct cpu* cpu, uint16_t reserved)				//bit 6, b
{
	bit(cpu, 6, cpu->b);
}

void extended_instruction_71(struct cpu* cpu, uint16_t reserved)				//bit 6, c
{
	bit(cpu, 6, cpu->c);
}

void extended_instruction_72(struct cpu* cpu, uint16_t reserved)				//bit 6, d
{
	bit(cpu, 6, cpu->d);
}

void extended_instruction_73(struct cpu* cpu, uint16_t reserved)				//bit 6, e
{
	bit(cpu, 6, cpu->e);
}

void extended_instruction_74(struct cpu* cpu, uint16_t reserved)				//bit 6, h
{
	bit(cpu, 6, cpu->h);
}

void extended_instruction_75(struct cpu* cpu, uint16_t reserved)				//bit 6, l
{
	bit(cpu, 6, cpu->l);
}

void extended_instruction_76(struct cpu* cpu, uint16_t reserved)				//bit 6, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 6, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_77(struct cpu* cpu, uint16_t reserved)				//bit 6, a   
{
	bit(cpu, 6, cpu->a);
}

void extended_instruction_78(struct cpu* cpu, uint16_t reserved)				//bit 7, b
{
	bit(cpu, 7, cpu->b);
}

void extended_instruction_79(struct cpu* cpu, uint16_t reserved)				//bit 7, c
{
	bit(cpu, 7, cpu->c);
}

void extended_instruction_7a(struct cpu* cpu, uint16_t reserved)				//bit 7, d
{
	bit(cpu, 7, cpu->d);
}

void extended_instruction_7b(struct cpu* cpu, uint16_t reserved)				//bit 7, e
{
	bit(cpu, 7, cpu->e);
}

void extended_instruction_7c(struct cpu* cpu, uint16_t reserved)				//bit 7, h
{
	bit(cpu, 7, cpu->h);
}

void extended_instruction_7d(struct cpu* cpu, uint16_t reserved)				//bit 7, l
{
	bit(cpu, 7, cpu->l);
}

void extended_instruction_7e(struct cpu* cpu, uint16_t reserved)				//bit 7, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	bit(cpu, 7, hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_7f(struct cpu* cpu, uint16_t reserved)				//bit 7, a   
{
	bit(cpu, 7, cpu->a);
}

void extended_instruction_80(struct cpu* cpu, uint16_t reserved)				//res 0, b
{
	res(cpu, 0, &cpu->b);
}

void extended_instruction_81(struct cpu* cpu, uint16_t reserved)				//res 0, c
{
	res(cpu, 0, &cpu->c);
}

void extended_instruction_82(struct cpu* cpu, uint16_t reserved)				//res 0, d
{
	res(cpu, 0, &cpu->d);
}

void extended_instruction_83(struct cpu* cpu, uint16_t reserved)				//res 0, e
{
	res(cpu, 0, &cpu->e);
}

void extended_instruction_84(struct cpu* cpu, uint16_t reserved)				//res 0, h
{
	res(cpu, 0, &cpu->h);
}

void extended_instruction_85(struct cpu* cpu, uint16_t reserved)				//res 0, l
{
	res(cpu, 0, &cpu->l);
}

void extended_instruction_86(struct cpu* cpu, uint16_t reserved)				//res 0, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 0, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_87(struct cpu* cpu, uint16_t reserved)				//res 0, a
{
	res(cpu, 0, &cpu->a);
}

void extended_instruction_88(struct cpu* cpu, uint16_t reserved)				//res 1, b
{
	res(cpu, 1, &cpu->b);
}

void extended_instruction_89(struct cpu* cpu, uint16_t reserved)				//res 1, c
{
	res(cpu, 1, &cpu->c);
}

void extended_instruction_8a(struct cpu* cpu, uint16_t reserved)				//res 1, d
{
	res(cpu, 1, &cpu->d);
}

void extended_instruction_8b(struct cpu* cpu, uint16_t reserved)				//res 1, e
{
	res(cpu, 1, &cpu->e);
}

void extended_instruction_8c(struct cpu* cpu, uint16_t reserved)				//res 1, h
{
	res(cpu, 1, &cpu->h);
}

void extended_instruction_8d(struct cpu* cpu, uint16_t reserved)				//res 1, l
{
	res(cpu, 1, &cpu->l);
}

void extended_instruction_8e(struct cpu* cpu, uint16_t reserved)				//res 1, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 1, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_8f(struct cpu* cpu, uint16_t reserved)				//res 1, a     
{
	res(cpu, 1, &cpu->a);
}

void extended_instruction_90(struct cpu* cpu, uint16_t reserved)				//res 2, b
{
	res(cpu, 2, &cpu->b);
}

void extended_instruction_91(struct cpu* cpu, uint16_t reserved)				//res 2, c
{
	res(cpu, 2, &cpu->c);
}

void extended_instruction_92(struct cpu* cpu, uint16_t reserved)				//res 2, d
{
	res(cpu, 2, &cpu->d);
}

void extended_instruction_93(struct cpu* cpu, uint16_t reserved)				//res 2, e
{
	res(cpu, 2, &cpu->e);
}

void extended_instruction_94(struct cpu* cpu, uint16_t reserved)				//res 2, h
{
	res(cpu, 2, &cpu->h);
}

void extended_instruction_95(struct cpu* cpu, uint16_t reserved)				//res 2, l
{
	res(cpu, 2, &cpu->l);
}

void extended_instruction_96(struct cpu* cpu, uint16_t reserved)				//res 2, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 2, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_97(struct cpu* cpu, uint16_t reserved)				//res 2, a
{
	res(cpu, 2, &cpu->a);
}

void extended_instruction_98(struct cpu* cpu, uint16_t reserved)				//res 3, b
{
	res(cpu, 3, &cpu->b);
}

void extended_instruction_99(struct cpu* cpu, uint16_t reserved)				//res 3, c
{
	res(cpu, 3, &cpu->c);
}

void extended_instruction_9a(struct cpu* cpu, uint16_t reserved)				//res 3, d
{
	res(cpu, 3, &cpu->d);
}

void extended_instruction_9b(struct cpu* cpu, uint16_t reserved)				//res 3, e
{
	res(cpu, 3, &cpu->e);
}

void extended_instruction_9c(struct cpu* cpu, uint16_t reserved)				//res 3, h
{
	res(cpu, 3, &cpu->h);
}

void extended_instruction_9d(struct cpu* cpu, uint16_t reserved)				//res 3, l
{
	res(cpu, 3, &cpu->l);
}

void extended_instruction_9e(struct cpu* cpu, uint16_t reserved)				//res 3, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 3, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_9f(struct cpu* cpu, uint16_t reserved)				//res 3, a     
{
	res(cpu, 3, &cpu->a);
}

void extended_instruction_a0(struct cpu* cpu, uint16_t reserved)				//res 4, b
{
	res(cpu, 4, &cpu->b);
}

void extended_instruction_a1(struct cpu* cpu, uint16_t reserved)				//res 4, c
{
	res(cpu, 4, &cpu->c);
}

void extended_instruction_a2(struct cpu* cpu, uint16_t reserved)				//res 4, d
{
	res(cpu, 4, &cpu->d);
}

void extended_instruction_a3(struct cpu* cpu, uint16_t reserved)				//res 4, e
{
	res(cpu, 4, &cpu->e);
}

void extended_instruction_a4(struct cpu* cpu, uint16_t reserved)				//res 4, h
{
	res(cpu, 4, &cpu->h);
}

void extended_instruction_a5(struct cpu* cpu, uint16_t reserved)				//res 4, l
{
	res(cpu, 4, &cpu->l);
}

void extended_instruction_a6(struct cpu* cpu, uint16_t reserved)				//res 4, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 4, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_a7(struct cpu* cpu, uint16_t reserved)				//res 4, a
{
	res(cpu, 4, &cpu->a);
}

void extended_instruction_a8(struct cpu* cpu, uint16_t reserved)				//res 5, b
{
	res(cpu, 5, &cpu->b);
}

void extended_instruction_a9(struct cpu* cpu, uint16_t reserved)				//res 5, c
{
	res(cpu, 5, &cpu->c);
}

void extended_instruction_aa(struct cpu* cpu, uint16_t reserved)				//res 5, d
{
	res(cpu, 5, &cpu->d);
}

void extended_instruction_ab(struct cpu* cpu, uint16_t reserved)				//res 5, e
{
	res(cpu, 5, &cpu->e);
}

void extended_instruction_ac(struct cpu* cpu, uint16_t reserved)				//res 5, h
{
	res(cpu, 5, &cpu->h);
}

void extended_instruction_ad(struct cpu* cpu, uint16_t reserved)				//res 5, l
{
	res(cpu, 5, &cpu->l);
}

void extended_instruction_ae(struct cpu* cpu, uint16_t reserved)				//res 5, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 5, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_af(struct cpu* cpu, uint16_t reserved)				//res 5, a     
{
	res(cpu, 5, &cpu->a);
}

void extended_instruction_b0(struct cpu* cpu, uint16_t reserved)				//res 6, b
{
	res(cpu, 6, &cpu->b);
}

void extended_instruction_b1(struct cpu* cpu, uint16_t reserved)				//res 6, c
{
	res(cpu, 6, &cpu->c);
}

void extended_instruction_b2(struct cpu* cpu, uint16_t reserved)				//res 6, d
{
	res(cpu, 6, &cpu->d);
}

void extended_instruction_b3(struct cpu* cpu, uint16_t reserved)				//res 6, e
{
	res(cpu, 6, &cpu->e);
}

void extended_instruction_b4(struct cpu* cpu, uint16_t reserved)				//res 6, h
{
	res(cpu, 6, &cpu->h);
}

void extended_instruction_b5(struct cpu* cpu, uint16_t reserved)				//res 6, l
{
	res(cpu, 6, &cpu->l);
}

void extended_instruction_b6(struct cpu* cpu, uint16_t reserved)				//res 6, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 6, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_b7(struct cpu* cpu, uint16_t reserved)				//res 6, a
{
	res(cpu, 6, &cpu->a);
}

void extended_instruction_b8(struct cpu* cpu, uint16_t reserved)				//res 7, b
{
	res(cpu, 7, &cpu->b);
}

void extended_instruction_b9(struct cpu* cpu, uint16_t reserved)				//res 7, c
{
	res(cpu, 7, &cpu->c);
}

void extended_instruction_ba(struct cpu* cpu, uint16_t reserved)				//res 7, d
{
	res(cpu, 7, &cpu->d);
}

void extended_instruction_bb(struct cpu* cpu, uint16_t reserved)				//res 7, e
{
	res(cpu, 7, &cpu->e);
}

void extended_instruction_bc(struct cpu* cpu, uint16_t reserved)				//res 7, h
{
	res(cpu, 7, &cpu->a);
}

void extended_instruction_bd(struct cpu* cpu, uint16_t reserved)				//res 7, l
{
	res(cpu, 7, &cpu->a);
}

void extended_instruction_be(struct cpu* cpu, uint16_t reserved)				//res 7, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	res(cpu, 7, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_bf(struct cpu* cpu, uint16_t reserved)				//res 7, a       
{
	res(cpu, 7, &cpu->a);
}

void extended_instruction_c0(struct cpu* cpu, uint16_t reserved)				//set 0, b
{
	set(cpu, 0, &cpu->b);
}

void extended_instruction_c1(struct cpu* cpu, uint16_t reserved)				//set 0, c
{
	set(cpu, 0, &cpu->c);
}

void extended_instruction_c2(struct cpu* cpu, uint16_t reserved)				//set 0, d
{
	set(cpu, 0, &cpu->d);
}

void extended_instruction_c3(struct cpu* cpu, uint16_t reserved)				//set 0, e
{
	set(cpu, 0, &cpu->e);
}

void extended_instruction_c4(struct cpu* cpu, uint16_t reserved)				//set 0, h
{
	set(cpu, 0, &cpu->h);
}

void extended_instruction_c5(struct cpu* cpu, uint16_t reserved)				//set 0, l
{
	set(cpu, 0, &cpu->l);
}

void extended_instruction_c6(struct cpu* cpu, uint16_t reserved)				//set 0, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 0, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_c7(struct cpu* cpu, uint16_t reserved)				//set 0, a
{
	set(cpu, 0, &cpu->a);
}

void extended_instruction_c8(struct cpu* cpu, uint16_t reserved)				//set 1, b
{
	set(cpu, 1, &cpu->b);
}

void extended_instruction_c9(struct cpu* cpu, uint16_t reserved)				//set 1, c
{
	set(cpu, 1, &cpu->c);
}

void extended_instruction_ca(struct cpu* cpu, uint16_t reserved)				//set 1, d
{
	set(cpu, 1, &cpu->d);
}

void extended_instruction_cb(struct cpu* cpu, uint16_t reserved)				//set 1, e
{
	set(cpu, 1, &cpu->e);
}

void extended_instruction_cc(struct cpu* cpu, uint16_t reserved)				//set 1, h
{
	set(cpu, 1, &cpu->h);
}

void extended_instruction_cd(struct cpu* cpu, uint16_t reserved)				//set 1, l
{
	set(cpu, 1, &cpu->l);
}

void extended_instruction_ce(struct cpu* cpu, uint16_t reserved)				//set 1, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 1, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_cf(struct cpu* cpu, uint16_t reserved)				//set 1, a    
{
	set(cpu, 1, &cpu->a);
}

void extended_instruction_d0(struct cpu* cpu, uint16_t reserved)				//set 2, b
{
	set(cpu, 2, &cpu->b);
}

void extended_instruction_d1(struct cpu* cpu, uint16_t reserved)				//set 2, c
{
	set(cpu, 2, &cpu->c);
}

void extended_instruction_d2(struct cpu* cpu, uint16_t reserved)				//set 2, d
{
	set(cpu, 2, &cpu->d);
}

void extended_instruction_d3(struct cpu* cpu, uint16_t reserved)				//set 2, e
{
	set(cpu, 2, &cpu->e);
}

void extended_instruction_d4(struct cpu* cpu, uint16_t reserved)				//set 2, h
{
	set(cpu, 2, &cpu->h);
}

void extended_instruction_d5(struct cpu* cpu, uint16_t reserved)				//set 2, l
{
	set(cpu, 2, &cpu->l);
}

void extended_instruction_d6(struct cpu* cpu, uint16_t reserved)				//set 2, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 2, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_d7(struct cpu* cpu, uint16_t reserved)				//set 2, a
{
	set(cpu, 2, &cpu->a);
}

void extended_instruction_d8(struct cpu* cpu, uint16_t reserved)				//set 3, b
{
	set(cpu, 3, &cpu->b);
}

void extended_instruction_d9(struct cpu* cpu, uint16_t reserved)				//set 3, c
{
	set(cpu, 3, &cpu->c);
}

void extended_instruction_da(struct cpu* cpu, uint16_t reserved)				//set 3, d
{
	set(cpu, 3, &cpu->d);
}

void extended_instruction_db(struct cpu* cpu, uint16_t reserved)				//set 3, e
{
	set(cpu, 3, &cpu->e);
}

void extended_instruction_dc(struct cpu* cpu, uint16_t reserved)				//set 3, h
{
	set(cpu, 3, &cpu->h);
}

void extended_instruction_dd(struct cpu* cpu, uint16_t reserved)				//set 3, l
{
	set(cpu, 3, &cpu->l);
}

void extended_instruction_de(struct cpu* cpu, uint16_t reserved)				//set 3, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 3, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_df(struct cpu* cpu, uint16_t reserved)				//set 3, a    
{
	set(cpu, 3, &cpu->a);
}

void extended_instruction_e0(struct cpu* cpu, uint16_t reserved)				//set 4, b
{
	set(cpu, 4, &cpu->b);
}

void extended_instruction_e1(struct cpu* cpu, uint16_t reserved)				//set 4, c
{
	set(cpu, 4, &cpu->c);
}

void extended_instruction_e2(struct cpu* cpu, uint16_t reserved)				//set 4, d
{
	set(cpu, 4, &cpu->d);
}

void extended_instruction_e3(struct cpu* cpu, uint16_t reserved)				//set 4, e
{
	set(cpu, 4, &cpu->e);
}

void extended_instruction_e4(struct cpu* cpu, uint16_t reserved)				//set 4, h
{
	set(cpu, 4, &cpu->h);
}

void extended_instruction_e5(struct cpu* cpu, uint16_t reserved)				//set 4, l
{
	set(cpu, 4, &cpu->l);
}

void extended_instruction_e6(struct cpu* cpu, uint16_t reserved)				//set 4, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 4, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_e7(struct cpu* cpu, uint16_t reserved)				//set 4, a
{
	set(cpu, 4, &cpu->a);
}

void extended_instruction_e8(struct cpu* cpu, uint16_t reserved)				//set 5, b
{
	set(cpu, 5, &cpu->b);
}

void extended_instruction_e9(struct cpu* cpu, uint16_t reserved)				//set 5, c
{
	set(cpu, 5, &cpu->c);
}

void extended_instruction_ea(struct cpu* cpu, uint16_t reserved)				//set 5, d
{
	set(cpu, 5, &cpu->d);
}

void extended_instruction_eb(struct cpu* cpu, uint16_t reserved)				//set 5, e
{
	set(cpu, 5, &cpu->d);
}

void extended_instruction_ec(struct cpu* cpu, uint16_t reserved)				//set 5, h
{
	set(cpu, 5, &cpu->h);
}

void extended_instruction_ed(struct cpu* cpu, uint16_t reserved)				//set 5, l
{
	set(cpu, 5, &cpu->l);
}

void extended_instruction_ee(struct cpu* cpu, uint16_t reserved)				//set 5, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 5, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_ef(struct cpu* cpu, uint16_t reserved)				//set 5, a    
{
	set(cpu, 5, &cpu->a);
}

void extended_instruction_f0(struct cpu* cpu, uint16_t reserved)				//set 6, b
{
	set(cpu, 6, &cpu->b);
}

void extended_instruction_f1(struct cpu* cpu, uint16_t reserved)				//set 6, c
{
	set(cpu, 6, &cpu->c);
}

void extended_instruction_f2(struct cpu* cpu, uint16_t reserved)				//set 6, d
{
	set(cpu, 6, &cpu->d);
}

void extended_instruction_f3(struct cpu* cpu, uint16_t reserved)				//set 6, e
{
	set(cpu, 6, &cpu->e);
}

void extended_instruction_f4(struct cpu* cpu, uint16_t reserved)				//set 6, h
{
	set(cpu, 6, &cpu->h);
}

void extended_instruction_f5(struct cpu* cpu, uint16_t reserved)				//set 6, l
{
	set(cpu, 6, &cpu->l);
}

void extended_instruction_f6(struct cpu* cpu, uint16_t reserved)				//set 6, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 6, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_f7(struct cpu* cpu, uint16_t reserved)				//set 6, a
{
	set(cpu, 6, &cpu->a);
}

void extended_instruction_f8(struct cpu* cpu, uint16_t reserved)				//set 7, b
{
	set(cpu, 7, &cpu->b);
}

void extended_instruction_f9(struct cpu* cpu, uint16_t reserved)				//set 7, c
{
	set(cpu, 7, &cpu->c);
}

void extended_instruction_fa(struct cpu* cpu, uint16_t reserved)				//set 7, d
{
	set(cpu, 7, &cpu->d);
}

void extended_instruction_fb(struct cpu* cpu, uint16_t reserved)				//set 7, e
{
	set(cpu, 7, &cpu->e);
}

void extended_instruction_fc(struct cpu* cpu, uint16_t reserved)				//set 7, h
{
	set(cpu, 7, &cpu->h);
}

void extended_instruction_fd(struct cpu* cpu, uint16_t reserved)				//set 7, l
{
	set(cpu, 7, &cpu->l);
}

void extended_instruction_fe(struct cpu* cpu, uint16_t reserved)				//set 7, (hl)
{
	uint8_t hl_address_value = bus_read(cpu->bus, REGISTER_HL(cpu));

	set(cpu, 7, &hl_address_value);

	bus_write(cpu->bus, REGISTER_HL(cpu), hl_address_value);
}

void extended_instruction_ff(struct cpu* cpu, uint16_t reserved)				//set 7, a
{
	set(cpu, 7, &cpu->a);
}