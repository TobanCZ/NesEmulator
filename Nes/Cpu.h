#pragma once
#include <cstdint>
#include "Bus.h"
#include <string>
#include <vector>

class Cpu
{
public:
	Cpu();

public:
	void connectBus(Bus* ptr);

public:
	enum Flags
	{
		C = 0,	//Carry
		Z = 1,	//Zero
		I = 2,	//Disable Interrupts
		D = 3,	//Decimal
		B = 4,	//Break
		U = 5,	//Unused
		V = 6,	//Overflow
		N = 7,	//Negative
	};

	uint8_t rA = 0x00;  //register Accumulator
	uint8_t rX = 0x00;  //register X
	uint8_t rY = 0x00;	//register Y
	uint8_t status = 0x00;	//register status

	uint16_t pc = 0x0000;	//program counter
	uint8_t sp = 0x00;		//stack pointer


	uint8_t data = 0x00; //data se kteryma se pracuej
	uint16_t address = 0x00; //adressa se kteryma se pracuje
	int8_t addressRel = 0x00; //branch relativni

private:
	//address modes
	uint8_t IMP();
	uint8_t ACC();
	uint8_t IMM();
	uint8_t ABS();
	uint8_t XABS();
	uint8_t YABS();
	uint8_t ABSI();
	uint8_t ZP();
	uint8_t XZP();
	uint8_t YZP();
	uint8_t XZPI();
	uint8_t YZPI();
	uint8_t REL();

	//instructions
	//Load
	uint8_t LDA();
	uint8_t LDX();
	uint8_t LDY();
	uint8_t STA();
	uint8_t STX();
	uint8_t STY();
	//Trans
	uint8_t TAX();
	uint8_t TAY();
	uint8_t TSX();
	uint8_t TXA();
	uint8_t TXS();
	uint8_t TYA();
	//Stacks
	uint8_t PHA();
	uint8_t PHP();
	uint8_t PLA();
	uint8_t PLP();
	//Shift
	uint8_t ASL();
	uint8_t LSR();
	uint8_t ROL();
	uint8_t ROR();
	//logic
	uint8_t AND();
	uint8_t BIT();
	uint8_t EOR();
	uint8_t ORA();
	//arithmetic
	uint8_t ADC();
	uint8_t CMP();
	uint8_t CPX();
	uint8_t CPY();
	uint8_t SBC();
	//Inc
	uint8_t DEC();
	uint8_t DEX();
	uint8_t DEY();
	uint8_t INC();
	uint8_t INX();
	uint8_t INY();
	//Ctrl
	uint8_t BRK();
	uint8_t JMP();
	uint8_t JSR();
	uint8_t RTI();
	uint8_t RTS();
	//Bra
	uint8_t BCC();
	uint8_t BCS();
	uint8_t BEQ();
	uint8_t BMI();
	uint8_t BNE();
	uint8_t BPL();
	uint8_t BVC();
	uint8_t BVS();
	//Flags
	uint8_t CLC();
	uint8_t CLD();
	uint8_t CLI();
	uint8_t CLV();
	uint8_t SEC();
	uint8_t SED();
	uint8_t SEI();
	//Error
	uint8_t ERR();

private:	
	void write(uint16_t address, uint8_t data);
	uint8_t read(uint16_t address);

	void setFlag(Flags flag, bool state);
	bool getFlag(Flags flag);


private:
	Bus* bus = nullptr;

	struct OPCODE
	{
		uint8_t(Cpu::* instruction)(void) = nullptr;
		uint8_t(Cpu::* addressMode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	std::vector<OPCODE> opcodes;
};

