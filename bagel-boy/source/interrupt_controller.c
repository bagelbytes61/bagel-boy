#include "interrupt_controller.h"

#include "io_registers.h"

#include <memory.h>
#include <stdlib.h>

void interupt_controller_initialize(struct interrupt_controller* interrupt_controller, uint8_t* interrupt_enable);
void interrupt_controller_deinitialize(struct interrupt_controller* interrupt_controller);

struct interrupt_controller* interrupt_controller_create(uint8_t* interrupt_enable)
{
    struct interrupt_controller* interrupt_controller = (struct interrupt_controller*)malloc(sizeof(struct interrupt_controller));

    interupt_controller_initialize(interrupt_controller, interrupt_enable);

    return interrupt_controller;
}

void interrupt_controller_destroy(struct interrupt_controller* interrupt_controller)
{
    interrupt_controller_deinitialize(interrupt_controller);

    free(interrupt_controller);
}

void interupt_controller_initialize(struct interrupt_controller* interrupt_controller, uint8_t* interrupt_enable)
{
    memset(interrupt_controller, 0, sizeof(struct interrupt_controller));

    interrupt_controller->interrupt_enable = interrupt_enable;
}

void interrupt_controller_deinitialize(struct interrupt_controller* interrupt_controller)
{

}

void interrupt_controller_request_interrupt(struct interrupt_controller* interrupt_controller, uint8_t interrupt)
{
    interrupt_controller->interrupt_flags |= interrupt;
}