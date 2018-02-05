#include "serial_transfer_controller.h"

#include "io_registers.h"

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

void serial_transfer_controller_initialize(struct serial_transfer_controller* serial_transfer_controller, struct interrupt_controller* interrupt_controller);
void serial_transfer_controller_deinitialize(struct serial_transfer_controller* serial_transfer_controller);

struct serial_transfer_controller* serial_transfer_controller_create(struct interrupt_controller* interrupt_controller)
{
	struct serial_transfer_controller* serial_transfer_controller = (struct serial_transfer_controller*)malloc(sizeof(struct serial_transfer_controller));

	serial_transfer_controller_initialize(serial_transfer_controller, interrupt_controller);

	return serial_transfer_controller;
}

void serial_transfer_controller_destroy(struct serial_transfer_controller* serial_transfer_controller)
{
	serial_transfer_controller_deinitialize(serial_transfer_controller);
}

void serial_transfer_controller_initialize(struct serial_transfer_controller* serial_transfer_controller, struct interrupt_controller* interrupt_controller)
{
	memset(serial_transfer_controller, 0, sizeof(struct serial_transfer_controller));

	serial_transfer_controller->interrupt_controller = interrupt_controller;
}

void serial_transfer_controller_deinitialize(struct serial_transfer_controller* serial_transfer_controller)
{

}

uint8_t serial_transfer_controller_on_read(struct serial_transfer_controller* serial_transfer_controller, uint16_t address)
{
	switch (address)
	{
		case IO_REGISTER_SB:
			return serial_transfer_controller->sb;
		case IO_REGISTER_SC:
			return serial_transfer_controller->sc;
	}
}

void serial_transfer_controller_on_write(struct serial_transfer_controller* serial_transfer_controller, uint16_t address, uint8_t value)
{
	switch (address)
	{
		case IO_REGISTER_SB:
			serial_transfer_controller->sb = value;
			break;
		case IO_REGISTER_SC:
			//if (value == 0x81)
			{
				printf("%c", serial_transfer_controller->sb);
			}
			break;
	}

	Sleep(1000);
}