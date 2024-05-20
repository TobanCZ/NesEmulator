#include "Cartrige.h"
#include <fstream> 

Cartrige::Cartrige(const std::string path) //https://www.nesdev.org/wiki/INES
{
    std::ifstream ifs;
    ifs.open(path, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.read((char*)&header, sizeof(HEADER));

        mapperId = (header.mapper2 & 0xF0) | (header.mapper1 >> 4);   //https://www.nesdev.org/wiki/INES#Flags_7
    }

    fileType = 1;

    if (fileType == 1)  //https://www.nesdev.org/wiki/INES#iNES_file_format
    {
        PRGbanks = header.PRGsize;
        mPRG.resize(PRGbanks * 16384);
        ifs.read((char*)mPRG.data(), mPRG.size());

        CHRbanks = header.CHRsize;
        mCHR.resize(CHRbanks * 8192);
        ifs.read((char*)mCHR.data(), mCHR.size());
    }

    switch (mapperId)
    {
    case 0:mapper = &Mapper_000(PRGbanks,CHRbanks);
    }

    ifs.close();
}

void Cartrige::CpuWrite(uint16_t address, uint8_t data)
{
    mPRG[mapper->CpuWrite(address)] = data;
}

uint8_t Cartrige::CpuRead(uint16_t address)
{
    return mPRG[mapper->CpuRead(address)];
}

void Cartrige::PpuWrite(uint16_t address, uint8_t data)
{
    mCHR[mapper->PpuWrite(address)] = data;
}

uint8_t Cartrige::PpuRead(uint16_t address)
{
    return mCHR[mapper->PpuRead(address)];
}
