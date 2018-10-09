#include "io_registers.h"

uint8_t io_registers_on_read(struct io_registers* io_registers, uint16_t address)
{
    switch (address) {
        case IO_REGISTER_P1:
            break;
        case IO_REGISTER_SB:
            break;
        case IO_REGISTER_SC:
    }
}

void io_registers_on_write(struct io_registers* io_registers, uint16_t address, uint8_t value)
{

}