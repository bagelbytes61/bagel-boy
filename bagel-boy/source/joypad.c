#include "joypad.h"

#include <memory.h>
#include <stdlib.h>

void joypad_initialize(struct joypad* joypad, struct interrupt_controller* interrupt_controller);
void joypad_deinitialize(struct joypad* joypad);

struct joypad* joypad_create(struct interrupt_controller* interrupt_controller)
{
	struct joypad* joypad = (struct joypad*)malloc(sizeof(struct joypad));

	joypad_initialize(joypad, interrupt_controller);

	return joypad;
}

void joypad_destroy(struct joypad* joypad)
{
	free(joypad);
}

void joypad_initialize(struct joypad* joypad, struct interrupt_controller* interrupt_controller)
{
	memset(joypad, 0, sizeof(struct joypad));

	joypad->interrupt_controller = interrupt_controller;
}

void joypad_deinitialize(struct joypad* joypad)
{

}

uint8_t joypad_on_read(struct joypad* joypad, uint16_t address)
{
	return joypad->p1;
}

void joypad_on_write(struct joypad* joypad, uint16_t address, uint8_t value)
{

}