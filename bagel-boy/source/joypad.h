#ifndef JOYPAD_H_
#define JOYPAD_H_

#pragma once

#include <stdint.h>

struct joypad {
	uint8_t p1;

	struct interrupt_controller* interrupt_controller;
};

struct joypad* joypad_create(struct interrupt_controller* interrupt_controller);
void joypad_destroy(struct joypad* joypad);

uint8_t joypad_on_read(struct joypad* joypad, uint16_t address);
void joypad_on_write(struct joypad* joypad, uint16_t address, uint8_t value);

#endif