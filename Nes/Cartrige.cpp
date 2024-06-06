#include "Cartrige.h"
#include <fstream> 

Cartrige::Cartrige(const std::string path) //https://www.nesdev.org/wiki/INES
{
    std::ifstream ifs;
    ifs.open(path, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.read((char*)&header, sizeof(HEADER));

        if (header.mapper1 & 0x04)
            ifs.seekg(512, std::ios_base::cur);

        mapperId = (header.mapper2 & 0xF0) | (header.mapper1 >> 4);   //https://www.nesdev.org/wiki/INES#Flags_7
        vertical = (header.mapper1 & 0x01) ? true : false;
    }

    fileType = 1;

    if (fileType == 1)  //https://www.nesdev.org/wiki/INES#iNES_file_format
    {
        PRGbanks = header.PRGsize;
        if (PRGbanks < 2) PRGbanks = 2;
        mPRG.resize(PRGbanks * 16384);
        ifs.read((char*)mPRG.data(), mPRG.size());

        CHRbanks = header.CHRsize;
        if(CHRbanks == 0)
            mCHR.resize(8192);
        else
            mCHR.resize(CHRbanks * 8192);
        ifs.read((char*)mCHR.data(), mCHR.size());
    }

    switch (mapperId)
    {
    case 0:mapper = std::make_shared<Mapper_000>(PRGbanks, CHRbanks); break;
    }

    ifs.close();
    bImageValid = true;
}

Cartrige::~Cartrige()
{
}

bool Cartrige::CpuWrite(uint16_t address, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if (mapper->CpuWrite(address, mapped_addr, data))
    {
        mPRG[mapped_addr] = data;
        return true;
    }
    else
        return false;
}

bool Cartrige::CpuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped_addr = 0;
    if (mapper->CpuRead(address, mapped_addr))
    {
        data = mPRG[mapped_addr];
        return true;
    }
    else
        return false;
}
bool Cartrige::PpuWrite(uint16_t address, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if (mapper->PpuWrite(address, mapped_addr))
    {
        mCHR[mapped_addr] = data;
        return true;
    }
    else
        return false;
}

bool Cartrige::PpuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped_addr = 0;
    if (mapper->PpuRead(address, mapped_addr))
    {
        data = mCHR[mapped_addr];
        return true;
    }
    else
        return false;
}
