#ifndef SERIAL_TRANSFER_CONTROLLER_H_
#define SERIAL_TRANSFER_CONTROLLER_H_

#pragma once

#include <stdint.h>

struct serial_transfer_controller
{
	uint8_t sb;
	uint8_t sc;

	struct interrupt_controller* interrupt_controller;
};

struct serial_transfer_controller* serial_transfer_controller_create(struct interrupt_controller* interrupt_controller);
void serial_transfer_controller_destroy(struct serial_transfer_controller* serial_transfer_controller);

uint8_t serial_transfer_controller_on_read(struct serial_transfer_controller* serial_transfer_controller, uint16_t address);
void serial_transfer_controller_on_write(struct serial_transfer_controller* serial_transfer_controller, uint16_t address, uint8_t value);

#endif