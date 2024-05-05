#pragma once
#include <cstdint>
#include "Cpu.h"
#include <array>

class Bus
{
public:
	Bus();

public: //pripojene zarizeni
	Cpu cpu;
	std::array<uint8_t, 64 * 1024> ram;

public: //funkce
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address, bool readOnly = false);
};

