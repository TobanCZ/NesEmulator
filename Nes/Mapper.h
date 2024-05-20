#pragma once
#include <cstdint>
class Mapper
{
public:
	Mapper(uint8_t PRGbanks,uint8_t CHRbanks);
	~Mapper();


public:
	virtual uint32_t CpuWrite(uint16_t address);
	virtual uint32_t CpuRead(uint16_t address);
	virtual uint32_t PpuWrite(uint16_t address);
	virtual uint32_t PpuRead(uint16_t address);

private:
	uint8_t PRGbanks = 0;
	uint8_t CHRbanks = 0;
};

