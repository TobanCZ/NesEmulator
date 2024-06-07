#include "Mapper_001.h"

Mapper_001::Mapper_001(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
    vRAMStatic.resize(32 * 1024);
}

Mapper_001::~Mapper_001()
{
}

bool Mapper_001::CpuWrite(uint16_t address, uint32_t& mapped_address, uint8_t data)
{
	if (address >= 0x6000 && address <= 0x7FFF)
	{
		mapped_address = 0xFFFFFFFF;
		vRAMStatic[address & 0x1FFF] = data;
		return true;
	}

	if (address >= 0x8000)
	{
		if (data & 0x80)
		{
			nLoadRegister = 0x00;
			nLoadRegisterCount = 0;
			nControlRegister = nControlRegister | 0x0C;
		}
		else
		{
			nLoadRegister >>= 1;
			nLoadRegister |= (data & 0x01) << 4;
			nLoadRegisterCount++;

			if (nLoadRegisterCount == 5)
			{
				uint8_t nTargetRegister = (address >> 13) & 0x03;

				if (nTargetRegister == 0) // 0x8000 - 0x9FFF
				{
					nControlRegister = nLoadRegister & 0x1F;

					switch (nControlRegister & 0x03)
					{
					case 0:	mirrormode = ONESCREEN_LO; break;
					case 1: mirrormode = ONESCREEN_HI; break;
					case 2: mirrormode = VERTICAL;     break;
					case 3:	mirrormode = HORIZONTAL;   break;
					}
				}
				else if (nTargetRegister == 1) // 0xA000 - 0xBFFF
				{
					if (nControlRegister & 0b10000)
					{
						nCHRBankSelect4Lo = nLoadRegister & 0x1F;
					}
					else
					{
						nCHRBankSelect8 = nLoadRegister & 0x1E;
					}
				}
				else if (nTargetRegister == 2) // 0xC000 - 0xDFFF
				{
					if (nControlRegister & 0b10000)
					{
						nCHRBankSelect4Hi = nLoadRegister & 0x1F;
					}
				}
				else if (nTargetRegister == 3) // 0xE000 - 0xFFFF
				{
					uint8_t nPRGMode = (nControlRegister >> 2) & 0x03;

					if (nPRGMode == 0 || nPRGMode == 1)
					{
						nPRGBankSelect32 = (nLoadRegister & 0x0E) >> 1;
					}
					else if (nPRGMode == 2)
					{
						nPRGBankSelect16Lo = 0;
						nPRGBankSelect16Hi = nLoadRegister & 0x0F;
					}
					else if (nPRGMode == 3)
					{
						nPRGBankSelect16Lo = nLoadRegister & 0x0F;
						nPRGBankSelect16Hi = PRGbanks - 1;
					}
				}
				nLoadRegister = 0x00;
				nLoadRegisterCount = 0;
			}

		}

	}
	return false;
}

bool Mapper_001::CpuRead(uint16_t address, uint32_t& mapped_address, uint8_t& data)
{
	if (address >= 0x6000 && address <= 0x7FFF)
	{
		mapped_address = 0xFFFFFFFF;
		data = vRAMStatic[address & 0x1FFF];
		return true;
	}

	if (address >= 0x8000)
	{
		if (nControlRegister & 0b01000)
		{
			if (address >= 0x8000 && address <= 0xBFFF)
			{
				mapped_address = nPRGBankSelect16Lo * 0x4000 + (address & 0x3FFF);
				return true;
			}

			if (address >= 0xC000 && address <= 0xFFFF)
			{
				mapped_address = nPRGBankSelect16Hi * 0x4000 + (address & 0x3FFF);
				return true;
			}
		}
		else
		{
			mapped_address = nPRGBankSelect32 * 0x8000 + (address & 0x7FFF);
			return true;
		}
	}
	return false;
}

bool Mapper_001::PpuWrite(uint16_t address, uint32_t& mapped_address)
{
	if (address < 0x2000)
	{
		if (CHRbanks == 0)
		{
			mapped_address = address;
			return true;
		}

		return true;
	}
	else
		return false;
}

bool Mapper_001::PpuRead(uint16_t address, uint32_t& mapped_address)
{
	if (address < 0x2000)
	{
		if (CHRbanks == 0)
		{
			mapped_address = address;
			return true;
		}
		else
		{
			if (nControlRegister & 0b10000)
			{
				if (address >= 0x0000 && address <= 0x0FFF)
				{
					mapped_address = nCHRBankSelect4Lo * 0x1000 + (address & 0x0FFF);
					return true;
				}

				if (address >= 0x1000 && address <= 0x1FFF)
				{
					mapped_address = nCHRBankSelect4Hi * 0x1000 + (address & 0x0FFF);
					return true;
				}
			}
			else
			{
				mapped_address = nCHRBankSelect8 * 0x2000 + (address & 0x1FFF);
				return true;
			}
		}
	}

	return false;
}

void Mapper_001::reset()
{
	nControlRegister = 0x1C;
	nLoadRegister = 0x00;
	nLoadRegisterCount = 0x00;

	nCHRBankSelect4Lo = 0;
	nCHRBankSelect4Hi = 0;
	nCHRBankSelect8 = 0;

	nPRGBankSelect32 = 0;
	nPRGBankSelect16Lo = 0;
	nPRGBankSelect16Hi = PRGbanks - 1;
}

MIRROR Mapper_001::mirror()
{
	return mirrormode;
}
