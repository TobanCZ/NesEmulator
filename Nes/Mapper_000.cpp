#include "Mapper_000.h"
//https://www.nesdev.org/wiki/NROM



uint32_t Mapper_000::CpuWrite(uint16_t address)
{
	return address & (PRGbanks > 1 ? 0x7FFF : 0x3FFF);
}

uint32_t Mapper_000::CpuRead(uint16_t address)
{
	return address & (PRGbanks > 1 ? 0x7FFF : 0x3FFF);
}

uint32_t Mapper_000::PpuWrite(uint16_t address)
{
	if (CHRbanks == 0)
	{
		return address;
	}
}

uint32_t Mapper_000::PpuRead(uint16_t address)
{
	return address;
}
