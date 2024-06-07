#pragma once
#include <cstdint>

enum MIRROR
{
	HARDWARE,
	HORIZONTAL,
	VERTICAL,
	ONESCREEN_LO,
	ONESCREEN_HI,
};

class Mapper
{
public:
	Mapper(uint8_t PRGbanks,uint8_t CHRbanks);
	~Mapper();


public:
	virtual bool CpuWrite(uint16_t address, uint32_t& mapped_address, uint8_t data) = 0;
	virtual bool CpuRead(uint16_t address , uint32_t& mapped_address, uint8_t& data) = 0;
	virtual bool PpuWrite(uint16_t address, uint32_t& mapped_address) = 0;
	virtual bool PpuRead(uint16_t address, uint32_t& mapped_address) = 0;

	virtual void reset() = 0;
	virtual MIRROR mirror();

protected:
	uint8_t PRGbanks = 0;
	uint8_t CHRbanks = 0;
};

