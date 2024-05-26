#include "Mapper_000.h"
//https://www.nesdev.org/wiki/NROM

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
}


Mapper_000::~Mapper_000()
{
}


bool Mapper_000::CpuWrite(uint16_t address, uint32_t& mapped_address, uint8_t data)
{
	if (address >= 0x8000 && address <= 0xFFFF)
	{
		mapped_address = address & (PRGbanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper_000::CpuRead(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x8000 && address <= 0xFFFF)
	{
		mapped_address = address & (PRGbanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper_000::PpuWrite(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		if (CHRbanks == 0)
		{
			mapped_address = address;
		}
	}
	return false;
}

bool Mapper_000::PpuRead(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		mapped_address = address;
		return true;
	}
	return false;
}
