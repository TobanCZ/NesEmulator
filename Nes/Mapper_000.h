#pragma once
#include "Mapper.h"
class Mapper_000 :public Mapper
{
public:
	Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper_000();

public:
	bool CpuWrite(uint16_t address , uint32_t& mapped_address, uint8_t data = 0) override;
	bool CpuRead(uint16_t address, uint32_t& mapped_address, uint8_t& data) override;
	bool PpuWrite(uint16_t address, uint32_t& mapped_address) override;
	bool PpuRead(uint16_t address, uint32_t& mapped_address) override;

	void reset() override;
};

