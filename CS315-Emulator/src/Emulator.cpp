#include <memory>
#include "Input.h"
#include "Emulator.h"
#include "Disassembler.h"

int Chip8::LoadRom(std::string rom)
{
	if (mDisassembler.LoadRom(rom) < 0)
		return -1;

	char mFonts[16][5] = { { (char)0xF0, (char)0x90, (char)0x90, (char)0x90, (char)0xF0 },
						   { (char)0x20, (char)0x60, (char)0x20, (char)0x20, (char)0x70 },
						   { (char)0xF0, (char)0x10, (char)0xF0, (char)0x80, (char)0xF0 },
						   { (char)0xF0, (char)0x10, (char)0xF0, (char)0x10, (char)0xF0 },
						   { (char)0x90, (char)0x90, (char)0xF0, (char)0x10, (char)0x10 },
						   { (char)0xF0, (char)0x80, (char)0xF0, (char)0x10, (char)0xF0 },
						   { (char)0xF0, (char)0x80, (char)0xF0, (char)0x90, (char)0xF0 },
						   { (char)0xF0, (char)0x10, (char)0x20, (char)0x40, (char)0x40 },
						   { (char)0xF0, (char)0x90, (char)0xF0, (char)0x90, (char)0xF0 },
						   { (char)0xF0, (char)0x90, (char)0xF0, (char)0x10, (char)0xF0 },
						   { (char)0xF0, (char)0x90, (char)0xF0, (char)0x90, (char)0x90 },
						   { (char)0xE0, (char)0x90, (char)0xE0, (char)0x90, (char)0xE0 },
						   { (char)0xF0, (char)0x80, (char)0x80, (char)0x80, (char)0xF0 },
						   { (char)0xE0, (char)0x90, (char)0x90, (char)0x90, (char)0xE0 },
						   { (char)0xF0, (char)0x80, (char)0xF0, (char)0x80, (char)0xF0 },
						   { (char)0xF0, (char)0x80, (char)0xF0, (char)0x80, (char)0x80 } 
						 };

	std::memcpy(&mRAM[0x0], &mFonts[0x0], 16 * 5);


	return 0;
}

