#ifndef INTERRUPT_CONTROLLER_H_
#define INTERRUPT_CONTROLLER_H_

#pragma once

#include <stdint.h>

#define INTERRUPT_FLAG_V_BLANK		0x01
#define INTERRUPT_FLAG_LCDC_STAT	0x02
#define INTERRUPT_FLAG_TIMER		0x04
#define INTERRUPT_FLAG_SERIAL		0x08
#define INTERRUPT_FLAG_JOYPAD		0x10

#define INTERRUPT_V_BLANK_ADDRESS	0x40
#define INTERRUPT_LCDC_STAT_ADDRESS	0x48
#define INTERRUPT_TIMER_ADDRESS		0x50
#define INTERRUPT_SERIAL_ADDRESS	0x58
#define INTERRUPT_JOYPAD_ADDRESS	0x60

struct interrupt_controller
{
	uint8_t interrupt_flag;
	uint8_t interrupt_enable;
};

struct interrupt_controller* interrupt_controller_create();
void interrupt_controller_destroy(struct interrupt_controller* interrupt_controller);

void interrupt_controller_request_interrupt(struct interrupt_controller* interrupt_controller, uint8_t interrupt);

uint8_t interrupt_controller_on_read(struct interrupt_controller* interrupt_controller, uint16_t address);
void interrupt_controller_on_write(struct interrupt_controller* interrupt_controller, uint16_t address, uint8_t value);

#endif