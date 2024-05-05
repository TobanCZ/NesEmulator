#include "Bus.h"

Bus::Bus()
{
	cpu.connectBus(this);
}

void Bus::write(uint16_t address, uint8_t data)
{
	if(address >= 0x0000 && address <= 0xFFFF)
		ram[address] = data;
}

uint8_t Bus::read(uint16_t address, bool readOnly)
{
	if (address >= 0x0000 && address <= 0xFFFF)
		return ram[address];
}