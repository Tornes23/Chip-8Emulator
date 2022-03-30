#include "Disassembler.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
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

int Disassembler::LoadRom(std::string file)
{
	rom.open(file, std::ios::binary | std::ios::in);
	if (rom.bad())
		return -1;

	rom.seekg(0, rom.end);
	romSize = rom.tellg();
	rom.seekg(0, rom.beg);

	return 0;
}

Disassembler::Opcode Disassembler::GetInstruction()
{
	if (seekpos > romSize)
		return Opcode(0x000);

	char buf[2];

	rom.read(buf, 2);
	std::swap(buf[0], buf[1]); // little endian conversor
	uint16_t opcode = *reinterpret_cast<uint16_t*>(buf);

	std::cout << std::hex << seekpos << ": ";
	Disassembler::PrintOpcode(opcode);
	std::cout << std::endl;

	seekpos += 2;

	return Opcode(opcode);
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