void Chip8::Update()
{
	//get Opcode, update stack
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
	uint8_t src = op.GetSrcRegister();
	uint8_t dst = op.GetDestRegister();
	uint16_t mem = op.GetMemory();
	uint8_t val = op.GetValue();
	uint8_t last = op.GetCount();
	mPC += 2;

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
			JP(mem);
			break;
		}
		case 0x2:// CALL NNN
		{
			CALL(mem);
			break;
		}
		case 0x3://SE VX KK
		{
			SE_VAL(src, val);
			break;
		}
		case 0x4://SNE VX KK
		{
			SNE_VAL(src, val);
			break;
		}
		case 0x5:
		{
			if (op.GetCount() == 0)//SE VX VY
				SE_RGSTR(src, dst);
			break;
		}
		case 0x6://LD VX KK
		{
			LD_VAL(src, val);
			break;
		}
		case 0x7://ADD VX KK
		{
			ADD_VAL(src, val);
			break;
		}
		case 0x8:
		{
			Ox8(op);
			break;
		}
		case 0x9://SNE VX VY
		{
			SNE_RGSTR(src, dst);
			break;
		}
		case 0xA://LD I NNN
		{
			LDIN(mem);
			break;
		}
		case 0xB://JP V0 NNN
		{
			JPV0(mem);
			break;
		}
		case 0xC://RND VX KK
		{
			RND(src, val);
			break;
		}
		case 0xD://DRW VX VY N
		{
			DRW(src, dst, last);
			break;
		}
		case 0xE://
		{
			if (val == 0x9E)//SKP VX
				SKP(src);
			else if (val == 0xA1)//SKNP VX
				SKNP(src);

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
	uint8_t src = op.GetSrcRegister();
	uint8_t dst = op.GetDestRegister();
	uint16_t mem = op.GetMemory();
	uint8_t val = op.GetValue();
	uint8_t last = op.GetCount();

	switch (last)
	{
		case 0x0://LD VX VY
		{
			LD_RGSTR(src, dst);
			break;
		}
		case 0x1://OR VX VY
		{
			OR(src, dst);
			break;
		}
		case 0x2://AND VX VY
		{
			AND(src, dst);
			break;
		}
		case 0x3://XOR VX VY
		{
			XOR(src, dst);
			break;
		}
		case 0x4://ADD VX VY
		{
			ADD_RGSTR(src, dst);
			break;
		}
		case 0x5://SUB VX VY
		{
			SUB(src, dst);
			break;
		}
		case 0x6://SHR VX VY
		{
			SHR(src, dst);
			break;
		}
		case 0x7://SUBN VX VY
		{
			SUBN(src, dst);
			break;
		}
		case 0xE://SHL VX VY
		{
			SHL(src, dst);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Chip8::OxF(const Opcode& op)
{
	uint8_t src = op.GetSrcRegister();
	uint8_t dst = op.GetDestRegister();
	uint16_t mem = op.GetMemory();
	uint8_t val = op.GetValue();
	uint8_t last = op.GetCount();

	switch (last)
	{
		case 0x07://LD VX DT
		{
			LDVDT(src);
			break;
		}
		case 0x0A://LD VX K
		{
			LDVK(src);
			break;
		}
		case 0x15://LD DT VX
		{
			LDDTV(src);
			break;
		}
		case 0x18://LD ST VX
		{
			LDSTV(src);
			break;
		}
		case 0x1E://ADD I VX
		{
			ADDI(src);
			break;
		}
		case 0x29://LD F VX
		{
			LDFV(src);
			break;
		}
		case 0x33://LD B VX
		{
			LDBV(src);
			break;
		}
		case 0x55://LD I VX
		{
			LDIV(src);
			break;
		}
		case 0x65://LD VX I
		{
			LDVI(src);
			break;
		}
		default:
		{
			break;
		}
	}
}

#pragma region SYSTEM FUNCTIONS

void Chip8::CLS()
{
	std::memset(&mFrameBuffer, false, sizeof(mFrameBuffer));
}
void Chip8::RET()
{
	mSP--;//decrease stack pointer
	mPC = mStack[mSP];//update the program counter to the correct address
	mPC += 2;//moving the program counter
}

void Chip8::JP(unsigned short addr)
{
	mPC = addr;
	mPC -= 2;
}

void Chip8::JPV0(unsigned short addr)
{
	mPC = mV[0] + addr;
	mPC -= 2;
}

void Chip8::CALL(unsigned short addr)
{
	mStack[mSP] = mPC;//storing the current program counter address
	mSP++;//increment the stack pointer
	mPC = addr;//setting the program counter address
	mPC -= 2;
}

void Chip8::DRW(char vX, char vY, unsigned short size)
{

}
#pragma endregion

#pragma region INSTRUCTION FUNCTIONS
void Chip8::SE_VAL(char v, char k)
{
	if (mV[v] == k)
		mPC += 4;//addign 4 to skip next instruction
	else
		mPC += 2;//regular update of the program counter
}

void Chip8::SE_RGSTR(char v0, char v1)
{
	if (mV[v0] == mV[v1])
		mPC += 4;//addign 4 to skip next instruction
	else
		mPC += 2;//regular update of the program counter
}
void Chip8::SNE_VAL(char v, char k)
{
	if (mV[v] != k)
		mPC += 4;//addign 4 to skip next instruction
	else
		mPC += 2;//regular update of the program counter
}
void Chip8::SNE_RGSTR(char v0, char v1)
{
	if (mV[v0] != mV[v0])
		mPC += 4;//addign 4 to skip next instruction
	else
		mPC += 2;//regular update of the program counter
}
#pragma endregion

#pragma region INSTRUCTION FUNCTIONS
void Chip8::SKP(char v)
{
	if(KeyDown(mInputKeys[mV[v]]))
		mPC += 4;//addign 4 to skip next instruction
	else
		mPC += 2;//regular update of the program counter
}
void Chip8::SKNP(char v)
{
	if (!KeyDown(mInputKeys[mV[v]]))
		mPC += 4;//addign 4 to skip next instruction
	else
		mPC += 2;//regular update of the program counter
}
void Chip8::LD_VAL(char v, char val)
{
	mV[v] = val;
}
void Chip8::LD_RGSTR(char srcV, char dstV)
{
	mV[dstV] = mV[srcV];
}
#pragma endregion

#pragma region LOAD FUNCTIONS
void Chip8::LDIN(unsigned short addr)
{
	mI = addr;
}
void Chip8::LDVDT(char v)
{
	mV[v] = mDT;
}
void Chip8::LDVK(char v)
{
	//check if any of the input jeys pressed
	for (unsigned short i = 0; i < 16; i++)
	{
		//if pressed store the value in mV[v]
		if (KeyDown(mInputKeys[i]))
		{
			mV[v] = (unsigned char)i;
			break;
		}
	}

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
	mI = mRAM[mV[v] * 5];
}
void Chip8::LDBV(char v)
{
	unsigned char hundreds = mV[v] / 100;
	unsigned char decimals = (mV[v] % 100) / 10;
	unsigned char units = (mV[v] % 100) % 10;

	mRAM[mI] = hundreds;
	mRAM[mI + 1] = decimals;
	mRAM[mI + 2] = units;

}
void Chip8::LDIV(char v)
{
	for (char i = 0; i < v; i++)
		mRAM[mI + i] = mV[i];//storing the states of the resgisters at address I

	mI += v + 1;
}
void Chip8::LDVI(char v)
{
	for (char i = 0; i < v; i++)
		mV[i] = mRAM[mI + i];//loading the states of the resgisters at address I

	mI += v + 1;
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

void Chip8::ADD_VAL(char v, char val)
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
void Chip8::ADD_RGSTR(char srcV, char dstV)
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
	unsigned short result = mV[v] + mI;
	//check if overflow
	if (result > 255)
		mV[15] = 1;
	else
		mV[15] = 0;
	//getting the lowest 8 bits of the short
	mI = result & 0x00FF;
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
void Chip8::RND(char v, char val)
{
	mV[v] = ((char)std::rand()) & val;
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

