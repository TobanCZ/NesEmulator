#include "Cartrige.h"
#include <fstream> 
#include "Mapper_001.h"

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
        hw_mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;
    }
    fileType = 1;
    if ((header.mapper2 & 0x0C) == 0x08) fileType = 2;

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

    if (fileType == 2)
    {
        PRGbanks = ((header.PRGRamSize & 0x07) << 8) | header.PRGsize;
        mPRG.resize(PRGbanks * 16384);
        ifs.read((char*)mPRG.data(), mPRG.size());

        CHRbanks = ((header.PRGRamSize & 0x38) << 8) | header.CHRsize;
        mCHR.resize(CHRbanks * 8192);
        ifs.read((char*)mCHR.data(), mCHR.size());
    }

    switch (mapperId)
    {
    case 0:mapper = std::make_shared<Mapper_000>(PRGbanks, CHRbanks); break;
    case 1:mapper = std::make_shared<Mapper_001>(PRGbanks, CHRbanks); break;
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
        if (mapped_addr == 0xFFFFFFFF)
        {
            return true;
        }
        else
        {
            mPRG[mapped_addr] = data;
        }
        return true;
    }
    else
        return false;
}

bool Cartrige::CpuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped_addr = 0;
    if (mapper->CpuRead(address, mapped_addr, data))
    {
        if (mapped_addr == 0xFFFFFFFF)
        {
            return true;
        }
        else
        {
            data = mPRG[mapped_addr];
        }
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

void Cartrige::reset()
{
    if (mapper != nullptr)
        mapper->reset();
}

MIRROR Cartrige::Mirror()
{
    MIRROR m = mapper->mirror();
    if (m == MIRROR::HARDWARE)
    {
        return hw_mirror;
    }
    else
    {
        return m;
    }
}