#include <memory>
#include <iostream>
#include "Input.h"
#include "Emulator.h"
#include "Disassembler.h"
#include "Utils.h"

int Chip8::LoadRom(std::string rom)
{
	if (mDisassembler.LoadRom(rom) < 0)
		return -1;


	uint8_t mFonts[16][5] = {	{ 0xF0, 0x90, 0x90, 0x90, 0xF0 },
								{ 0x20, 0x60, 0x20, 0x20, 0x70 },
								{ 0xF0, 0x10, 0xF0, 0x80, 0xF0 },
								{ 0xF0, 0x10, 0xF0, 0x10, 0xF0 },
								{ 0x90, 0x90, 0xF0, 0x10, 0x10 },
								{ 0xF0, 0x80, 0xF0, 0x10, 0xF0 },
								{ 0xF0, 0x80, 0xF0, 0x90, 0xF0 },
								{ 0xF0, 0x10, 0x20, 0x40, 0x40 },
								{ 0xF0, 0x90, 0xF0, 0x90, 0xF0 },
								{ 0xF0, 0x90, 0xF0, 0x10, 0xF0 },
								{ 0xF0, 0x90, 0xF0, 0x90, 0x90 },
								{ 0xE0, 0x90, 0xE0, 0x90, 0xE0 },
								{ 0xF0, 0x80, 0x80, 0x80, 0xF0 },
								{ 0xE0, 0x90, 0x90, 0x90, 0xE0 },
								{ 0xF0, 0x80, 0xF0, 0x80, 0xF0 },
								{ 0xF0, 0x80, 0xF0, 0x80, 0x80 } 
						 };

	std::memcpy(&mRAM[0x0], &mFonts[0x0], 16 * 5);

	mPC = 0x200;
	Opcode op;
	while (mDisassembler.seekpos < mDisassembler.romSize)
	{
		op = mDisassembler.GetInstruction();
		std::memcpy(&mRAM[mPC], &op.mOpcode, sizeof(uint16_t));
		mPC += 2;
	}

	mPC = 0x200;
	return 0;
}

void Chip8::Update()
{
	//get Opcode, update stack
	Opcode operation(*reinterpret_cast<uint16_t*>(&mRAM[mPC]));
	std::cout << std::hex << mPC << ": ";
	mDisassembler.PrintOpcode(operation.mOpcode);
	std::cout << std::endl;

	HandleOpcode(operation);
	mPC += 2;

	if (mDT > 0)
		mDT--;
	if (mST > 0)
		mST--;
}

void Chip8::HandleOpcode(const Opcode& op)
{
	uint16_t first = Utils::GetBits(op.mOpcode, 0);
	uint8_t VX = op.GetVXRegister();
	uint8_t VY = op.GetVYRegister();
	uint16_t mem = op.GetMemory();
	uint8_t val = op.GetValue();
	uint8_t last = op.GetCount();

	switch (first)
	{
		case 0x0:
		{
			if (op.mOpcode == 0xE000)// CLS
				CLS();
			else if (op.mOpcode == 0xEE00)// RET
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
			SE_VAL(VX, val);
			break;
		}
		case 0x4://SNE VX KK
		{
			SNE_VAL(VX, val);
			break;
		}
		case 0x5:
		{
			if (op.GetCount() == 0)//SE VX VY
				SE_RGSTR(VX, VY);
			break;
		}
		case 0x6://LD VX KK
		{
			LD_VAL(VX, val);
			break;
		}
		case 0x7://ADD VX KK
		{
			ADD_VAL(VX, val);
			break;
		}
		case 0x8:
		{
			Ox8(op);
			break;
		}
		case 0x9://SNE VX VY
		{
			SNE_RGSTR(VX, VY);
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
			RND(VX, val);
			break;
		}
		case 0xD://DRW VX VY N
		{
			DRW(VX, VY, last);
			break;
		}
		case 0xE://
		{
			if (val == 0x9E)//SKP VX
				SKP(VX);
			else if (val == 0xA1)//SKNP VX
				SKNP(VX);
			break;
		}
		case 0xF:
		{
			OxF(op);
			break;
		}
		default:
		{
			std::cout << "ERROR NO HANDLER" << std::endl;
		}
	}
}

