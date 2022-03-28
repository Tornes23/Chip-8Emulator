#include "Disassembler.h"
#include "Utils.h"
#include <iostream>
#include <string>

void Disassembler::PrintOpcode(uint16_t opcode)
{
	uint16_t initial_byte = Utils::GetBits(opcode, 0);
	Opcode code(opcode);
	std::cout << std::hex << opcode << " -> ";
	std::string assembly_out{};

	switch (initial_byte)
	{
	case 0x0:
	{
		if (opcode == 0x00E0)
		{
			// CLS
		}
		else if (opcode == 0x00EE)
		{
			// RET
		}
		else
		{
			uint16_t address = code.GetMemory();
			// SYS NNN
		}
		break;
	}
	case 0x1:
	{
		uint16_t address = code.GetMemory();
		// JP NNN
		break;
	}
	case 0x2:
	{
		uint16_t address = code.GetMemory();
		// CALL NNN
		break;
	}
	case 0x3:
	{
		uint16_t VX = code.GetSrcRegister();

		break;
	}
	case 0x4:
	{
		break;
	}
	case 0x5:
	{
		break;
	}
	case 0x6:
	{
		break;
	}
	case 0x7:
	{
		break;
	}
	case 0x8:
	{
		break;
	}
	case 0x9:
	{
		break;
	}
	case 0xA:
	{
		break;
	}
	case 0xB:
	{
		break;
	}
	case 0xC:
	{
		break;
	}
	case 0xD:
	{
		break;
	}
	case 0xE:
	{
		break;
	}
	case 0xF:
	{
		break;
	}
	}

	std::cout << assembly_out;

}

Disassembler::Opcode::Opcode(uint16_t code)
	: mOpcode(code)
{
}

uint16_t Disassembler::Opcode::GetSrcRegister() const
{
	return Utils::GetBits(mOpcode, 1);
}

uint16_t Disassembler::Opcode::GetDestRegister() const
{
	return Utils::GetBits(mOpcode, 2);
}

uint16_t Disassembler::Opcode::GetValue() const
{
	return Utils::GetBits(mOpcode, 2, 2);
}

uint16_t Disassembler::Opcode::GetMemory() const
{
	return Utils::GetBits(mOpcode, 1, 3);
}

uint16_t Disassembler::Opcode::GetCount() const
{
	return Utils::GetBits(mOpcode, 3);
}
