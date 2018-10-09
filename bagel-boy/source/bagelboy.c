#include "bagelboy.h"

#include "bus.h"
#include "cartridge.h"
#include "cpu.h"
#include "interrupt_controller.h"
#include "io_registers.h"
#include "joypad.h"
#include "lcd_controller.h"
#include "msc.h"
#include "rom_chip.h"
#include "serial_transfer_controller.h"

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void bagelboy_initialize(struct bagelboy* bagelboy, const char* filename)
{
    memset(bagelboy, 0, sizeof(struct bagelboy));

    bagelboy->power = true;
}

void bagelboy_destroy(struct bagelboy* bagelboy)
{
    
}

void bagelboy_emulate(struct bagelboy* bagelboy)
{
	if (!bagelboy->cpu.halted)
	{
		cpu_cycle(&bagelboy->cpu, &bagelboy->cpu.clock_cycles);
	}

	//lcd_controller_update(bagelboy->lcd_controller, &bagelboy->cpu->clock_cycles);

	cpu_handle_interrupts(&bagelboy->cpu);

	++bagelboy->cpu.clock_cycles;
}