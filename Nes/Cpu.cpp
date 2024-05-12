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
		{&Cpu::BCC,&Cpu::REL,2} ,{&Cpu::STA,&Cpu::YZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::XZP,4},{&Cpu::STA,&Cpu::XZP,4},{&Cpu::STX,&Cpu::YZP,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TYA,&Cpu::IMP,2},{&Cpu::STA,&Cpu::YABS,5},{&Cpu::TXS,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::STA,&Cpu::XABS,5},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::LDY,&Cpu::IMM,2},{&Cpu::LDA,&Cpu::XZPI,6},{&Cpu::LDX,&Cpu::IMM,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::ZP,3}, {&Cpu::LDA,&Cpu::ZP,3}, {&Cpu::LDX,&Cpu::ZP,3}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TAY,&Cpu::IMP,2},{&Cpu::LDA,&Cpu::IMM,2},{&Cpu::TAX,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::ABS,4}, {&Cpu::LDA,&Cpu::ABS,4}, {&Cpu::LDX,&Cpu::ABS,4}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BCS,&Cpu::REL,2},{&Cpu::LDA,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::XZP,4},{&Cpu::LDA,&Cpu::XZP,4},{&Cpu::LDX,&Cpu::YZP,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLV,&Cpu::IMP,2},{&Cpu::LDA,&Cpu::YABS,4},{&Cpu::TSX,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::XABS,4},{&Cpu::LDA,&Cpu::XABS,4},{&Cpu::LDX,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::CPY,&Cpu::IMM,2},{&Cpu::CMP,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPY,&Cpu::ZP,3}, {&Cpu::CMP,&Cpu::ZP,3}, {&Cpu::DEC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INY,&Cpu::IMP,2},{&Cpu::CMP,&Cpu::IMM,2},{&Cpu::DEX,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPY,&Cpu::ABS,4}, {&Cpu::CMP,&Cpu::ABS,4}, {&Cpu::DEC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BNE,&Cpu::REL,2},{&Cpu::CMP,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CMP,&Cpu::XZP,4},{&Cpu::DEC,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLD,&Cpu::IMP,2},{&Cpu::CMP,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::CMP,&Cpu::XABS,4},{&Cpu::DEC,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::CPX,&Cpu::IMM,2},{&Cpu::SBC,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ZP,3}, {&Cpu::SBC,&Cpu::ZP,3}, {&Cpu::INC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INX,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::IMM,2},{&Cpu::ERR,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ABS,4}, {&Cpu::SBC,&Cpu::ABS,4}, {&Cpu::INC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
		{&Cpu::BEQ,&Cpu::REL,2},{&Cpu::SBC,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SBC,&Cpu::XZP,4},{&Cpu::INC,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SED,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::SBC,&Cpu::XABS,4},{&Cpu::INC,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	};
}

void Cpu::connectBus(Bus *ptr)
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
		status &= ~(1 << flag);
}

bool Cpu::getFlag(Flags flag)
{
	return (status >> flag) & 0x01;
}

bool Cpu::complete()
{
	return cycles == 0;
}

void Cpu::clock()
{
	if (cycles == 0)
	{
		currentOpcode = read(pc);
		setFlag(U, 1);
		cycles = opcodes[currentOpcode].cycles;
		cycles += (this->*opcodes[currentOpcode].addressMode)();
		cycles += (this->*opcodes[currentOpcode].instruction)();
		pc++;
	}
	else
	{
		cycles--;
	}
}

void Cpu::reset()
{
	rA = 0;
	rX = 0;
	rY = 0;
	status = 0;
	sp = 0xFD;
	address = 0;
	addressRel = 0;
	data = 0;

	uint16_t low = read(0xFFFC);
	uint16_t high = read(0xFFFC + 1);

	pc = (high << 8) | low;

	cycles = 8;
}

void Cpu::irq()
{
	if (getFlag(I) == 0)
	{
		write(0x0100 + sp, pc & 0xFF00);
		sp--;
		write(0x0100 + sp, pc & 0x00FF);
		sp--;

		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + sp, status);
		sp--;

		uint16_t low = read(0xFFFE);
		uint16_t high = read(0xFFFE + 1);
		pc = (high << 8) | low;

		cycles = 7;
	}
}

void Cpu::nmi()
{
	write(0x0100 + sp, pc & 0xFF00);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	setFlag(B, 0);
	setFlag(U, 1);
	setFlag(I, 1);
	write(0x0100 + sp, status);
	sp--;

	uint16_t low = read(0xFFFA);
	uint16_t high = read(0xFFFA + 1);
	pc = (high << 8) | low;

	cycles = 7;
}

