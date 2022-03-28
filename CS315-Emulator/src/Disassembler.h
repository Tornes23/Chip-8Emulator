#pragma once
#include <fstream>

namespace Disassembler
{
	void PrintOpcode(uint16_t opcode);

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
}