#include "Mapper.h"

Mapper::Mapper(uint8_t PRGbanks, uint8_t CHRbanks)
{
	this->PRGbanks = PRGbanks;
	this->CHRbanks = CHRbanks;

	reset();
}

Mapper::~Mapper()
{
}

void Mapper::reset()
{

}

MIRROR Mapper::mirror()
{
	return MIRROR::HARDWARE;
}