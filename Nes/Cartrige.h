#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <array>

#include "Mapper_000.h"

class Cartrige
{
public:
	Cartrige(const std::string path);
	~Cartrige();

private:
	struct HEADER
	{
		std::array<uint8_t, 4> name;
		uint8_t PRGsize;
		uint8_t CHRsize;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t PRGRamSize;
		uint8_t TVsystem1;
		uint8_t TVsystem2;
		std::array<uint8_t, 5> unused;
	};

	HEADER header;

	std::vector<uint8_t> mPRG;
	std::vector<uint8_t> mCHR;

	uint8_t fileType = 0;
	uint8_t mapperId = 0;
	uint8_t PRGbanks = 0;
	uint8_t CHRbanks = 0;

	Mapper* mapper;

public:
	void CpuWrite(uint16_t address, uint8_t data);
	uint8_t CpuRead(uint16_t address);

	void PpuWrite(uint16_t address, uint8_t data);
	uint8_t PpuRead(uint16_t address);
};

