#pragma once
#include "Mapper.h"
class Mapper_000 :public Mapper
{
public:
	using Mapper::Mapper;

public:
	uint32_t CpuWrite(uint16_t address) override;
	uint32_t CpuRead(uint16_t address) override;
	uint32_t PpuWrite(uint16_t address) override;
	uint32_t PpuRead(uint16_t address) override;

private:
	uint8_t PRGbanks = 0;
	uint8_t CHRbanks = 0;
};

