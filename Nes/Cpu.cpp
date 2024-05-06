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
		{&Cpu::CPX,&Cpu::IMM,2},{&Cpu::SBC,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ZP,3}, {&Cpu::SBC,&Cpu::ZP,3}, {&Cpu::INC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INX,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::IMM,2},{&Cpu::ERR,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ABS,4}, {&Cpu::SBC,&Cpu::ABS,4}, {&Cpu::INC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
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

void Cpu::setFlag(Flags flag, bool state)
{
	if (state)
		status |= (1 << flag); 
	else
		status &= (0 << flag);
}

bool Cpu::getFlag(Flags flag)
{
	return (status >> flag) & 0x01;
}

//address modes
uint8_t Cpu::IMP()
{
	return 0;
}
uint8_t Cpu::ACC()
{
	fetched = rA;
	return 0;
}
uint8_t Cpu::IMM()
{
	pc++;
	fetched = read(pc);
	adress = pc;
	return 0;
}
uint8_t Cpu::ABS()
{
	pc++;
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);

	uint16_t addr = (high << 8) | low;
	fetched = read(addr);
	adress = addr;
	return 0;
}
uint8_t Cpu::XABS()
{
	pc++;
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);

	uint16_t addr = ((high << 8) | low) + rX;
	fetched = read(addr);
	adress = addr;

	if (high != (addr >> 8)) //tady si nejsem jisty
		return 1;

	return 0;
}
uint8_t Cpu::YABS()
{
	pc++;
	uint8_t low = read(pc);
	pc++;
	uint8_t high = read(pc);

	uint16_t addr = ((high << 8) | low) + rY;
	fetched = read(addr);
	adress = addr;

	if (high != (addr >> 8)) //tady si nejsem jisty
		return 1;

	return 0;
}
uint8_t Cpu::ABSI()
{
	pc++;
	uint8_t low = read(pc);
	pc++;
	uint8_t high = read(pc);

	uint16_t addr = ((high << 8) | low) + rY;

	addr = (read(addr + 1) << 8) | read(addr);

	fetched = read(addr); //tady si taky nejsem jisty
	adress = addr;

	return 0;
}
uint8_t Cpu::ZP()
{
	pc++;
	uint8_t adrr = read(pc);
	adrr = 0x00FF & adrr;
	fetched = read(adrr);
	adress = adrr;
	return 0;
}
uint8_t Cpu::XZP()
{
	pc++;
	uint8_t adrr = read(pc);
	adrr = 0x00FF & (adrr + rX);
	fetched = read(adrr);
	adress = adrr;
	return 0;
}
uint8_t Cpu::YZP()
{
	pc++;
	uint8_t adrr = read(pc);
	adrr = 0x00FF & (adrr + rY);
	fetched = read(adrr);
	adress = adrr;
	return 0;
}
uint8_t Cpu::XZPI() //X-Indexed Zero Page Indirect
{
	pc++;
	uint8_t adrr = read(pc);

	uint8_t low = read((adrr + rX) & 0x00FF);
	uint8_t high = read((adrr + rX + 1) & 0x00FF);

	adrr = (high << 8) | low;
	fetched = read(adrr);
	adress = adrr;

	return 0;
}
uint8_t Cpu::YZPI() //Zero Page Indirect Y-Indexed
{
	pc++;
	uint8_t adrr = read(pc);

	uint8_t low = read(adrr & 0x00FF);
	uint8_t high = read((adrr + 1) & 0x00FF);

	adrr = (high << 8) | low;
	fetched = read(adrr) + rY;
	adress = adrr;

	if (high != (fetched >> 8)) //tady si nejsem jisty
		return 1;

	return 0;
}
uint8_t Cpu::REL()
{
	pc++;	
	adressRel = read(pc);

	return 0;
}



