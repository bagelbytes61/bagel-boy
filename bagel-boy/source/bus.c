#include "bus.h"

#include "bagelboy.h"
#include "joypad.h"
#include "lcd_controller.h"
#include "msc.h"
#include "serial_transfer_controller.h"
#include "vram.h"

#include <memory.h>
#include <stdlib.h>

void bus_initialize(struct bus* bus, struct bagelboy* bagelboy);
void bus_deinitialize(struct bus* bus);

struct bus* bus_create(struct bagelboy* bagelboy)
{
	struct bus* bus = (struct bus*)malloc(sizeof(struct bus));

	bus_initialize(bus, bagelboy);

	return bus;
}

void bus_destroy(struct bus* bus)
{
	free(bus);
}

void bus_initialize(struct bus* bus, struct bagelboy* bagelboy)
{
	memset(bus, 0, sizeof(struct bus));

	bus->hardware_map[ROM_BANK_0_].hardware = bagelboy->msc;
	bus->hardware_map[ROM_BANK_0_].begin = ROM_BANK_0_BEGIN_ADDRESS;
	bus->hardware_map[ROM_BANK_0_].end = ROM_BANK_0_END_ADDRESS;
	bus->hardware_map[ROM_BANK_0_].on_read = &msc_on_read;
	bus->hardware_map[ROM_BANK_0_].on_write = &msc_on_write;

	bus->hardware_map[ROM_BANK_1_].hardware = bagelboy->msc;
	bus->hardware_map[ROM_BANK_1_].begin = ROM_BANK_1_BEGIN_ADDRESS;
	bus->hardware_map[ROM_BANK_1_].end = ROM_BANK_1_END_ADDRESS;
	bus->hardware_map[ROM_BANK_1_].on_read = &msc_on_read;
	bus->hardware_map[ROM_BANK_1_].on_write = &msc_on_write;

	bus->hardware_map[VRAM_].hardware = bagelboy->vram;
	bus->hardware_map[VRAM_].begin = VRAM_BEGIN_ADDRESS;
	bus->hardware_map[VRAM_].end = VRAM_END_ADDRESS;
	bus->hardware_map[VRAM_].on_read = &vram_on_read;
	bus->hardware_map[VRAM_].on_write = &vram_on_write;

	bus->hardware_map[WRAM_BANK_0_].hardware = bagelboy->wram_bank_0;
	bus->hardware_map[WRAM_BANK_0_].begin = WRAM_BANK_0_BEGIN_ADDRESS;
	bus->hardware_map[WRAM_BANK_0_].end = WRAM_BANK_0_END_ADDRESS;
	bus->hardware_map[WRAM_BANK_0_].on_read = &wram_on_read;
	bus->hardware_map[WRAM_BANK_0_].on_write = &wram_on_write;

	bus->hardware_map[WRAM_BANK_1_].hardware = bagelboy->wram_bank_1;
	bus->hardware_map[WRAM_BANK_1_].begin = WRAM_BANK_0_BEGIN_ADDRESS;
	bus->hardware_map[WRAM_BANK_1_].end = WRAM_BANK_0_END_ADDRESS;
	bus->hardware_map[WRAM_BANK_1_].on_read = &wram_on_read;
	bus->hardware_map[WRAM_BANK_1_].on_write = &wram_on_write;

	bus->hardware_map[JOYPAD_].hardware = bagelboy->joypad;
	bus->hardware_map[JOYPAD_].begin = JOYPAD_BEGIN_ADDRESS;
	bus->hardware_map[JOYPAD_].end = JOYPAD_END_ADDRESS;
	bus->hardware_map[JOYPAD_].on_read = &joypad_on_read;
	bus->hardware_map[JOYPAD_].on_write = &joypad_on_write;

	bus->hardware_map[SERIAL_TRANSFER_CONTROLLER_].hardware = bagelboy->serial_transfer_controller;
	bus->hardware_map[SERIAL_TRANSFER_CONTROLLER_].begin = SERIAL_TRANSFER_CONTROLLER_BEGIN_ADDRESS;
	bus->hardware_map[SERIAL_TRANSFER_CONTROLLER_].end = SERIAL_TRANSFER_CONTROLLER_END_ADDRESS;
	bus->hardware_map[SERIAL_TRANSFER_CONTROLLER_].on_read = &serial_transfer_controller_on_read;
	bus->hardware_map[SERIAL_TRANSFER_CONTROLLER_].on_write = &serial_transfer_controller_on_write;

	bus->hardware_map[LCD_CONTROLLER_].hardware = bagelboy->lcd_controller;
	bus->hardware_map[LCD_CONTROLLER_].begin = LCD_CONTROLLER_BEGIN_ADDRESS;
	bus->hardware_map[LCD_CONTROLLER_].end = LCD_CONTROLLER_END_ADDRESS;
	bus->hardware_map[LCD_CONTROLLER_].on_read = &lcd_controller_on_read;
	bus->hardware_map[LCD_CONTROLLER_].on_write = &lcd_controller_on_write;

	bus->hardware_map[MSC_].hardware = bagelboy->msc;
	bus->hardware_map[MSC_].begin = MSC_BEGIN_ADDRESS;
	bus->hardware_map[MSC_].end = MSC_END_ADDRESS;
	bus->hardware_map[MSC_].on_read = &msc_on_read;
	bus->hardware_map[MSC_].on_write = &msc_on_write;

	bus->hardware_map[HRAM_].hardware = bagelboy->hram;
	bus->hardware_map[HRAM_].begin = HRAM_BEGIN_ADDRESS;
	bus->hardware_map[HRAM_].end = HRAM_END_ADDRESS;
	bus->hardware_map[HRAM_].on_read = &hram_on_read;
	bus->hardware_map[HRAM_].on_write = &hram_on_write;
}

void bus_deinitialize(struct bus* bus)
{

}

uint8_t bus_read(struct bus* bus, uint16_t address)
{
	uint8_t hardware_index = 0;
	while (address > bus->hardware_map[hardware_index].end) { hardware_index++; }

	void* hardware = bus->hardware_map[hardware_index].hardware;

	uint16_t offset = bus->hardware_map[hardware_index].begin;

	return bus->hardware_map[hardware_index].on_read(hardware, address - offset);
}

void bus_write(struct bus* bus, uint16_t address, uint8_t value)
{
	uint8_t hardware_index = 0;
	while (address > bus->hardware_map[hardware_index].end) { hardware_index++; }

	void* hardware = bus->hardware_map[hardware_index].hardware;

	uint16_t offset = bus->hardware_map[hardware_index].begin;

	bus->hardware_map[hardware_index].on_write(hardware, address - offset, value);
}