#include "Cpu.h"
#include "Bus.h"

Cpu::Cpu()
{
	opcodes = { 
		{&Cpu::BRK,&Cpu::IMP,7},{&Cpu::ORA,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ORA,&Cpu::ZP,3}, {&Cpu::ASL,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PHP,&Cpu::IMP,3},{&Cpu::ORA,&Cpu::IMM,2},{&Cpu::ASL,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ORA,&Cpu::ABS,4}, {&Cpu::ASL,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BPL,&Cpu::REL,2},{&Cpu::ORA,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ORA,&Cpu::XZP,4},{&Cpu::ASL,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLC,&Cpu::IMP,2},{&Cpu::ORA,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ORA,&Cpu::XABS,4},{&Cpu::ASL,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::JSR,&Cpu::ABS,6},{&Cpu::AND,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::BIT,&Cpu::ZP,3}, {&Cpu::AND,&Cpu::ZP,3}, {&Cpu::ROL,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PLP,&Cpu::IMP,4},{&Cpu::AND,&Cpu::IMM,2},{&Cpu::ROL,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::BIT,&Cpu::ABS,4}, {&Cpu::AND,&Cpu::ABS,4}, {&Cpu::ROL,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BMI,&Cpu::REL,2},{&Cpu::AND,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::AND,&Cpu::XZP,4},{&Cpu::ROL,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SEC,&Cpu::IMP,2},{&Cpu::AND,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::AND,&Cpu::XABS,4},{&Cpu::ROL,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::RTI,&Cpu::IMP,6},{&Cpu::EOR,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::EOR,&Cpu::ZP,3}, {&Cpu::LSR,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PHA,&Cpu::IMP,3},{&Cpu::EOR,&Cpu::IMM,2},{&Cpu::LSR,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::JMP,&Cpu::ABS,3}, {&Cpu::EOR,&Cpu::ABS,4}, {&Cpu::LSR,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BVC,&Cpu::REL,2},{&Cpu::EOR,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::EOR,&Cpu::XZP,4},{&Cpu::LSR,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLI,&Cpu::IMP,2},{&Cpu::EOR,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::EOR,&Cpu::XABS,4},{&Cpu::LSR,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::RTS,&Cpu::IMP,6},{&Cpu::ADC,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ADC,&Cpu::ZP,3}, {&Cpu::ROR,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PLA,&Cpu::IMP,4},{&Cpu::ADC,&Cpu::IMM,2},{&Cpu::ROR,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::JMP,&Cpu::ABSI,5},{&Cpu::ADC,&Cpu::ABS,4}, {&Cpu::ROR,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BVS,&Cpu::REL,2},{&Cpu::ADC,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ADC,&Cpu::XZP,4},{&Cpu::ROR,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SEI,&Cpu::IMP,2},{&Cpu::ADC,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ADC,&Cpu::XABS,4},{&Cpu::ROR,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STA,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::ZP,3}, {&Cpu::STA,&Cpu::ZP,3}, {&Cpu::STX,&Cpu::ZP,3}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::DEY,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TXA,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::ABS,4}, {&Cpu::STA,&Cpu::ABS,4}, {&Cpu::STX,&Cpu::ABS,4}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BCC,Cpu::REL,2} ,{&Cpu::STA,&Cpu::YZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::XZP,4},{&Cpu::STA,&Cpu::XZP,4},{&Cpu::STX,&Cpu::YZP,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TYA,&Cpu::IMP,2},{&Cpu::STA,&Cpu::YABS,5},{&Cpu::TXS,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::STA,&Cpu::XABS,5},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::LDY,&Cpu::IMM,2},{&Cpu::LDA,&Cpu::XZPI,6},{&Cpu::LDX,&Cpu::IMM,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::ZP,3}, {&Cpu::LDA,&Cpu::ZP,3}, {&Cpu::LDX,&Cpu::ZP,3}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TAY,&Cpu::IMP,2},{&Cpu::LDA,&Cpu::IMM,2},{&Cpu::TAX,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::ABS,4}, {&Cpu::LDA,&Cpu::ABS,4}, {&Cpu::LDX,&Cpu::ABS,4}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BCS,&Cpu::REL,2},{&Cpu::LDA,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::XZP,4},{&Cpu::LDA,&Cpu::XZP,4},{&Cpu::LDX,&Cpu::YZP,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLV,&Cpu::IMP,2},{&Cpu::LDA,&Cpu::YABS,4},{&Cpu::TSX,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::XABS,4},{&Cpu::LDA,&Cpu::XABS,4},{&Cpu::LDX,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::CPY,&Cpu::IMM,2},{&Cpu::CMP,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPY,&Cpu::ZP,3}, {&Cpu::CMP,&Cpu::ZP,3}, {&Cpu::DEC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INY,&Cpu::IMP,2},{&Cpu::CMP,&Cpu::IMM,2},{&Cpu::DEX,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPY,&Cpu::ABS,4}, {&Cpu::CMP,&Cpu::ABS,4}, {&Cpu::DEC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BNE,&Cpu::REL,2},{&Cpu::CMP,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CMP,&Cpu::XZP,4},{&Cpu::DEC,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLD,&Cpu::IMP,2},{&Cpu::CMP,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::CMP,&Cpu::XABS,4},{&Cpu::DEC,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::CPX,&Cpu::IMM,2},{&Cpu::SBC,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ZP,3}, {&Cpu::SBC,&Cpu::ZP,3}, {&Cpu::INC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INX,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::IMM,2},{&Cpu::ERR,&Cpu::ERR,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ABS,4}, {&Cpu::SBC,&Cpu::ABS,4}, {&Cpu::INC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BEQ,&Cpu::REL,2},{&Cpu::SBC,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SBC,&Cpu::XZP,4},{&Cpu::INC,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SED,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::SBC,&Cpu::XABS,4},{&Cpu::INC,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	};
}

