#include "interrupt_controller.h"

#include "io_registers.h"

#include <memory.h>
#include <stdlib.h>

void interupt_controller_initialize(struct interrupt_controller* interrupt_controller);
void interrupt_controller_deinitialize(struct interrupt_controller* interrupt_controller);

struct interrupt_controller* interrupt_controller_create()
{
	struct interrupt_controller* interrupt_controller = (struct interrupt_controller*)malloc(sizeof(struct interrupt_controller));

	interupt_controller_initialize(interrupt_controller);

	return interrupt_controller;
}

void interrupt_controller_destroy(struct interrupt_controller* interrupt_controller)
{
	interrupt_controller_deinitialize(interrupt_controller);

	free(interrupt_controller);
}

void interupt_controller_initialize(struct interrupt_controller* interrupt_controller)
{
	memset(interrupt_controller, 0, sizeof(struct interrupt_controller));
}

void interrupt_controller_deinitialize(struct interrupt_controller* interrupt_controller)
{

}

void interrupt_controller_request_interrupt(struct interrupt_controller* interrupt_controller, uint8_t interrupt)
{
	
}

uint8_t interrupt_controller_on_read(struct interrupt_controller* interrupt_controller, uint16_t address)
{
	switch (address)
	{
		case IO_REGISTER_IF:
			return interrupt_controller->interrupt_flag;
		case IO_REGISTER_IE:
			return interrupt_controller->interrupt_enable;
	}
}

void interrupt_controller_on_write(struct interrupt_controller* interrupt_controller, uint16_t address, uint8_t value)
{

}