#include <iostream>
#include <sstream>
#include "Bus.h"


std::string hex(uint32_t n, uint8_t d)
{
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};



int main()
{

	Bus nes;

	std::stringstream ss;
	ss << "A2 80 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
	uint16_t nOffset = 0x8000;
	while (!ss.eof())
	{
		std::string b;
		ss >> b;
		nes.ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
	}

	// Set Reset Vector
	nes.ram[0xFFFC] = 0x00;
	nes.ram[0xFFFD] = 0x80;

	nes.cpu.reset();


		for (int i = 0; i < 1000; i++)
		{
			while( true)
			{
				std::cout << (uint16_t)nes.cpu.rA;
				do
				{
					nes.cpu.clock();

				} while (!nes.cpu.complete());
			}
		}
	return 0;
}