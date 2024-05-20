#include "Bus.h"

Bus::Bus()
{
	cpu.connectBus(this);
}

void Bus::CpuWrite(uint16_t address, uint8_t data)
{
	if (address >= 0x0000 && address <= 0x1FFF)
		ram[address & 0x07FF] = data;
	else if (address >= 0x2000 && address <= 0x3FFF)
		ppu.CpuWrite(address & 0x07FF, data);
}

uint8_t Bus::CpuRead(uint16_t address, bool readOnly)
{
	if (address >= 0x0000 && address <= 0x1FFF)
		return ram[address & 0x07FF];
	else if (address >= 0x2000 && address <= 0x3FFF)
		return ppu.CpuRead(address & 0x07FF,readOnly);
}

void Bus::insertCartrige(const Cartrige& cartrige)
{
	this->cartrige = cartrige;
	ppu.connectCartrige(cartrige);
}

void Bus::reset()
{
	cpu.reset();
	clockCount = 0;
}

void Bus::clock()
{
}