//address modes
uint8_t Cpu::IMP()
{
	return 0;
}
uint8_t Cpu::ACC()
{
	data = rA;
	return 0;
}
uint8_t Cpu::IMM()
{
	pc++;
	data = read(pc);
	address = pc;
	return 0; 
}
uint8_t Cpu::ABS()
{
	pc++;
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);

	uint16_t addr = (high << 8) | low;
	data = read(addr);
	address = addr;
	return 0;
}
uint8_t Cpu::XABS()
{
	pc++;
	uint16_t low = read(pc);
	pc++;
	uint16_t high = read(pc);

	uint16_t addr = ((high << 8) | low) + rX;
	data = read(addr);
	address = addr;

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
	data = read(addr);
	address = addr;

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

	data = read(addr); //tady si taky nejsem jisty
	address = addr;

	return 0;
}
uint8_t Cpu::ZP()
{
	pc++;
	uint8_t addr = read(pc);
	addr = 0x00FF & addr;
	data = read(addr);
	address = addr;
	return 0;
}
uint8_t Cpu::XZP()
{
	pc++;
	uint8_t addr = read(pc);
	addr = 0x00FF & (addr + rX);
	data = read(addr);
	address = addr;
	return 0;
}
uint8_t Cpu::YZP()
{
	pc++;
	uint8_t addr = read(pc);
	addr = 0x00FF & (addr + rY);
	data = read(addr);
	address = addr;
	return 0;
}
uint8_t Cpu::XZPI() //X-Indexed Zero Page Indirect
{
	pc++;
	uint8_t addr = read(pc);

	uint8_t low = read((addr + rX) & 0x00FF);
	uint8_t high = read((addr + rX + 1) & 0x00FF);

	addr = (high << 8) | low;
	data = read(addr);
	address = addr;

	return 0;
}
uint8_t Cpu::YZPI() //Zero Page Indirect Y-Indexed
{
	pc++;
	uint8_t addr = read(pc);

	uint8_t low = read(addr & 0x00FF);
	uint8_t high = read((addr + 1) & 0x00FF);

	addr = (high << 8) | low;
	data = read(addr) + rY;
	address = addr;

	if (high != (uint16_t)(data >> 8)) //tady si nejsem jisty
		return 1;

	return 0;
}
uint8_t Cpu::REL()
{
	pc++;	
	addressRel = read(pc);

	return 0;
}


