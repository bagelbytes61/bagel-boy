#include "lcd_controller.h"

#include "bus.h"
#include "io_registers.h"
#include "utilities.h"

#include "Graphics\Buffer.h"
#include "Graphics\GraphicsDevice.h"
#include "Graphics\Shader.h"
#include "Graphics\Texture.h"

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static const float display_vertices[] =
{
	-1.0f, -1.0f, 0.5f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.5f, 0.0f, 0.0f,
	1.0f,  1.0f, 0.5f, 1.0f, 0.0f,

	-1.0f, -1.0f, 0.5f, 0.0f, 1.0f,
	1.0f,  1.0f, 0.5f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.5f, 1.0f, 1.0f
};

static uint8_t background_texture_data[256 * 256 * 4];

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

	lcd_controller->device = device;

	lcd_controller->display_vertex_buffer = L3D_CreateBuffer(lcd_controller->device, 6, 20, L3D_eBuffer_Vertex, L3D_eUsage_Static);
	lcd_controller->vertex_shader = L3D_CreateShader(lcd_controller->device, "shaders\\gameboy.vs", L3D_eShader_Vertex);
	lcd_controller->pixel_shader = L3D_CreateShader(lcd_controller->device, "shaders\\gameboy.ps", L3D_eShader_Pixel);
	lcd_controller->vertex_layout = L3D_CreateVertexLayout(lcd_controller->device, L3D_GetShaderBytecode(lcd_controller->vertex_shader), L3D_GetShaderBytecodeLength(lcd_controller->vertex_shader));
	lcd_controller->background_texture = L3D_CreateTexture(lcd_controller->device, 256, 256, L3D_eTexture_Texture2D, L3D_eTextureBindFlag_ShaderResourceView, L3D_eUsage_Dynamic);

	lcd_controller->interrupt_controller = interrupt_controller;

	lcd_controller->bus = bus;

	memset(background_texture_data, 255, sizeof(background_texture_data));

	L3D_UpdateGPUObject(lcd_controller->device, lcd_controller->display_vertex_buffer, display_vertices);

	L3D_SetVertexBuffers(lcd_controller->device, 0, 1, &lcd_controller->display_vertex_buffer);
	L3D_SetVertexShader(lcd_controller->device, lcd_controller->vertex_shader);
	L3D_SetPixelShader(lcd_controller->device, lcd_controller->pixel_shader);
	L3D_SetVertexLayout(lcd_controller->device, lcd_controller->vertex_layout);
	L3D_SetTextures(lcd_controller->device, 0, 1, &lcd_controller->background_texture);
}

void lcd_controller_deinitialize(struct lcd_controller* lcd_controller)
{
	
}

void lcd_controller_update(struct lcd_controller* lcd_controller, const uint64_t* clock_cycles)
{
/*	if (*clock_cycles % 456 >= LCD_MODE_10_CLOCK_CYCLE_BEGIN && *clock_cycles % 456 <= LCD_MODE_10_CLOCK_CYCLE_END)
	{
		lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_10;
	}
	else if (*clock_cycles % 456 >= LCD_MODE_11_CLOCK_CYCLE_BEGIN && *clock_cycles % 456 <= LCD_MODE_11_CLOCK_CYCLE_END)
	{
		lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_11;
	}
	else if (*clock_cycles % 456 >= LCD_MODE_00_CLOCK_CYCLE_BEGIN && *clock_cycles % 456 <= LCD_MODE_00_CLOCK_CYCLE_END)
	{
		lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_00;
	}
	else */if (*clock_cycles % 456 == 0)
	{
		if (lcd_controller->ly >= 144 && lcd_controller->ly <= 153)
		{
			lcd_controller->stat = (lcd_controller->stat & ~0x03) | LCD_MODE_01;

			if (lcd_controller->ly == 144)
			{
				const uint8_t color_pallette[4] = { 0x00, 0x0F, 0xF0, 0xFF };

				for (uint16_t y = 0; y < 256; y++)
				{
				    for (uint16_t x = 0; x < 256; x++)
				    {
				        uint8_t tile_index_value = lcd_controller->vram[0x1800 + (y / 8) * 32 + (x / 8)];

				        uint16_t vram_index = tile_index_value * 16 + (y % 8) * 2 + (x / 4) % 2;

				        uint8_t tile_pixel_value = (lcd_controller->vram[vram_index] >>  (6 - ((x % 4) * 2))) & 0x03;
				        
				        background_texture_data[(y * 256 + x) * 4 + 0] = color_pallette[tile_pixel_value];
				        background_texture_data[(y * 256 + x) * 4 + 1] = color_pallette[tile_pixel_value];
				        background_texture_data[(y * 256 + x) * 4 + 2] = color_pallette[tile_pixel_value];
				        background_texture_data[(y * 256 + x) * 4 + 3] = 0xFF;
				    }
				}

				L3D_UpdateGPUObject(lcd_controller->device, lcd_controller->background_texture, background_texture_data);

				L3D_ClearRenderTarget(lcd_controller->device, NULL);

				L3D_Draw(lcd_controller->device, L3D_ePrimitiveTopology_TriangleList, 6, 0);

				L3D_Present(lcd_controller->device);
			}
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