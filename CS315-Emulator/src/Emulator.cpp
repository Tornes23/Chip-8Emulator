#include <memory>
#include "Emulator.h"
#include "Disassembler.h"

int Chip8::LoadRom(std::string rom)
{
	if (mDisassembler.LoadRom(rom) < 0)
		return -1;

	return 0;
}

void Chip8::Update()
{
	//get Opcode, update stack and shit
	Opcode operation = mDisassembler.GetInstruction();
	mPC += 2;

	HandleOpcode(operation);

	if (mDT > 0)
		mDT--;
	if (mST > 0)
		mST--;


}

void Chip8::HandleOpcode(const Opcode& op)
{
	unsigned short first = op.mOpcode & 0xF000;
	switch (first)
	{
		case 0x0:
		{
			if (op.mOpcode == 0x00E0)// CLS
				CLS();
			else if (op.mOpcode == 0x00EE)// RET
				RET();
			break;
		}
		case 0x1:// JP NNN
		{
			JP(op.GetMemory());
			break;
		}
		case 0x2:// CALL NNN
		{
			CALL(op.GetMemory());
			break;
		}
		case 0x3://SE VX KK
		{
			SE_VAL(op.GetSrcRegister(), op.GetValue());
			break;
		}
		case 0x4://SNE VX KK
		{
			SNE_VAL(op.GetSrcRegister(), op.GetValue());
			break;
		}
		case 0x5:
		{
			if (op.GetCount() == 0)//SE VX VY
				SE_RGSTR(op.GetSrcRegister(), op.GetDestRegister());
			break;
		}
		case 0x6://LD VX KK
		{
			LD_VAL(op.GetSrcRegister(), op.GetValue());
			break;
		}
		case 0x7://ADD VX KK
		{
			ADD_VAL(op.GetSrcRegister(), op.GetValue());
			break;
		}
		case 0x8:
		{
			Ox8(op);
			break;
		}
		case 0x9://SNE VX VY
		{
			SNE_RGSTR(op.GetSrcRegister(), op.GetDestRegister());
			break;
		}
		case 0xA://LD I NNN
		{
			LDIN(op.GetMemory());
			break;
		}
		case 0xB://JP V0 NNN
		{
			JPV0(op.GetMemory());
			break;
		}
		case 0xC://RND VX KK
		{
			RND(op.GetSrcRegister(), op.GetValue());
			break;
		}
		case 0xD://DRW VX VY N
		{
			DRW(op.GetSrcRegister(), op.GetDestRegister(), op.GetCount());
			break;
		}
		case 0xE://
		{
			short val = op.GetValue();
			short src = op.GetSrcRegister();

			if (val == 0x9E)//SKP VX
				SKP(op.GetSrcRegister());
			else if (val == 0xA1)//SKNP VX
				SKNP(op.GetSrcRegister());

			break;
		}
		case 0xF:
		{
			OxF(op);
			break;
		}
	}
}

void Chip8::Ox8(const Opcode& op)
{
	uint16_t last = op.GetCount();

	switch (last)
	{
	case 0x0://LD VX VY
	{
		std::cout << "LD V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x1:
	{
		std::cout << "OR V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x2:
	{
		std::cout << "AND V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x3:
	{
		std::cout << "XOR V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x4:
	{
		std::cout << "ADD V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x5:
	{
		std::cout << "SUB V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x6:
	{
		std::cout << "SHR V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0x7:
	{
		std::cout << "SUBN V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
		break;
	}
	case 0xE:
	{
		std::cout << "SHL V" << std::hex << op.GetSrcRegister() << " V" << std::hex << op.GetDestRegister();
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

void Chip8::OxF(const Opcode& op)
{
	short val = op.GetValue();
	short src = op.GetSrcRegister();

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

}

#pragma region SYSTEM FUNCTIONS
void Chip8::CLS()
{
	std::memset(&mFrameBuffer, false, sizeof(mFrameBuffer));
}
void Chip8::RET()
{
}

void Chip8::JP(short addr)
{
}

void Chip8::JP(char v0, short addr)
{
}

void Chip8::CALL(short addr)
{
}

void Chip8::DRW(char vX, char vY, short size)
{

}
#pragma endregion

#pragma region INSTRUCTION FUNCTIONS
void Chip8::SE_VAL(char v, char k)
{
	if (mV[v] == k)
	{

	}
}

void Chip8::SE_RGSTR(char v0, char v1)
{
	if (mV[v0] == mV[v1])
	{

	}
}
void Chip8::SNE_VAL(char v, char k)
{
	if (mV[v] != k)
	{

	}
}
void Chip8::SNE_RGSTR(char v0, char v1)
{
	if (mV[v0] != mV[v0])
	{

	}
}
void Chip8::SKP(char v)
{

}
void Chip8::SKNP(char v)
{

}
#pragma endregion

#pragma region LOAD FUNCTIONS
void Chip8::LD_VAL(char v, char val)
{
	mV[v] = val;
}
void Chip8::LD_RGSTR(char srcV, char dstV)
{
	mV[dstV] = mV[srcV];
}
void Chip8::LDIN(unsigned short addr)
{
	mI = addr;
}
void Chip8::LDVDT(char v)
{
	mV[v] = mDT;
}
void Chip8::LDVK(char v, char key)
{
	mV[v] = key;
}
void Chip8::LDDTV(char v)
{
	mDT = mV[v];
}
void Chip8::LDSTV(char v)
{
	mST = mV[v];
}
void Chip8::LDFV(char v)
{

}
void Chip8::LDBV(char v)
{

}
void Chip8::LDIV(char v)
{

}
void Chip8::LDVI(char v)
{

}
#pragma endregion

#pragma region LOGICAL FUNCTIONS
void Chip8::OR(char srcV, char dstV)
{
	mV[dstV] |= mV[srcV];
}
void Chip8::AND(char srcV, char dstV)
{
	mV[dstV] &= mV[srcV];
}
void Chip8::XOR(char srcV, char dstV)
{
	mV[dstV] ^= mV[srcV];
}
#pragma endregion

#pragma region ARITHMETIC FUNCTIONS

void Chip8::ADD(char v, short val)
{
	unsigned short result = mV[v] + val;
	//check if overflow
	if (result > 255)
		mV[15] = 1;
	else
		mV[15] = 0;
	//getting the lowest 8 bits of the short
	mV[v] = result & 0x00FF;
	
}
void Chip8::ADD(char srcV, char dstV)
{
	unsigned short result = mV[dstV] + mV[srcV];
	//check if overflow
	if (result > 255)
		mV[15] = 1;
	else
		mV[15] = 0;
	//getting the lowest 8 bits of the short
	mV[dstV] = result & 0x00FF;
}
void Chip8::ADDI(char v)
{

}
void Chip8::SUB(char srcV, char dstV)
{
	//check if underflow
	if (mV[dstV] < mV[srcV])
		mV[15] = 1;
	else
		mV[15] = 0;

	mV[dstV] -= mV[srcV];
}
void Chip8::SUBN(char srcV, char dstV)
{
	if (mV[dstV] > mV[srcV])
		mV[15] = 1;
	else
		mV[15] = 0;

	mV[dstV] = mV[srcV] - mV[dstV];
}
void Chip8::RND(char v, short val)
{

}
#pragma endregion

#pragma region BIT MANIPULATION FUNCTIONS
void Chip8::SHR(char srcV, char dstV)
{
	mV[dstV] >>= mV[srcV];
}
void Chip8::SHL(char srcV, char dstV)
{
	mV[dstV] <<= mV[srcV];
}
#pragma endregion

