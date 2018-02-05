#ifndef BUS_H_
#define BUS_H_

#pragma once

#include <stdint.h>

#define HARDWARE									19

#define ROM_BANK_0_									0
#define ROM_BANK_0_BEGIN_ADDRESS					0x0000
#define ROM_BANK_0_END_ADDRESS						0x4000

#define ROM_BANK_1_									1
#define ROM_BANK_1_BEGIN_ADDRESS					0x4000
#define ROM_BANK_1_END_ADDRESS						0x8000

#define VRAM_										2
#define VRAM_BEGIN_ADDRESS							0x8000
#define VRAM_END_ADDRESS							0xA000

#define EXTERNAL_RAM_								3
#define EXTERNAL_RAM_BEGIN_ADDRESS					0xA000
#define EXTERNAL_RAM_END_ADDRESS					0xC000

#define WRAM_BANK_0_								4
#define WRAM_BANK_0_BEGIN_ADDRESS					0xC000
#define WRAM_BANK_0_END_ADDRESS						0xD000

#define WRAM_BANK_1_								5
#define WRAM_BANK_1_BEGIN_ADDRESS					0xD000
#define WRAM_BANK_1_END_ADDRESS						0xE000

#define ECHO_										6
#define ECHO_BEGIN_ADDRESS							0xE000
#define ECHO_END_ADDRESS							0xFE00

#define OAM_										7
#define OAM_BEGIN_ADDRESS							0xFE00
#define OAM_END_ADDRESS								0xFEA0

#define JOYPAD_										8
#define JOYPAD_BEGIN_ADDRESS						0xFF00
#define JOYPAD_END_ADDRESS							0xFF01

#define SERIAL_TRANSFER_CONTROLLER_					9
#define SERIAL_TRANSFER_CONTROLLER_BEGIN_ADDRESS	0xFF01
#define SERIAL_TRANSFER_CONTROLLER_END_ADDRESS		0xFF02

#define DIVIDER_									10
#define DIVIDER_BEGIN_ADDRESS						0xFF04
#define DIVIDER_END_ADDRESS							0xFF05

#define TIMA_										11
#define TIMA_BEGIN_ADDRESS							0xFF05
#define TIMA_END_ADDRESS							0xFF06

#define TMA_										12
#define TMA_BEGIN_ADDRESS							0xFF06
#define TMA_END_ADDRESS								0xFF07

#define TAC_										13
#define TAC_BEGIN_ADDRESS							0xFF07
#define TAC_END_ADDRESS								0xFF08

#define LCD_CONTROLLER_								14
#define LCD_CONTROLLER_BEGIN_ADDRESS				0xFF40
#define LCD_CONTROLLER_END_ADDRESS					0xFF46

#define IF_											15
#define IF_BEGIN_ADDRESS							0xFF00
#define IF_END_ADDRESS								0xFF10

#define MSC_										16
#define MSC_BEGIN_ADDRESS							0x0000
#define MSC_END_ADDRESS								0xFF51

#define HRAM_										17
#define HRAM_BEGIN_ADDRESS							0xFF80
#define HRAM_END_ADDRESS							0xFFFF

#define IER_										18
#define IER_BEGIN_ADDRESS							0xFFFF
#define IER_END_ADDRESS								0x10000

struct hardware
{
	void* hardware;

	uint16_t begin;
	uint16_t end;

	uint8_t(*on_read)(void*, uint16_t);
	void(*on_write)(void*, uint16_t, uint8_t);
};

struct bus
{
	struct hardware hardware_map[HARDWARE];
};

struct bus* bus_create(struct bagelboy* bagelboy);
void bus_destroy(struct bus* bus);

uint8_t bus_read(struct bus* bus, uint16_t address);
void bus_write(struct bus* bus, uint16_t address, uint8_t value);

#endif