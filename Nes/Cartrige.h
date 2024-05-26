#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <memory>

#include "Mapper_000.h"

class Cartrige
{
public:
	Cartrige(const std::string path);
	~Cartrige();


	bool vertical;
	bool bImageValid = false;

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

	std::shared_ptr<Mapper_000> mapper;

public:
	bool CpuWrite(uint16_t address, uint8_t data);
	bool CpuRead(uint16_t address, uint8_t& data);

	bool PpuWrite(uint16_t address, uint8_t data);
	bool PpuRead(uint16_t address, uint8_t& data);
};

