#pragma once
#include <cstdint>
#include <array>
#include "Cpu.h"
#include "Ppu.h"
#include "Cartrige.h"

class Bus
{
public:
	Bus();
	~Bus();

public: 
	Cpu cpu;
	Ppu ppu;
	std::shared_ptr<Cartrige> cartrige;
	std::array<uint8_t, 2 * 1024> ram;

public: //funkce
	void CpuWrite(uint16_t address, uint8_t data);
	uint8_t CpuRead(uint16_t address, bool readOnly = false);

public: //system
	void insertCartrige(const std::shared_ptr<Cartrige>& cartrige);
	void reset();
	void clock();

private:
	uint32_t clockCount;
	uint32_t nSystemClockCounter = 0;
};

