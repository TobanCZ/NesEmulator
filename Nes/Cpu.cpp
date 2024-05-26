#include "Cpu.h"
#include "Bus.h"
#include <iostream>
#include <map>

Cpu::Cpu()
{
	//opcodes = { 
	//	{&Cpu::BRK,&Cpu::IMP,7, "BRK"},{&Cpu::ORA,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ORA,&Cpu::ZP,3}, {&Cpu::ASL,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PHP,&Cpu::IMP,3},{&Cpu::ORA,&Cpu::IMM,2},{&Cpu::ASL,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ORA,&Cpu::ABS,4}, {&Cpu::ASL,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BPL,&Cpu::REL,2},{&Cpu::ORA,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ORA,&Cpu::XZP,4},{&Cpu::ASL,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLC,&Cpu::IMP,2},{&Cpu::ORA,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ORA,&Cpu::XABS,4},{&Cpu::ASL,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::JSR,&Cpu::ABS,6},{&Cpu::AND,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::BIT,&Cpu::ZP,3}, {&Cpu::AND,&Cpu::ZP,3}, {&Cpu::ROL,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PLP,&Cpu::IMP,4},{&Cpu::AND,&Cpu::IMM,2},{&Cpu::ROL,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::BIT,&Cpu::ABS,4}, {&Cpu::AND,&Cpu::ABS,4}, {&Cpu::ROL,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BMI,&Cpu::REL,2},{&Cpu::AND,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::AND,&Cpu::XZP,4},{&Cpu::ROL,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SEC,&Cpu::IMP,2},{&Cpu::AND,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::AND,&Cpu::XABS,4},{&Cpu::ROL,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::RTI,&Cpu::IMP,6},{&Cpu::EOR,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::EOR,&Cpu::ZP,3}, {&Cpu::LSR,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PHA,&Cpu::IMP,3},{&Cpu::EOR,&Cpu::IMM,2},{&Cpu::LSR,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::JMP,&Cpu::ABS,3}, {&Cpu::EOR,&Cpu::ABS,4}, {&Cpu::LSR,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BVC,&Cpu::REL,2},{&Cpu::EOR,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::EOR,&Cpu::XZP,4},{&Cpu::LSR,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLI,&Cpu::IMP,2},{&Cpu::EOR,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::EOR,&Cpu::XABS,4},{&Cpu::LSR,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::RTS,&Cpu::IMP,6},{&Cpu::ADC,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ADC,&Cpu::ZP,3}, {&Cpu::ROR,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::PLA,&Cpu::IMP,4},{&Cpu::ADC,&Cpu::IMM,2},{&Cpu::ROR,&Cpu::ACC,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::JMP,&Cpu::ABSI,5},{&Cpu::ADC,&Cpu::ABS,4}, {&Cpu::ROR,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BVS,&Cpu::REL,2},{&Cpu::ADC,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ADC,&Cpu::XZP,4},{&Cpu::ROR,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SEI,&Cpu::IMP,2},{&Cpu::ADC,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ADC,&Cpu::XABS,4},{&Cpu::ROR,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STA,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::ZP,3}, {&Cpu::STA,&Cpu::ZP,3}, {&Cpu::STX,&Cpu::ZP,3}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::DEY,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TXA,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::ABS,4}, {&Cpu::STA,&Cpu::ABS,4}, {&Cpu::STX,&Cpu::ABS,4}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BCC,&Cpu::REL,2},{&Cpu::STA,&Cpu::YZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::STY,&Cpu::XZP,4},{&Cpu::STA,&Cpu::XZP,4},{&Cpu::STX,&Cpu::YZP,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TYA,&Cpu::IMP,2},{&Cpu::STA,&Cpu::YABS,5},{&Cpu::TXS,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::STA,&Cpu::XABS,5},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::LDY,&Cpu::IMM,2},{&Cpu::LDA,&Cpu::XZPI,6},{&Cpu::LDX,&Cpu::IMM,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::ZP,3}, {&Cpu::LDA,&Cpu::ZP,3}, {&Cpu::LDX,&Cpu::ZP,3}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::TAY,&Cpu::IMP,2},{&Cpu::LDA,&Cpu::IMM,2},{&Cpu::TAX,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::ABS,4}, {&Cpu::LDA,&Cpu::ABS,4}, {&Cpu::LDX,&Cpu::ABS,4}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BCS,&Cpu::REL,2},{&Cpu::LDA,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::XZP,4},{&Cpu::LDA,&Cpu::XZP,4},{&Cpu::LDX,&Cpu::YZP,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLV,&Cpu::IMP,2},{&Cpu::LDA,&Cpu::YABS,4},{&Cpu::TSX,&Cpu::IMP,2},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::LDY,&Cpu::XABS,4},{&Cpu::LDA,&Cpu::XABS,4},{&Cpu::LDX,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::CPY,&Cpu::IMM,2},{&Cpu::CMP,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPY,&Cpu::ZP,3}, {&Cpu::CMP,&Cpu::ZP,3}, {&Cpu::DEC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INY,&Cpu::IMP,2},{&Cpu::CMP,&Cpu::IMM,2},{&Cpu::DEX,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPY,&Cpu::ABS,4}, {&Cpu::CMP,&Cpu::ABS,4}, {&Cpu::DEC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BNE,&Cpu::REL,2},{&Cpu::CMP,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CMP,&Cpu::XZP,4},{&Cpu::DEC,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CLD,&Cpu::IMP,2},{&Cpu::CMP,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::CMP,&Cpu::XABS,4},{&Cpu::DEC,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::CPX,&Cpu::IMM,2},{&Cpu::SBC,&Cpu::XZPI,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ZP,3}, {&Cpu::SBC,&Cpu::ZP,3}, {&Cpu::INC,&Cpu::ZP,5}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::INX,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::IMM,2},{&Cpu::ERR,&Cpu::IMP,2}, {&Cpu::ERR,&Cpu::ERR,0},{&Cpu::CPX,&Cpu::ABS,4}, {&Cpu::SBC,&Cpu::ABS,4}, {&Cpu::INC,&Cpu::ABS,6}, {&Cpu::ERR,&Cpu::ERR,0},
	//	{&Cpu::BEQ,&Cpu::REL,2},{&Cpu::SBC,&Cpu::YZPI,5},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SBC,&Cpu::XZP,4},{&Cpu::INC,&Cpu::XZP,6},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::SED,&Cpu::IMP,2},{&Cpu::SBC,&Cpu::YABS,4},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0},{&Cpu::ERR,&Cpu::ERR,0}, {&Cpu::SBC,&Cpu::XABS,4},{&Cpu::INC,&Cpu::XABS,7},{&Cpu::ERR,&Cpu::ERR,0},
	//};

	opcodes = {
	{&Cpu::BRK,&Cpu::IMP,7, "BRK"},{&Cpu::ORA,&Cpu::XZPI,6, "ORA"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ORA,&Cpu::ZP,3, "ORA"}, {&Cpu::ASL,&Cpu::ZP,5, "ASL"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::PHP,&Cpu::IMP,3, "PHP"},{&Cpu::ORA,&Cpu::IMM,2, "ORA"},{&Cpu::ASL,&Cpu::ACC,2, "ASL"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::ORA,&Cpu::ABS,4, "ORA"}, {&Cpu::ASL,&Cpu::ABS,6, "ASL"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BPL,&Cpu::REL,2, "BPL"},{&Cpu::ORA,&Cpu::YZPI,5, "ORA"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ORA,&Cpu::XZP,4, "ORA"},{&Cpu::ASL,&Cpu::XZP,6, "ASL"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CLC,&Cpu::IMP,2, "CLC"},{&Cpu::ORA,&Cpu::YABS,4, "ORA"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::ORA,&Cpu::XABS,4, "ORA"},{&Cpu::ASL,&Cpu::XABS,7, "ASL"},{&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::JSR,&Cpu::ABS,6, "JSR"},{&Cpu::AND,&Cpu::XZPI,6, "AND"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::BIT,&Cpu::ZP,3, "BIT"}, {&Cpu::AND,&Cpu::ZP,3, "AND"}, {&Cpu::ROL,&Cpu::ZP,5, "ROL"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::PLP,&Cpu::IMP,4, "PLP"},{&Cpu::AND,&Cpu::IMM,2, "AND"},{&Cpu::ROL,&Cpu::ACC,2, "ROL"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::BIT,&Cpu::ABS,4, "BIT"}, {&Cpu::AND,&Cpu::ABS,4, "AND"}, {&Cpu::ROL,&Cpu::ABS,6, "ROL"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BMI,&Cpu::REL,2, "BMI"},{&Cpu::AND,&Cpu::YZPI,5, "AND"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::AND,&Cpu::XZP,4, "AND"},{&Cpu::ROL,&Cpu::XZP,6, "ROL"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::SEC,&Cpu::IMP,2, "SEC"},{&Cpu::AND,&Cpu::YABS,4, "AND"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::AND,&Cpu::XABS,4, "AND"},{&Cpu::ROL,&Cpu::XABS,7, "ROL"},{&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::RTI,&Cpu::IMP,6, "RTI"},{&Cpu::EOR,&Cpu::XZPI,6, "EOR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::EOR,&Cpu::ZP,3, "EOR"}, {&Cpu::LSR,&Cpu::ZP,5, "LSR"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::PHA,&Cpu::IMP,3, "PHA"},{&Cpu::EOR,&Cpu::IMM,2, "EOR"},{&Cpu::LSR,&Cpu::ACC,2, "LSR"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::JMP,&Cpu::ABS,3, "JMP"}, {&Cpu::EOR,&Cpu::ABS,4, "EOR"}, {&Cpu::LSR,&Cpu::ABS,6, "LSR"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BVC,&Cpu::REL,2, "BVC"},{&Cpu::EOR,&Cpu::YZPI,5, "EOR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::EOR,&Cpu::XZP,4, "EOR"},{&Cpu::LSR,&Cpu::XZP,6, "LSR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CLI,&Cpu::IMP,2, "CLI"},{&Cpu::EOR,&Cpu::YABS,4, "EOR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::EOR,&Cpu::XABS,4, "EOR"},{&Cpu::LSR,&Cpu::XABS,7, "LSR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::RTS,&Cpu::IMP,6, "RTS"},{&Cpu::ADC,&Cpu::XZPI,6, "ADC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ADC,&Cpu::ZP,3, "ADC"}, {&Cpu::ROR,&Cpu::ZP,5, "ROR"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::PLA,&Cpu::IMP,4, "PLA"},{&Cpu::ADC,&Cpu::IMM,2, "ADC"},{&Cpu::ROR,&Cpu::ACC,2, "ROR"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::JMP,&Cpu::ABSI,5, "JMP"},{&Cpu::ADC,&Cpu::ABS,4, "ADC"}, {&Cpu::ROR,&Cpu::ABS,6, "ROR"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BVS,&Cpu::REL,2, "BVS"},{&Cpu::ADC,&Cpu::YZPI,5, "ADC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ADC,&Cpu::XZP,4, "ADC"},{&Cpu::ROR,&Cpu::XZP,6, "ROR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::SEI,&Cpu::IMP,2, "SEI"},{&Cpu::ADC,&Cpu::YABS,4, "ADC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::ADC,&Cpu::XABS,4, "ADC"},{&Cpu::ROR,&Cpu::XABS,7, "ROR"},{&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::STA,&Cpu::XZPI,6, "STA"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::STY,&Cpu::ZP,3, "STY"}, {&Cpu::STA,&Cpu::ZP,3, "STA"}, {&Cpu::STX,&Cpu::ZP,3, "STX"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::DEY,&Cpu::IMP,2, "DEY"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::TXA,&Cpu::IMP,2, "TXA"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::STY,&Cpu::ABS,4, "STY"}, {&Cpu::STA,&Cpu::ABS,4, "STA"}, {&Cpu::STX,&Cpu::ABS,4, "STX"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BCC,&Cpu::REL,2, "BCC"},{&Cpu::STA,&Cpu::YZPI,6, "STA"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::STY,&Cpu::XZP,4, "STY"},{&Cpu::STA,&Cpu::XZP,4, "STA"},{&Cpu::STX,&Cpu::YZP,4, "STX"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::TYA,&Cpu::IMP,2, "TYA"},{&Cpu::STA,&Cpu::YABS,5, "STA"},{&Cpu::TXS,&Cpu::IMP,2, "TXS"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::STA,&Cpu::XABS,5, "STA"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::LDY,&Cpu::IMM,2, "LDY"},{&Cpu::LDA,&Cpu::XZPI,6, "LDA"},{&Cpu::LDX,&Cpu::IMM,0, "LDX"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::LDY,&Cpu::ZP,3, "LDY"}, {&Cpu::LDA,&Cpu::ZP,3, "LDA"}, {&Cpu::LDX,&Cpu::ZP,3, "LDX"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::TAY,&Cpu::IMP,2, "TAY"},{&Cpu::LDA,&Cpu::IMM,2, "LDA"},{&Cpu::TAX,&Cpu::IMP,2, "TAX"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::LDY,&Cpu::ABS,4, "LDY"}, {&Cpu::LDA,&Cpu::ABS,4, "LDA"}, {&Cpu::LDX,&Cpu::ABS,4, "LDX"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BCS,&Cpu::REL,2, "BCS"},{&Cpu::LDA,&Cpu::YZPI,5, "LDA"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::LDY,&Cpu::XZP,4, "LDY"},{&Cpu::LDA,&Cpu::XZP,4, "LDA"},{&Cpu::LDX,&Cpu::YZP,4, "LDX"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CLV,&Cpu::IMP,2, "CLV"},{&Cpu::LDA,&Cpu::YABS,4, "LDA"},{&Cpu::TSX,&Cpu::IMP,2, "TSX"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::LDY,&Cpu::XABS,4, "LDY"},{&Cpu::LDA,&Cpu::XABS,4, "LDA"},{&Cpu::LDX,&Cpu::YABS,4, "LDX"},{&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::CPY,&Cpu::IMM,2, "CPY"},{&Cpu::CMP,&Cpu::XZPI,6, "CMP"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CPY,&Cpu::ZP,3, "CPY"}, {&Cpu::CMP,&Cpu::ZP,3, "CMP"}, {&Cpu::DEC,&Cpu::ZP,5, "DEC"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::INY,&Cpu::IMP,2, "INY"},{&Cpu::CMP,&Cpu::IMM,2, "CMP"},{&Cpu::DEX,&Cpu::IMP,2, "DEX"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CPY,&Cpu::ABS,4, "CPY"}, {&Cpu::CMP,&Cpu::ABS,4, "CMP"}, {&Cpu::DEC,&Cpu::ABS,6, "DEC"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BNE,&Cpu::REL,2, "BNE"},{&Cpu::CMP,&Cpu::YZPI,5, "CMP"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CMP,&Cpu::XZP,4, "CMP"},{&Cpu::DEC,&Cpu::XZP,6, "DEC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CLD,&Cpu::IMP,2, "CLD"},{&Cpu::CMP,&Cpu::YABS,4, "CMP"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::CMP,&Cpu::XABS,4, "CMP"},{&Cpu::DEC,&Cpu::XABS,7, "DEC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::CPX,&Cpu::IMM,2, "CPX"},{&Cpu::SBC,&Cpu::XZPI,6, "SBC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CPX,&Cpu::ZP,3, "CPX"}, {&Cpu::SBC,&Cpu::ZP,3, "SBC"}, {&Cpu::INC,&Cpu::ZP,5, "INC"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::INX,&Cpu::IMP,2, "INX"},{&Cpu::SBC,&Cpu::IMM,2, "SBC"},{&Cpu::ERR,&Cpu::IMP,2, "???"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::CPX,&Cpu::ABS,4, "CPX"}, {&Cpu::SBC,&Cpu::ABS,4, "SBC"}, {&Cpu::INC,&Cpu::ABS,6, "INC"}, {&Cpu::ERR,&Cpu::ERR,0, "???"},
	{&Cpu::BEQ,&Cpu::REL,2, "BEQ"},{&Cpu::SBC,&Cpu::YZPI,5, "SBC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::SBC,&Cpu::XZP,4, "SBC"},{&Cpu::INC,&Cpu::XZP,6, "INC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::SED,&Cpu::IMP,2, "SED"},{&Cpu::SBC,&Cpu::YABS,4, "SBC"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"},{&Cpu::ERR,&Cpu::ERR,0, "???"}, {&Cpu::SBC,&Cpu::XABS,4, "SBC"},{&Cpu::INC,&Cpu::XABS,7, "INC"},{&Cpu::ERR,&Cpu::ERR,0, "???"}
	};

}

Cpu::~Cpu()
{
}

void Cpu::connectBus(Bus *ptr)
{
	bus = ptr; 
}

void Cpu::write(uint16_t address, uint8_t data)
{
	bus->CpuWrite(address, data);
}

uint8_t Cpu::read(uint16_t address)
{
	return bus->CpuRead(address);
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

void Cpu::dataFetch()
{
	data = read(address);
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

	uint16_t addr = ((high << 8) | low);

	addr = (read(addr + 1) << 8) | read(addr);

	address = addr;

	return 0;
}
uint8_t Cpu::ZP()
{
	pc++;
	uint16_t addr = read(pc);
	addr = 0x00FF & addr;
	address = addr;
	return 0;
}
uint8_t Cpu::XZP()
{
	pc++;
	uint16_t addr = read(pc);
	addr = 0x00FF & (addr + rX);
	address = addr;
	return 0;
}
uint8_t Cpu::YZP()
{
	pc++;
	uint16_t addr = read(pc);
	addr = 0x00FF & (addr + rY);
	address = addr;
	return 0;
}
uint8_t Cpu::XZPI() //X-Indexed Zero Page Indirect
{
	pc++;
	uint16_t addr = read(pc);

	uint8_t low = read((addr + rX) & 0x00FF);
	uint8_t high = read((addr + rX + 1) & 0x00FF);

	addr = (high << 8) | low;
	address = addr;

	return 0;
}
uint8_t Cpu::YZPI() //Zero Page Indirect Y-Indexed
{
	pc++;
	uint16_t addr = read(pc);

	uint8_t low = read(addr & 0x00FF);
	uint8_t high = read((addr + 1) & 0x00FF);

	addr = (high << 8) | low;
	address = addr + rY;

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
	dataFetch();
	rA = data;

	setFlag(Z,(rA == 0x00));
	setFlag(N,(rA & 0x80));

	return 0;
}
uint8_t Cpu::LDX()
{
	dataFetch();
	rX = data;

	setFlag(Z, (rX == 0x00));
	setFlag(N, (rX & 0x80));

	return 0;
}
uint8_t Cpu::LDY()
{
	dataFetch();
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
	if (opcodes[currentOpcode].addressMode != &Cpu::ACC)
		dataFetch();

	uint8_t shift = data << 1;
	setFlag(C, (shift & 0xFF00) > 0);
	setFlag(Z, (shift & 0x00FF) == 0x00);
	setFlag(N, (shift & 0x80));
	if (opcodes[currentOpcode].addressMode == &Cpu::ACC)
		rA = shift & 0x00FF;
	else
		write(address, shift & 0x00FF);
	return 0;
}
uint8_t Cpu::LSR()
{
	if (opcodes[currentOpcode].addressMode != &Cpu::ACC)
		dataFetch();

	uint8_t shift = data >> 1;
	setFlag(C, (shift & 0xFF00) > 0);
	setFlag(Z, (shift & 0x00FF) == 0x00);
	setFlag(N, (shift & 0x80));
	if (opcodes[currentOpcode].addressMode == &Cpu::ACC)
		rA = shift & 0x00FF;
	else
		write(address, shift & 0x00FF);
	return 0;
}
uint8_t Cpu::ROL()
{
	if (opcodes[currentOpcode].addressMode != &Cpu::ACC)
		dataFetch();

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
	if (opcodes[currentOpcode].addressMode != &Cpu::ACC)
		dataFetch();

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
	dataFetch();
	rA &= data;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
uint8_t Cpu::BIT()
{
	dataFetch();
	setFlag(Z, ((rA & data) == 0x00));
	setFlag(N, data & (1 << 7));
	setFlag(V, data & (1 << 6));
	return 0;
}
uint8_t Cpu::EOR()
{
	dataFetch();
	rA ^= data;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
uint8_t Cpu::ORA()
{
	dataFetch();
	rA |= data;
	setFlag(Z, (rA == 0x00));
	setFlag(N, (rA & 0x80));
	return 0;
}
//arithmetic
uint8_t Cpu::ADC()
{
	dataFetch();
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
	dataFetch();
	uint16_t result = rA - data;
	setFlag(C, data <= rA);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::CPX()
{
	dataFetch();
	uint16_t result = rX - data;
	setFlag(C, data <= rX);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::CPY()
{
	dataFetch();
	uint16_t result = rY - data;
	setFlag(C, data <= rY);
	setFlag(Z, ((result & 0x00FF) == 0x00));
	setFlag(N, (result & 0x80));
	return 0;
}
uint8_t Cpu::SBC()
{
	dataFetch();
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
	dataFetch();
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
	dataFetch();
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
	write(0x0100 + sp, (pc >> 8) & 0xFF00);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;
	setFlag(B, 1);
	write(0x0100 + sp, status);
	setFlag(B, 0);
	sp--;


	pc = read(0xFFFE) | (read(0xFFFF) << 8);
	return 0;
}
uint8_t Cpu::JMP()
{
	pc = address -1;
	return 0;
}
uint8_t Cpu::JSR()
{
	
	write(0x0100 + sp, ((pc)>> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, (pc) & 0x00FF);
	sp--;

	pc = address -1;
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
	pc--;
	return 0;
}
uint8_t Cpu::RTS()
{
	sp++;
	uint8_t low = read(0x0100 + sp);
	sp++;
	uint8_t high = read(0x0100 + sp);

	pc = (high << 8) | low;
	pc;
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

std::map<uint16_t, std::string> Cpu::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
		{
			std::string s(d, '0');
			for (int i = d - 1; i >= 0; i--, n >>= 4)
				s[i] = "0123456789ABCDEF"[n & 0xF];
			return s;
		};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->CpuRead(addr, true); addr++;
		sInst += opcodes[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (opcodes[opcode].addressMode == &Cpu::IMP)
		{
			sInst += " {IMP}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::IMM)
		{
			value = bus->CpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::ZP)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::XZP)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {XZP}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::YZP)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {YZP}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::XZPI)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {XZPI}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::YZPI)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {YZPI}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::ABS)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::XABS)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {XABS}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::YABS)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {YABS}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::INC)
		{
			lo = bus->CpuRead(addr, true); addr++;
			hi = bus->CpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {INC}";
		}
		else if (opcodes[opcode].addressMode == &Cpu::REL)
		{
			value = bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}
  