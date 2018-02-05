#ifndef BAGEL_BOY_H_
#define BAGEL_BOY_H_

#pragma once

#include "hram.h"
#include "vram.h"
#include "wram.h"

#include <stdbool.h>

struct bagelboy
{
	struct L3D_Application* application;
	struct L3D_Window* window;
	struct L3D_GraphicsDevice* device;

	struct cartridge* cartridge;

	struct msc* msc;

	vram_t vram[0x2000];
	wram_t wram_bank_0[0x1000];
	wram_t wram_bank_1[0x1000];
	hram_t hram[0x80];

	struct interrupt_controller* interrupt_controller;

	struct joypad* joypad;
	struct serial_transfer_controller* serial_transfer_controller;
	struct divider* divider;
	struct timer* timer;
	struct lcd_controller* lcd_controller;

	struct bus* bus;

	struct cpu* cpu;

	bool power;
};

struct bagelboy* bagelboy_create(const char* filename);
void bagelboy_destroy(struct bagelboy* bagelboy);

void bagelboy_emulate(struct bagelboy* bagelboy);

#endif