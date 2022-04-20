#pragma once
#include <fstream>

struct Opcode
{
	Opcode(uint16_t code = 0);

	uint16_t mOpcode;
	uint8_t GetSrcRegister() const;
	uint8_t GetDestRegister() const;
	uint8_t GetValue() const;
	uint16_t GetMemory() const;
	uint8_t GetCount() const;
};

struct Disassembler
{
	void PrintOpcode(uint16_t opcode);
	int LoadRom(std::string rom);
	std::ifstream rom;
	int seekpos = 0;
	int romSize = 0;


	Opcode GetInstruction();
};