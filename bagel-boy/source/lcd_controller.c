#include "lcd_controller.h"

#include "bus.h"
#include "io_registers.h"
#include "utilities.h"

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static void lcd_controller_initialize(struct lcd_controller* lcd_controller, struct interrupt_controller* interrupt_controller, struct bus** bus, struct L3D_GraphicsDevice* device);
static void lcd_controller_deinitialize(struct lcd_controller* lcd_controller);

struct lcd_controller* lcd_controller_create(struct interrupt_controller* interrupt_controller, struct bus** bus, struct L3D_GraphicsDevice* device)
{
	struct lcd_controller* lcd_controller = (struct lcd_controller*)malloc(sizeof(struct lcd_controller));

	lcd_controller_initialize(lcd_controller, interrupt_controller, bus, device);

	return lcd_controller;
}

void lcd_controller_destroy(struct lcd_controller* lcd_controller)
{

}

void lcd_controller_initialize(struct lcd_controller* lcd_controller, struct interrupt_controller* interrupt_controller, struct bus** bus, struct L3D_GraphicsDevice* device)
{
	memset(lcd_controller, 0, sizeof(struct lcd_controller));

	lcd_controller->interrupt_controller = interrupt_controller;

	lcd_controller->bus = bus;
}

void lcd_controller_deinitialize(struct lcd_controller* lcd_controller)
{
	
}

void lcd_controller_update(struct lcd_controller* lcd_controller, const uint64_t* clock_cycles)
{
	if (*clock_cycles % 456 >= LCD_MODE_10_CLK_CYCLE_BEGIN && *clock_cycles % 456 <= LCD_MODE_10_CLK_CYCLE_END)
	{
		lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_10;
	}
	else if (*clock_cycles % 456 >= LCD_MODE_11_CLK_CYCLE_BEGIN && *clock_cycles % 456 <= LCD_MODE_11_CLK_CYCLE_END)
	{
		lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_11;
	}
	else if (*clock_cycles % 456 >= LCD_MODE_00_CLK_CYCLE_BEGIN && *clock_cycles % 456 <= LCD_MODE_00_CLK_CYCLE_END)
	{
		lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_00;
	}
	else if (*clock_cycles % 456 == 0)
	{
		if (lcd_controller->ly >= 144 && lcd_controller->ly <= 153)
		{
			lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_01;
		}

		lcd_controller->ly = ++(lcd_controller->ly) % 154;
	}
}

uint8_t lcd_controller_on_read(struct lcd_controller* lcd_controller, uint16_t address)
{
	switch (address)
	{
		case IO_REGISTER_LCDC:
			return lcd_controller->lcdc;
		case IO_REGISTER_STAT:
			return lcd_controller->stat;
		case IO_REGISTER_SCY:
			return lcd_controller->scy;
		case IO_REGISTER_SCX:
			return lcd_controller->scx;
		case IO_REGISTER_LY:
			return lcd_controller->ly;
		case IO_REGISTER_LYC:
			return lcd_controller->lyc;
		case IO_REGISTER_WY:
			return lcd_controller->wy;
		case IO_REGISTER_WX:
			return lcd_controller->wx;
	}
}

void lcd_controller_on_write(struct lcd_controller* lcd_controller, uint16_t address, uint8_t value)
{
	switch (address)
	{
		case IO_REGISTER_LCDC:
			break;
		case IO_REGISTER_STAT:
			break;
		case IO_REGISTER_SCY:
			lcd_controller->scy = value;
			break;
		case IO_REGISTER_SCX:
			lcd_controller->scx = value;
			break;
		case IO_REGISTER_LY:
			lcd_controller->ly = 0;
			break;
		case IO_REGISTER_LYC:
			break;
		case IO_REGISTER_WY:
			lcd_controller->wy = value;
			break;
		case IO_REGISTER_WX:
			lcd_controller->wx = value;
			break;
	}
}




















//if (lcd_controller->ly == 144)
//{
//    const uint8_t color_pallette[4] = { 0x00, 0x0F, 0xF0, 0xFF };
//
//    for (uint16_t y = 0; y < 256; y++)
//    {
//        for (uint16_t x = 0; x < 256; x++)
//        {
//            uint8_t tile_index_value = lcd_controller->vram[0x1800 + (y / 8) * 32 + (x / 8)];
//
//            uint16_t vram_index = tile_index_value * 16 + (y % 8) * 2 + (x / 4) % 2;
//
//            uint8_t tile_pixel_value = (lcd_controller->vram[vram_index] >> (6 - ((x % 4) * 2))) & 0x03;
//
//            background_texture_data[(y * 256 + x) * 4 + 0] = color_pallette[tile_pixel_value];
//            background_texture_data[(y * 256 + x) * 4 + 1] = color_pallette[tile_pixel_value];
//            background_texture_data[(y * 256 + x) * 4 + 2] = color_pallette[tile_pixel_value];
//            background_texture_data[(y * 256 + x) * 4 + 3] = 0xFF;
//        }
//    }
//}