//instructions
//Load
uint8_t Cpu::LDA()
{
	rA = fetched;

	setFlag(Z,(rA == 0x00));
	setFlag(N,(rA & 0x80));

	return 0;
}
uint8_t Cpu::LDX()
{
	rX = fetched;

	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));

	return 0;
}
uint8_t Cpu::LDY()
{
	rY = fetched;

	setFlag(Z, (rY == 0x00));
	setFlag(N, (rY & 0x80));

	return 0;
}
uint8_t Cpu::STA()
{
	write(adress, rA);
	return 0;
}
uint8_t Cpu::STX()
{
	write(adress, rX);
	return 0;
}
uint8_t Cpu::STY()
{
	write(adress, rY);
	return 0;
}
//Trans
uint8_t Cpu::TAX()
{
	return 0;
}
uint8_t Cpu::TAY()
{
	return 0;
}
uint8_t Cpu::TSX()
{
	return 0;
}
uint8_t Cpu::TXA()
{
	return 0;
}
uint8_t Cpu::TXS()
{
	return 0;
}
uint8_t Cpu::TYA()
{
	return 0;
}
//Stacks
uint8_t Cpu::PHA()
{
	return 0;
}
uint8_t Cpu::PHP()
{
	return 0;
}
uint8_t Cpu::PLA()
{
	return 0;
}
uint8_t Cpu::PLP()
{
	return 0;
}
//Shift
uint8_t Cpu::ASL()
{
	return 0;
}
uint8_t Cpu::LSR()
{
	return 0;
}
uint8_t Cpu::ROL()
{
	return 0;
}
uint8_t Cpu::ROR()
{
	return 0;
}
//logic
uint8_t Cpu::AND()
{
	return 0;
}
uint8_t Cpu::BIT()
{
	return 0;
}
uint8_t Cpu::EOR()
{
	return 0;
}
uint8_t Cpu::ORA()
{
	return 0;
}
//arithmetic
uint8_t Cpu::ADC()
{
	return 0;
}
uint8_t Cpu::CMP()
{
	return 0;
}
uint8_t Cpu::CPX()
{
	return 0;
}
uint8_t Cpu::CPY()
{
	return 0;
}
uint8_t Cpu::SBC()
{
	return 0;
}
//Inc
uint8_t Cpu::DEC()
{
	return 0;
}
uint8_t Cpu::DEX()
{
	return 0;
}
uint8_t Cpu::DEY()
{
	return 0;
}
uint8_t Cpu::INC()
{
	return 0;
}
uint8_t Cpu::INX()
{
	return 0;
}
uint8_t Cpu::INY()
{
	return 0;
}
//Ctrl
uint8_t Cpu::BRK()
{
	return 0;
}
uint8_t Cpu::JMP()
{
	return 0;
}
uint8_t Cpu::JSR()
{
	return 0;
}
uint8_t Cpu::RTI()
{
	return 0;
}
uint8_t Cpu::RTS()
{
	return 0;
}
//Bra
uint8_t Cpu::BCC()
{
	return 0;
}
uint8_t Cpu::BCS()
{
	return 0;
}
uint8_t Cpu::BEQ()
{
	return 0;
}
uint8_t Cpu::BMI()
{
	return 0;
}
uint8_t Cpu::BNE()
{
	return 0;
}
uint8_t Cpu::BPL()
{
	return 0;
}
uint8_t Cpu::BVC()
{
	return 0;
}
uint8_t Cpu::BVS()
{
	return 0;
}
//Flags
uint8_t Cpu::CLC()
{
	return 0;
}
uint8_t Cpu::CLD()
{
	return 0;
}
uint8_t Cpu::CLI()
{
	return 0;
}
uint8_t Cpu::CLV()
{
	return 0;
}
uint8_t Cpu::SEC()
{
	return 0;
}
uint8_t Cpu::SED()
{
	return 0;
}
uint8_t Cpu::SEI()
{
	return 0;
}
//Error
uint8_t Cpu::ERR()
{
	return 0;
}