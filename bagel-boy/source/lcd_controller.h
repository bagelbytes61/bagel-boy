#ifndef LCD_CONTROLLER_H_
#define LCD_CONTROLLER_H_

#pragma once

#include "vram.h"

#include <stdint.h>

#define LCD_MODE_00_CLK_CYCLE_BEGIN     201
#define LCD_MODE_00_CLK_CYCLE_END       207

#define LCD_MODE_01_CLK_CYCLE_BEGIN     144
#define LCD_MODE_01_CLK_CYCLE_END       153

#define LCD_MODE_10_CLK_CYCLE_BEGIN     77
#define LCD_MODE_10_CLK_CYCLE_END       83

#define LCD_MODE_11_CLK_CYCLE_BEGIN     169
#define LCD_MODE_11_CLK_CYCLE_END       175

#define LCD_MODE_00	0x00
#define LCD_MODE_01	0x01
#define LCD_MODE_10	0x02
#define LCD_MODE_11	0x03

#define BACKGROUND_TILE_MAP_OFFSET 0x1800

struct lcd_controller
{
	uint8_t lcdc;
	uint8_t stat;

	uint8_t scy, scx;

	uint8_t ly;
	uint8_t lyc;

	uint8_t bgp;

	uint8_t obp0, obp1;

	uint8_t wy, wx;

	struct interrupt_controller* interrupt_controller;

	struct bus** bus;

	vram_t* vram;
};

struct lcd_controller* lcd_controller_create(struct interrupt_controller* interrupt_controller, struct bus** bus, struct L3D_GraphicsDevice* device);
void lcd_controller_destroy(struct lcd_controller* lcd_controller);

void lcd_controller_update(struct lcd_controller* lcd_controller, const uint64_t* clock_cycles);

uint8_t lcd_controller_on_read(struct lcd_controller* lcd_controller, uint16_t address);
void lcd_controller_on_write(struct lcd_controller* lcd_controller, uint16_t address, uint8_t value);

#endif