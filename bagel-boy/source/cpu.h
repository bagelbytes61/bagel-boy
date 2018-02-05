#ifndef CPU_H_
#define CPU_H_

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define REGISTER_AF(cpu) ((uint16_t)(cpu)->a << 8 | (cpu)->f)
#define REGISTER_BC(cpu) ((uint16_t)(cpu)->b << 8 | (cpu)->c)
#define REGISTER_DE(cpu) ((uint16_t)(cpu)->d << 8 | (cpu)->e)
#define REGISTER_HL(cpu) ((uint16_t)(cpu)->h << 8 | (cpu)->l)

#define FLAG_ZERO		0x07
#define FLAG_SUBTRACT	0x06
#define FLAG_HALFCARRY	0x05
#define FLAG_CARRY		0x04

#define FLAG_STATE_SET		0x01
#define FLAG_STATE_RESET	0x00

#define SET_FLAG_STATE(cpu, flag, state) ((cpu)->f ^= (-(state) ^ (cpu)->f) & (1 << (flag)))

#define FLAG_STATE(cpu, flag)	(((cpu)->f >> (flag)) & 0x01)

extern const uint8_t boot_rom[256];

struct cpu
{
	uint8_t a, f;
	uint8_t b, c;
	uint8_t d, e;
	uint8_t h, l;
	
	uint16_t sp;
	uint16_t pc;

	uint8_t ime;

	bool halted;

	struct interrupt_controller* interrupt_controller;

	struct bus* bus;
};

struct cpu* cpu_create(struct interrupt_controller* interrupt_controller, struct bus* bus);
void cpu_destroy(struct cpu* cpu);

void cpu_cycle(struct cpu* cpu, const uint64_t* clock_cycles);

void cpu_handle_interrupts(struct cpu* cpu);

#endif