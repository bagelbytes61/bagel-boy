#include "bus.h"

#include "bagelboy.h"
#include "hram.h"
#include "msc.h"
#include "oam.h"
#include "vram.h"

#include <memory.h>
#include <stdlib.h>

void bus_initialize(struct bus* bus, struct bagelboy* bagelboy);
void bus_deinitialize(struct bus* bus);

struct bus* bus_create(struct bagelboy* bagelboy)
{
   struct bus* bus = malloc(sizeof(struct bus));

   bus_initialize(bus, bagelboy);

   return bus;
}

void bus_destroy(struct bus* bus)
{
   free(bus);
}

void bus_initialize(struct bus* bus, struct bagelboy* bagelboy)
{
   memset(bus, 0, sizeof(struct bus));

   bus->hardware_map[ROM_BANK_0].hardware = bagelboy->msc;
   bus->hardware_map[ROM_BANK_0].begin = ROM_BANK_0_BEGIN_ADDRESS;
   bus->hardware_map[ROM_BANK_0].end = ROM_BANK_0_END_ADDRESS;
   bus->hardware_map[ROM_BANK_0].on_read = &msc_on_read;
   bus->hardware_map[ROM_BANK_0].on_write = &msc_on_write;

   bus->hardware_map[ROM_BANK_N].hardware = bagelboy->msc;
   bus->hardware_map[ROM_BANK_N].begin = ROM_BANK_0_BEGIN_ADDRESS;
   bus->hardware_map[ROM_BANK_N].end = ROM_BANK_0_END_ADDRESS;
   bus->hardware_map[ROM_BANK_N].on_read = &msc_on_read;
   bus->hardware_map[ROM_BANK_N].on_write = &msc_on_write;

   bus->hardware_map[VRAM].hardware = bagelboy->vram;
   bus->hardware_map[VRAM].begin = VRAM_BEGIN_ADDRESS;
   bus->hardware_map[VRAM].end = VRAM_END_ADDRESS;
   bus->hardware_map[VRAM].on_read = &vram_on_read;
   bus->hardware_map[VRAM].on_write = &vram_on_write;

   bus->hardware_map[WRAM_BANK_0].hardware = bagelboy->wram_bank_0;
   bus->hardware_map[WRAM_BANK_0].begin = WRAM_BANK_0_BEGIN_ADDRESS;
   bus->hardware_map[WRAM_BANK_0].end = WRAM_BANK_0_END_ADDRESS;
   bus->hardware_map[WRAM_BANK_0].on_read = &wram_on_read;
   bus->hardware_map[WRAM_BANK_0].on_write = &wram_on_write;

   bus->hardware_map[WRAM_BANK_1].hardware = bagelboy->wram_bank_1;
   bus->hardware_map[WRAM_BANK_1].begin = WRAM_BANK_0_BEGIN_ADDRESS;
   bus->hardware_map[WRAM_BANK_1].end = WRAM_BANK_0_END_ADDRESS;
   bus->hardware_map[WRAM_BANK_1].on_read = &wram_on_read;
   bus->hardware_map[WRAM_BANK_1].on_write = &wram_on_write;

   bus->hardware_map[ECHO].hardware = NULL;
   bus->hardware_map[ECHO].begin = WRAM_BANK_0_BEGIN_ADDRESS;
   bus->hardware_map[ECHO].end = WRAM_BANK_0_END_ADDRESS;
   bus->hardware_map[ECHO].on_read = &wram_on_read;
   bus->hardware_map[ECHO].on_write = &wram_on_write;

   bus->hardware_map[OAM].hardware = NULL;
   bus->hardware_map[OAM].begin = OAM_BEGIN_ADDRESS;
   bus->hardware_map[OAM].end = OAM_END_ADDRESS;
   bus->hardware_map[OAM].on_read = &oam_on_read;
   bus->hardware_map[OAM].on_write = &oam_on_write;

   bus->hardware_map[IO_REGISTERS].hardware = bagelboy->

   bus->hardware_map[HRAM].hardware = bagelboy->hram;
   bus->hardware_map[HRAM].begin = HRAM_BEGIN_ADDRESS;
   bus->hardware_map[HRAM].end = HRAM_END_ADDRESS;
   bus->hardware_map[HRAM].on_read = &hram_on_read;
   bus->hardware_map[HRAM].on_write = &hram_on_write;

   bus->hardware_map[IER].hardware;
   bus->hardware_map[IER].begin = IER_BEGIN_ADDRESS;
   bus->hardware_map[IER].end = IER_END_ADDRESS;
   bus->hardware_map[IER].on_read = NULL;
   bus->hardware_map[IER].on_write = NULL;
}

void bus_deinitialize(struct bus* bus)
{

}

uint8_t bus_read(struct bus* bus, uint16_t address)
{
   uint8_t hardware_index = 0;
   while (address >= bus->hardware_map[hardware_index].end) { hardware_index++; }

   void* hardware = bus->hardware_map[hardware_index].hardware;

   uint16_t offset = bus->hardware_map[hardware_index].begin;

   return bus->hardware_map[hardware_index].on_read(hardware, address - offset);
}

void bus_write(struct bus* bus, uint16_t address, uint8_t value)
{
   uint8_t hardware_index = 0;
   while (address >= bus->hardware_map[hardware_index].end) { hardware_index++; }

   void* hardware = bus->hardware_map[hardware_index].hardware;

   uint16_t offset = bus->hardware_map[hardware_index].begin;

   bus->hardware_map[hardware_index].on_write(hardware, address - offset, value);
}