void Cpu::connectBus(Bus* ptr)
{
	bus = ptr; 
}

void Cpu::write(uint16_t address, uint8_t data)
{
	bus->write(address, data);
}

uint8_t Cpu::read(uint16_t address)
{
	return bus->read(address);
}

//address modes
uint8_t IMP()
{
	return 0;
}
uint8_t ACC()
{
	return 0;
}
uint8_t IMM()
{
	return 0;
}
uint8_t ABS()
{
	return 0;
}
uint8_t XABS()
{
	return 0;
}
uint8_t YABS()
{
	return 0;
}
uint8_t ABSI()
{
	return 0;
}
uint8_t ZP()
{
	return 0;
}
uint8_t XZP()
{
	return 0;
}
uint8_t YZP()
{
	return 0;
}
uint8_t XZPI()
{
	return 0;
}
uint8_t YZPI()
{
	return 0;
}
uint8_t REL()
{
	return 0;
}

//instructions
//Load
uint8_t LDA()
{
	return 0;
}
uint8_t LDX()
{
	return 0;
}
uint8_t LDY()
{
	return 0;
}
uint8_t STA()
{
	return 0;
}
uint8_t STX()
{
	return 0;
}
uint8_t STY()
{
	return 0;
}
//Trans
uint8_t TAX()
{
	return 0;
}
uint8_t TAY()
{
	return 0;
}
uint8_t TSX()
{
	return 0;
}
uint8_t TXA()
{
	return 0;
}
uint8_t TXS()
{
	return 0;
}
uint8_t TYA()
{
	return 0;
}
//Stacks
uint8_t PHA()
{
	return 0;
}
uint8_t PHP()
{
	return 0;
}
uint8_t PLA()
{
	return 0;
}
uint8_t PLP()
{
	return 0;
}
//Shift
uint8_t ASL()
{
	return 0;
}
uint8_t LSR()
{
	return 0;
}
uint8_t ROL()
{
	return 0;
}
uint8_t ROR()
{
	return 0;
}
//logic
uint8_t AND()
{
	return 0;
}
uint8_t BIT()
{
	return 0;
}
uint8_t EOR()
{
	return 0;
}
uint8_t ORA()
{
	return 0;
}
//arithmetic
uint8_t ADC()
{
	return 0;
}
uint8_t CMP()
{
	return 0;
}
uint8_t CPX()
{
	return 0;
}
uint8_t CPY()
{
	return 0;
}
uint8_t SBC()
{
	return 0;
}
//Inc
uint8_t DEC()
{
	return 0;
}
uint8_t DEX()
{
	return 0;
}
uint8_t DEY()
{
	return 0;
}
uint8_t INC()
{
	return 0;
}
uint8_t INX()
{
	return 0;
}
uint8_t INY()
{
	return 0;
}
//Ctrl
uint8_t BRK()
{
	return 0;
}
uint8_t JMP()
{
	return 0;
}
uint8_t JSR()
{
	return 0;
}
uint8_t RTI()
{
	return 0;
}
uint8_t RTS()
{
	return 0;
}
//Bra
uint8_t BCC()
{
	return 0;
}
uint8_t BCS()
{
	return 0;
}
uint8_t BEQ()
{
	return 0;
}
uint8_t BMI()
{
	return 0;
}
uint8_t BNE()
{
	return 0;
}
uint8_t BPL()
{
	return 0;
}
uint8_t BVC()
{
	return 0;
}
uint8_t BVS()
{
	return 0;
}
//Flags
uint8_t CLC()
{
	return 0;
}
uint8_t CLD()
{
	return 0;
}
uint8_t CLI()
{
	return 0;
}
uint8_t CLV()
{
	return 0;
}
uint8_t SEC()
{
	return 0;
}
uint8_t SED()
{
	return 0;
}
uint8_t SEI()
{
	return 0;
}
//Error
uint8_t ERR()
{
	return 0;
}