#pragma once
#include <fstream>

struct Disassembler
{
	void PrintOpcode(uint16_t opcode);
	int LoadRom(std::string rom);
	std::ifstream rom;
	int seekpos = 0;
	int romSize = 0;
	struct Opcode
	{
		Opcode(uint16_t code = 0);

		uint16_t mOpcode;
		uint16_t GetSrcRegister() const;
		uint16_t GetDestRegister() const;
		uint16_t GetValue() const;
		uint16_t GetMemory() const;
		uint16_t GetCount() const;
	};

	Opcode GetInstruction();
};