//instructions
//Load
uint8_t Cpu::LDA()
{
	rA = data;

	setFlag(Z,(rA == 0x00));
	setFlag(N,(rA & 0x80));

	return 0;
}
uint8_t Cpu::LDX()
{
	rX = data;

	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));

	return 0;
}
uint8_t Cpu::LDY()
{
	rY = data;

	setFlag(Z, (rY == 0x00));
	setFlag(N, (rY & 0x80));

	return 0;
}
uint8_t Cpu::STA()
{
	write(address, rA);
	return 0;
}
uint8_t Cpu::STX()
{
	write(address, rX);
	return 0;
}
uint8_t Cpu::STY()
{
	write(address, rY);
	return 0;
}
//Trans
uint8_t Cpu::TAX()
{
	rX = rA;
	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));
	return 0;
}
uint8_t Cpu::TAY()
{
	rY = rA;
	setFlag(Z, (rY == 0x00));
	setFlag(N, (rY & 0x80));
	return 0;
}
uint8_t Cpu::TSX()
{
	rX = sp;
	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));
	return 0;
}
uint8_t Cpu::TXA()
{
	rA = rX;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
uint8_t Cpu::TXS()
{
	sp = rX;
	return 0;
}
uint8_t Cpu::TYA()
{
	rA = rY;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
//Stacks
uint8_t Cpu::PHA()
{
	write(0x0100 + sp, rA);
	sp--;
	return 0;
}
uint8_t Cpu::PHP()
{
	write(0x0100 + sp, status);
	sp--;
	return 0;
}
uint8_t Cpu::PLA()
{
	sp++;
	rA = read(0x0100 + sp);
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
uint8_t Cpu::PLP()
{
	sp++;
	status = read(0x0100 + sp);
	return 0;
}
//Shift
uint8_t Cpu::ASL()
{
	uint8_t carry = data & 0x80;
	uint8_t shift = data << 1;
	setFlag(C, carry > 0);
	setFlag(Z, (shift == 0x00));
	setFlag(N, (shift & 0x80));
	if (opcodes[currentOpcode].addressMode == &Cpu::ACC)
		rA = shift;
	else
		write(address, shift);
	return 0;
}
uint8_t Cpu::LSR()
{
	uint8_t carry = data & 0x01;
	uint8_t shift = data >> 1;
	setFlag(C, carry > 0);
	setFlag(Z, (shift == 0x00));
	setFlag(N, (shift & 0x80));
	if (opcodes[currentOpcode].addressMode == &Cpu::ACC)
		rA = shift;
	else
		write(address, shift);
	return 0;
}
uint8_t Cpu::ROL()
{
	uint8_t carry = data & 0x80;
	uint8_t shift = data << 1;
	shift |= getFlag(C);
	setFlag(C, carry > 0);
	setFlag(Z, (shift == 0x00));
	setFlag(N, (shift & 0x80));
	if (opcodes[currentOpcode].addressMode == &Cpu::ACC)
		rA = shift;
	else
		write(address, shift);
	return 0;
}
uint8_t Cpu::ROR()
{
	uint8_t carry = data & 0x01;
	uint8_t shift = data >> 1;
	shift |= getFlag(C) << 7;
	setFlag(C, carry > 0);
	setFlag(Z, (shift == 0x00));
	setFlag(N, (shift & 0x80));
	if (opcodes[currentOpcode].addressMode == &Cpu::ACC)
		rA = shift;
	else
		write(address, shift);
	return 0;
}
//logic
uint8_t Cpu::AND()
{
	rA &= data;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
uint8_t Cpu::BIT()
{
	setFlag(Z, ((rA & data) == 0x00));
	setFlag(N, data & (1 << 7));
	setFlag(V, data & (1 << 6));
	return 0;
}
uint8_t Cpu::EOR()
{
	rA ^= data;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
uint8_t Cpu::ORA()
{
	rA |= data;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
//arithmetic
uint8_t Cpu::ADC()
{
	uint16_t result = rA + data + getFlag(C);
	setFlag(C, (result > 255) || (getFlag(D) && result > 99));
	setFlag(V, (~(rA ^ data) & (rA ^ result)) & 0x0080);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	rA = result & 0x00FF;
	return 0;
}
uint8_t Cpu::CMP()
{
	uint16_t result = rA - data;
	setFlag(C, data <= rA);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::CPX()
{
	uint16_t result = rX - data;
	setFlag(C, data <= rX);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::CPY()
{
	uint16_t result = rY - data;
	setFlag(C, data <= rY);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::SBC()
{
	uint16_t result = rA + ~data + getFlag(C);
	setFlag(C, (result > 255) || (getFlag(D) && result > 99));
	setFlag(V, (~(rA ^ ~data) & (rA ^ result)) & 0x0080);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	rA = result & 0x00FF;
	return 0;
}
//Inc
uint8_t Cpu::DEC()
{
	uint8_t result = data - 1;
	write(address, result);
	setFlag(Z, (result == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::DEX()
{
	rX--;
	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));
	return 0;
}
uint8_t Cpu::DEY()
{
	rY--;
	setFlag(Z, (rY == 0x00));
	setFlag(N, (rY & 0x80));
	return 0;
}
uint8_t Cpu::INC()
{
	uint8_t result = data + 1;
	write(address, result);
	setFlag(Z, (result == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::INX()
{
	rX++;
	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));
	return 0;
}
uint8_t Cpu::INY()
{
	rY++;
	setFlag(Z, (rY == 0x00));
	setFlag(N, (rY & 0x80));
	return 0;
}
//Ctrl
uint8_t Cpu::BRK()
{
	setFlag(I, 1);
	write(0x0100 + sp, pc & 0xFF00);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;
	setFlag(B, 1);
	write(0x0100 + sp, status);
	setFlag(B, 0);
	sp--;

	pc++;

	pc = read(0xFFFE) | (read(0xFFFF) << 8);
	return 0;
}
uint8_t Cpu::JMP()
{
	pc = address;
	return 0;
}
uint8_t Cpu::JSR()
{

	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	pc = address;
	return 0;
}
uint8_t Cpu::RTI()
{
	sp++;
	status = read(0x0100 + sp);


	sp++;
	uint8_t low = read(0x0100 + sp);
	sp++;
	uint8_t high = read(0x0100 + sp);

	pc = (high << 8) | low;
	return 0;
}
uint8_t Cpu::RTS()
{
	sp++;
	uint8_t low = read(0x0100 + sp);
	sp++;
	uint8_t high = read(0x0100 + sp);

	pc = (high << 8) | low;
	pc++;
	return 0;
}
//Bra
uint8_t Cpu::BCC()
{
	if (!getFlag(C))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BCS()
{
	if (getFlag(C))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BEQ()
{
	if (getFlag(Z))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BMI()
{
	if (getFlag(N))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BNE()
{
	if (!getFlag(Z))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BPL()
{
	if (!getFlag(N))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BVC()
{
	if (!getFlag(V))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
uint8_t Cpu::BVS()
{
	if (getFlag(V))
	{
		pc = addressRel + pc;
		return 1;
	}
	return 0;
}
//Flags
uint8_t Cpu::CLC()
{
	setFlag(C, false);
	return 0;
}
uint8_t Cpu::CLD()
{
	setFlag(D, false);
	return 0;
}
uint8_t Cpu::CLI()
{
	setFlag(I, false);
	return 0;
}
uint8_t Cpu::CLV()
{
	setFlag(V, false);
	return 0;
}
uint8_t Cpu::SEC()
{
	setFlag(C, true);
	return 0;
}
uint8_t Cpu::SED()
{
	setFlag(D, true);
	return 0;
}
uint8_t Cpu::SEI()
{
	setFlag(I, true);
	return 0;
}
//Error
uint8_t Cpu::ERR()
{
	return 0;
}