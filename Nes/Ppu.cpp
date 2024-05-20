#include "Ppu.h"
#include <cstdint>
#include "Cartrige.h"

void Ppu::CpuWrite(uint16_t address, uint8_t data)
{
}

uint8_t Ppu::CpuRead(uint16_t address, bool readOnly)
{
    uint8_t data = 0x00;
    return data;
}

void Ppu::PpuWrite(uint16_t address, uint8_t data)
{
}

uint8_t Ppu::PpuRead(uint16_t address, bool readOnly)
{
    return 0;
}

void Ppu::connectCartrige(const Cartrige& cartrige)
{
    this->cartrige = cartrige;
}
