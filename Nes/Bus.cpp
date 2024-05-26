#include "Bus.h"

Bus::Bus()
{
	cpu.connectBus(this);
}

Bus::~Bus()
{
}

void Bus::CpuWrite(uint16_t address, uint8_t data)
{
	if (cartrige->CpuWrite(address, data))
	{}
	else if (address >= 0x0000 && address <= 0x1FFF)
		ram[address & 0x07FF] = data;
	else if (address >= 0x2000 && address <= 0x3FFF)
		ppu.CpuWrite(address & 0x0007, data);
	//else if (address >= 0x4016 && address <= 0x4017)
	//	//dodleat 
}

uint8_t Bus::CpuRead(uint16_t address, bool readOnly)
{
	uint8_t data = 0x00;

	if(cartrige->CpuRead(address,data))
	{ }
	else if (address >= 0x0000 && address <= 0x1FFF)
		data = ram[address & 0x07FF];
	else if (address >= 0x2000 && address <= 0x3FFF)
		data = ppu.CpuRead(address & 0x0007, readOnly);
//dodelat

	return data;
}

void Bus::insertCartrige(const std::shared_ptr<Cartrige>& cartrige)
{
	this->cartrige = cartrige;
	ppu.connectCartrige(cartrige);
}

void Bus::reset()
{
	cpu.reset();
	ppu.Reset();
	ram.fill(0);
	clockCount = 0;
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();
	if (nSystemClockCounter % 3 == 0)
	{
		cpu.clock();
	}

	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nmi();
	}
	nSystemClockCounter++;
}
