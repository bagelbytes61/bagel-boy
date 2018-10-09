#ifndef BAGEL_BOY_H_
#define BAGEL_BOY_H_

#pragma once

#include "bus.h"
#include "cpu.h"
#include "cartridge.h"
#include "interrupt_controller.h"
#include "io_registers.h"

#include <stdbool.h>

struct bagelboy {
    struct cartridge cartridge;

    struct interrupt_controller interrupt_controller;
    
    struct bus bus;

    struct cpu cpu;

    struct io_registers io_registers;

    bool power;
};

void bagelboy_initialize(struct bagelboy* bagelboy, const char* filename);
void bagelboy_destroy(struct bagelboy* bagelboy);

void bagelboy_emulate(struct bagelboy* bagelboy);

#endif























////struct cartridge* cartridge;
////
////struct msc* msc;
////
////vram_t vram[0x2000];
////wram_t wram_bank_0[0x1000];
////wram_t wram_bank_1[0x1000];
////hram_t hram[0x80];
////
////struct interrupt_controller* interrupt_controller;
////
////struct bus* bus;
////
////struct cpu* cpu;