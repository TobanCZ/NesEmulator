#pragma once
#include <array>
class Ppu
{
public:
	Ppu();
	~Ppu();

public:
	std::array<uint8_t, 8192> pattern;
	std::array<uint8_t, 2048> name;
	std::array<uint8_t,32> pallet;

private:
	std::array<uint32_t, 16*4> colorPallet;

public:
	void CpuWrite(uint16_t address, uint8_t data);
	uint8_t CpuRead(uint16_t address, bool readOnly = false);

	void PpuWrite(uint16_t address, uint8_t data);
	uint8_t PpuRead(uint16_t address, bool readOnly = false);

public:
	void connectCartrige(const Cartrige* cartrige);
	void clock();

private:
	Cartrige* cartrige;
};