void Chip8::Ox8(const Opcode& op)
{
	uint8_t VX = op.GetVXRegister();
	uint8_t VY = op.GetVYRegister();
	uint16_t mem = op.GetMemory();
	uint8_t val = op.GetValue();
	uint8_t last = op.GetCount();

	switch (last)
	{
		case 0x0://LD VX VY
		{
			LD_RGSTR(VX, VY);
			break;
		}
		case 0x1://OR VX VY
		{
			OR(VX, VY);
			break;
		}
		case 0x2://AND VX VY
		{
			AND(VX, VY);
			break;
		}
		case 0x3://XOR VX VY
		{
			XOR(VX, VY);
			break;
		}
		case 0x4://ADD VX VY
		{
			ADD_RGSTR(VX, VY);
			break;
		}
		case 0x5://SUB VX VY
		{
			SUB(VX, VY);
			break;
		}
		case 0x6://SHR VX VY
		{
			SHR(VX, VY);
			break;
		}
		case 0x7://SUBN VX VY
		{
			SUBN(VX, VY);
			break;
		}
		case 0xE://SHL VX VY
		{
			SHL(VX, VY);
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
	uint8_t VX = op.GetVXRegister();
	uint8_t VY = op.GetVYRegister();
	uint16_t mem = op.GetMemory();
	uint8_t val = op.GetValue();
	uint8_t last = op.GetCount();

	switch (last)
	{
		case 0x07://LD VX DT
		{
			LDVDT(VX);
			break;
		}
		case 0x0A://LD VX K
		{
			LDVK(VX);
			break;
		}
		case 0x15://LD DT VX
		{
			LDDTV(VX);
			break;
		}
		case 0x18://LD ST VX
		{
			LvY(VX);
			break;
		}
		case 0x1E://ADD I VX
		{
			ADDI(VX);
			break;
		}
		case 0x29://LD F VX
		{
			LDFV(VX);
			break;
		}
		case 0x33://LD B VX
		{
			LDBV(VX);
			break;
		}
		case 0x55://LD I VX
		{
			LDIV(VX);
			break;
		}
		case 0x65://LD VX I
		{
			LDVI(VX);
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
	uint16_t row = vY;
	uint16_t col = vX;
	uint8_t * location = &mRAM[mI];
	bool collision = false;

	for (unsigned short i = 0; i < size; i++)
	{
		row = (mV[vY] + i) % HEIGHT;
		std::bitset<8> spriterow = *location;

		for (unsigned short j = 0; j < 8; j++)
		{
			col = (mV[vX] + j) % WIDTH;

			if (mFrameBuffer[row][col] && spriterow[7 - j])
				collision = true;
	
				bool paint = mFrameBuffer[row][col] ^ spriterow[7 - j];
				mFrameBuffer[row][col] = paint;
		}
		location++;
	}
	
	mV[0xF] = collision;
}
#pragma endregion

#pragma region INSTRUCTION FUNCTIONS
void Chip8::SE_VAL(char v, char k)
{
	if (mV[v] == (unsigned char)k)
		mPC += 2;
}

void Chip8::SE_RGSTR(char v0, char v1)
{
	if (mV[v0] == mV[v1])
		mPC += 2;
}
void Chip8::SNE_VAL(char v, char k)
{
	if (mV[v] != (unsigned char)k)
		mPC += 2;
}
void Chip8::SNE_RGSTR(char v0, char v1)
{
	if (mV[v0] != mV[v1])
		mPC += 2;
}
#pragma endregion

#pragma region INSTRUCTION FUNCTIONS
void Chip8::SKP(char v)
{
	if (KeyDown(mInputKeys[mV[v]]))
		mPC += 2;
}
void Chip8::SKNP(char v)
{
	if (!KeyDown(mInputKeys[mV[v]]))
		mPC += 2;
}
void Chip8::LD_VAL(char v, char val)
{
	mV[v] = val;
}
void Chip8::LD_RGSTR(char vX, char vY)
{
	mV[vX] = mV[vY];
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
void Chip8::LvY(char v)
{
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
void Chip8::OR(char vX, char vY)
{
	mV[vX] |= mV[vY];
}
void Chip8::AND(char vX, char vY)
{
	mV[vX] &= mV[vY];
}
void Chip8::XOR(char vX, char vY)
{
	mV[vX] ^= mV[vY];
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
void Chip8::ADD_RGSTR(char vX, char vY)
{
	unsigned short result = mV[vX] + mV[vY];

	//check if overflow
	mV[0xF] = (result > 255);

	//getting the lowest 8 bits of the short
	mV[vX] = result & 0x00FF;
}
void Chip8::ADDI(char v)
{
	unsigned short result = mV[v] + mI;
	//check if overflow
	mV[0xF] = (result > 255);

	//getting the lowest 8 bits of the short
	mI = result & 0x00FF;
}
void Chip8::SUB(char vX, char vY)
{
	// check if underflow
	mV[0xF] = mV[vY] > mV[vX];

	// substract
	mV[vX] = mV[vX] - mV[vY];
}
void Chip8::SUBN(char vX, char vY)
{
	// check if underflow
	mV[0xF] = !(mV[vX] > mV[vY]);

	mV[vX] = mV[vY] - mV[vX];
}
void Chip8::RND(char v, char val)
{
	mV[v] = ((char)std::rand()) & val;
}
#pragma endregion

#pragma region BIT MANIPULATION FUNCTIONS
void Chip8::SHR(char vX, char vY)
{
	mV[0xF] = mV[vX] & 1;
	mV[vX] <<= 1;
}
void Chip8::SHL(char vX, char vY)
{
	mV[vY] <<= mV[vX];
}
#pragma endregion

