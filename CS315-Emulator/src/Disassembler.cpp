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
			std::cout << "CLS";
		}
		else if (opcode == 0x00EE)
		{
			// RET
			std::cout << "RET";
		}
		else
		{
			// SYS NNN
			uint16_t address = code.GetMemory();
			std::cout << "SYS " << std::hex << address;
		}
		break;
	}
	case 0x1:
	{
		// JP NNN
		uint16_t address = code.GetMemory();
		std::cout << "JP " << std::hex << address;
		break;
	}
	case 0x2:
	{
		// CALL NNN
		uint16_t address = code.GetMemory();
		std::cout << "CALL " << address;
		break;
	}
	case 0x3:
	{
		uint16_t VX = code.GetSrcRegister();
		uint16_t KK = code.GetValue();
		std::cout << "SE V" << std::hex << VX << " " << std::hex << KK;

		break;
	}
	case 0x4:
	{
		uint16_t VX = code.GetSrcRegister();
		uint16_t KK = code.GetValue();
		std::cout << "SNE V" << std::hex << VX << " " << std::hex << KK;

		break;
	}
	case 0x5:
	{
		if (code.GetCount() == 0)
		{
			std::cout << "SE V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		std::cout << "UNKN";
		break;
	}
	case 0x6:
	{
		uint16_t VX = code.GetSrcRegister();
		uint16_t KK = code.GetValue();
		std::cout << "LD V" << VX << " " << KK;

		break;
	}
	case 0x7:
	{
		uint16_t VX = code.GetSrcRegister();
		uint16_t KK = code.GetValue();
		std::cout << "ADD V" << VX << " " << KK;

		break;
	}
	case 0x8:
	{
		uint16_t last = code.GetCount();

		switch (last)
		{
		case 0x0:
		{
			std::cout << "LD V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x1:
		{
			std::cout << "OR V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x2:
		{
			std::cout << "AND V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x3:
		{
			std::cout << "XOR V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x4:
		{
			std::cout << "ADD V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x5:
		{
			std::cout << "SUB V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x6:
		{
			std::cout << "SHR V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0x7:
		{
			std::cout << "SUBN V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		case 0xE:
		{
			std::cout << "SHL V" << std::hex << code.GetSrcRegister() << " V" << std::hex << code.GetDestRegister();
			break;
		}
		default:
		{
			std::cout << "UNKN";
			break;
		}
		}
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
