#ifndef IO_REGISTERS_H_
#define IO_REGISTERS_H_

#pragma once

#include <stdint.h>

#define IO_REGISTER_P1              0xFF00  // joy pad
                                            // bit 5 ~ p15 out port
                                            // bit 4 ~ p14 out port
                                            // bit 3 ~ p13 in port
                                            // bit 2 ~ p12 in port
                                            // bit 1 ~ p11 in port
                                            // bit 0 ~ p10 in port

#define IO_REGISTER_SB              0xFF01  // serial transfer data

#define IO_REGISTER_SC              0xFF02  // sio control
                                            // bit 7 ~ transfer start flag
                                            //  - 0 ~ non transfer
                                            //  - 1 ~ start transfer
                                            // bit 0 ~ shift clock
                                            //  - 0 ~ external clock
                                            //  - 1 ~ internal clock

#define IO_REGISTER_DIV             0xFF04  // divider

#define IO_REGISTER_TIMA            0xFF05  // timer counter

#define IO_REGISTER_TMA             0xFF06  // timer modulo

#define IO_REGISTER_TAC             0xFF07  // timer control
                                            // bit 2 ~ timer stop
                                            //  - 0 ~ stop timer
                                            //  - 1 ~ start timer
                                            // bit 1 - 0 ~ input clock select
                                            //  - 00 ~ 4.096 KHz
                                            //  - 01 ~ 262.144 KHz
                                            //  - 10 ~ 65.536 KHz
                                            //  - 11 ~ 16.384 KHz

#define IO_REGISTER_IF              0xFF0F  // interrupt flag
                                            // bit 4 ~ transition from high to low of pin p10 - p13
                                            // bit 3 ~ serial i/o transfer complete
                                            // bit 2 ~ timer overflow
                                            // bit 1 ~ lcdc
                                            // bit 0 ~ v-blank

#define IO_REGISTER_NR_10           0xFF10
#define IO_REGISTER_NR_11           0xFF11
#define IO_REGISTER_NR_12           0xFF12
#define IO_REGISTER_NR_13           0xFF13
#define IO_REGISTER_NR_14           0xFF14
#define IO_REGISTER_NR_21           0xFF16
#define IO_REGISTER_NR_22           0xFF17
#define IO_REGISTER_NR_23           0xFF18
#define IO_REGISTER_NR_24           0xFF19
#define IO_REGISTER_NR_30           0xFF1A
#define IO_REGISTER_NR_31           0xFF1B
#define IO_REGISTER_NR_32           0xFF1C
#define IO_REGISTER_NR_33           0xFF1D
#define IO_REGISTER_NR_34           0xFF1E
#define IO_REGISTER_NR_41           0xFF20
#define IO_REGISTER_NR_42           0xFF21
#define IO_REGISTER_NR_43           0xFF22
#define IO_REGISTER_NR_44           0xFF23
#define IO_REGISTER_NR_50           0xFF24
#define IO_REGISTER_NR_51           0xFF25
#define IO_REGISTER_NR_52           0xFF26

//#define IO_REGISTER_WAVE_PATTER_RAM 0xFF

#define IO_REGISTER_LCDC            0xFF40
#define IO_REGISTER_STAT            0xFF41
#define IO_REGISTER_SCY             0xFF42
#define IO_REGISTER_SCX             0xFF43
#define IO_REGISTER_LY              0xFF44
#define IO_REGISTER_LYC             0xFF45

#define IO_REGISTER_DMA             0xFF46

#define IO_REGISTER_BGP             0xFF47
#define IO_REGISTER_OBP0            0xFF48
#define IO_REGISTER_OBP1            0xFF49
#define IO_REGISTER_WY              0xFF4A
#define IO_REGISTER_WX              0xFF4B

struct io_registers {
    uint8_t p1;

    uint8_t sb, sc;

    uint8_t div;

    uint8_t tima, tma, tac;

    uint8_t interrupt_flag;

    uint8_t lcdc;
    uint8_t stat;

    uint8_t scy, scx;
    uint8_t ly;
    uint8_t lyc;

    uint8_t dma;

    uint8_t bgp;
    uint8_t obp0, obp1;
    uint8_t wy, wx;

    uint8_t ff50;
};

uint8_t io_registers_on_read(struct io_registers* io_registers, uint16_t address);
void io_registers_on_write(struct io_registers* io_registers, uint16_t address, uint8_t value);

#endif