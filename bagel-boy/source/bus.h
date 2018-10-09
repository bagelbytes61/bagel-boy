#ifndef BUS_H_
#define BUS_H_

#pragma once

#include <stdint.h>

#define HARDWARE                                    11

#define ROM_BANK_0                                  0
#define ROM_BANK_0_BEGIN_ADDRESS                    0x0000
#define ROM_BANK_0_END_ADDRESS                      0x4000

#define ROM_BANK_N                                  1
#define ROM_BANK_N_BEGIN_ADDRESS                    0x4000
#define ROM_BANK_N_END_ADDRESS                      0x8000

#define VRAM                                        2
#define VRAM_BEGIN_ADDRESS                          0x8000
#define VRAM_END_ADDRESS                            0xA000

#define EXTERNAL_RAM                                3
#define EXTERNAL_RAM_BEGIN_ADDRESS                  0xA000
#define EXTERNAL_RAM_END_ADDRESS                    0xC000

#define WRAM_BANK_0                                 4
#define WRAM_BANK_0_BEGIN_ADDRESS                   0xC000
#define WRAM_BANK_0_END_ADDRESS                     0xD000

#define WRAM_BANK_1                                 5
#define WRAM_BANK_1_BEGIN_ADDRESS                   0xD000
#define WRAM_BANK_1_END_ADDRESS                     0xE000

#define ECHO                                        6
#define ECHO_BEGIN_ADDRESS                          0xE000
#define ECHO_END_ADDRESS                            0xFE00

#define OAM                                         7
#define OAM_BEGIN_ADDRESS                           0xFE00
#define OAM_END_ADDRESS                             0xFEA0

#define IO_REGISTERS                                8
#define IO_REGISTERS_BEGIN_ADDRESS                  0xFF00
#define IO_REGISTERS_END_ADDRESS                    0xFF51

#define HRAM                                        9
#define HRAM_BEGIN_ADDRESS                          0xFF80
#define HRAM_END_ADDRESS                            0xFFFF

#define IER                                         10
#define IER_BEGIN_ADDRESS                           0xFFFF
#define IER_END_ADDRESS                             0x10000

struct hardware {
    void* hardware;

    uint16_t begin;
    uint16_t end;

    uint8_t(*on_read)(void*, uint16_t);
    void(*on_write)(void*, uint16_t, uint8_t);
};

struct bus {
    struct hardware hardware_map[HARDWARE];
};

struct bus* bus_create(struct bagelboy* bagelboy);
void bus_destroy(struct bus* bus);

uint8_t bus_read(struct bus* bus, uint16_t address);
void bus_write(struct bus* bus, uint16_t address, uint8_t value);

#endif