#include "Disassembler.h"
#include "Utils.h"
#include <iostream>
#include <string>

void Disassembler::PrintOpcode(uint16_t opcode)
{
	uint16_t initial_byte = Utils::GetBits(opcode, 0);
	Opcode code(opcode);
	std::cout << std::hex << opcode << " -> ";

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
		short src = code.GetSrcRegister();
		short dst = code.GetDestRegister();

		std::cout << "SNE V" << std::hex << src << " V" << std::hex << dst;
		break;
	}
	case 0xA:
	{
		short address = code.GetMemory();
		std::cout << "LD I" << std::hex << address;
		break;
	}
	case 0xB:
	{
		short address = code.GetMemory();
		std::cout << "JP I" << std::hex << address;
		break;
	}
	case 0xC:
	{
		short src = code.GetSrcRegister();
		short val = code.GetValue();

		std::cout << "RND V" << std::hex << src << " " << std::hex << val;
		break;
	}
	case 0xD:
	{
		short src = code.GetSrcRegister();
		short dst = code.GetDestRegister();
		short val = code.GetCount();

		std::cout << "DRW V" << std::hex << src << " V" << std::hex << dst << " " << std::hex << val;
		break;
	}
	case 0xE:
	{
		short val = code.GetValue();
		short src = code.GetSrcRegister();

		if (val == 0x9e)
			std::cout << "SKP V" << std::hex << src;
		else if (val == 0xa1)
			std::cout << "SKNP V" << std::hex << src;
		else
			std::cout << "UNKN";

		break;
	}
	case 0xF:
	{
		short val = code.GetValue();
		short src = code.GetSrcRegister();

		if (val == 0x07)
			std::cout << "LD V" << std::hex << src << "DT";
		else if (val == 0x0a)
			std::cout << "LD V" << std::hex << src << "K";
		else if (val == 0x15)
			std::cout << "LD DT" << " V" << std::hex << src;
		else if (val == 0x18)
			std::cout << "LD ST" << " V" << std::hex << src;
		else if (val == 0x1e)
			std::cout << "ADD I" << " V" << std::hex << src;
		else if (val == 0x29)
			std::cout << "LD F" << " V" << std::hex << src;
		else if (val == 0x33)
			std::cout << "LD B" << " V" << std::hex << src;
		else if (val == 0x55)
			std::cout << "LD I" << " V" << std::hex << src;
		else if (val == 0x65)
			std::cout << "LD V" << std::hex << src << " I";
		else
			std::cout << "UNKN";

		break;
	}
	default:
		std::cout << "UNKN";

	}
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
