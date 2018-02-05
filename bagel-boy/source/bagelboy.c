#include "bagelboy.h"

#include "Graphics\GraphicsDevice.h"

#include "Platform\Application.h"
#include "Platform\Window.h"

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

void bagelboy_initialize(struct bagelboy* bagelboy, const char* filename);
void bagelboy_deinitialize(struct bagelboy* bagelboy);

struct bagelboy* bagelboy_create(const char* filename)
{
	struct bagelboy* bagelboy = (struct bagelboy*)malloc(sizeof(struct bagelboy));
	bagelboy_initialize(bagelboy, filename);

	return bagelboy;
}

void bagelboy_destroy(struct bagelboy* bagelboy)
{
	bagelboy_deinitialize(bagelboy);

	free(bagelboy);
}

void bagelboy_initialize(struct bagelboy* bagelboy, const char* filename)
{
	memset(bagelboy, 0, sizeof(struct bagelboy));

	bagelboy->application = L3D_CreateApplication();
	L3D_InitializeWindow(bagelboy->application, &bagelboy->window, "Bagel Boy", 768, 576, 0, 0, false);

	bagelboy->device = L3D_CreateGraphicsDevice(L3D_GetWindowHandle(bagelboy->window), 768, 576, false);

	bagelboy->cartridge = cartridge_create(filename);

	bagelboy->msc = msc_create(boot_rom, bagelboy->cartridge->rom_chip);

	bagelboy->interrupt_controller = interrupt_controller_create();

	bagelboy->joypad = joypad_create(bagelboy->interrupt_controller);
	bagelboy->serial_transfer_controller = serial_transfer_controller_create(bagelboy->interrupt_controller);
	bagelboy->lcd_controller = lcd_controller_create(bagelboy->interrupt_controller, &bagelboy->bus, bagelboy->device);

	bagelboy->bus = bus_create(bagelboy);

	bagelboy->cpu = cpu_create(bagelboy->interrupt_controller, bagelboy->bus);

	bagelboy->power = true;

	bagelboy->lcd_controller->vram = bagelboy->vram;
}

void bagelboy_deinitialize(struct bagelboy* bagelboy)
{
	cpu_destroy(bagelboy->cpu);

	bus_destroy(bagelboy->bus);

	lcd_controller_destroy(bagelboy->lcd_controller);
	serial_transfer_controller_destroy(bagelboy->serial_transfer_controller);
	joypad_destroy(bagelboy->joypad);

	interrupt_controller_destroy(bagelboy->interrupt_controller);

	cartridge_destroy(bagelboy->cartridge);

	L3D_DestroyGraphicsDevice(bagelboy->device);

	L3D_DestroyWindow(bagelboy->window);
	L3D_DestroyApplication(bagelboy->application);
}

void bagelboy_emulate(struct bagelboy* bagelboy)
{
	static uint64_t clock_cycles = 0;

	L3D_UpdateApplication(bagelboy->application);

	if (!bagelboy->cpu->halted)
	{
		cpu_cycle(bagelboy->cpu, &clock_cycles);
	}

	lcd_controller_update(bagelboy->lcd_controller, &clock_cycles);

	cpu_handle_interrupts(bagelboy->cpu);

	clock_cycles++;
}