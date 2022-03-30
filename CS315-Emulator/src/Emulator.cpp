#include <memory>
#include "Emulator.h"
#include "Disassembler.h"

void Chip8::Update()
{
	//get Opcode, update stack and shit
	mDisassembler.GetInstruction();
	mPC += 2;

	if (mDT > 0)
		mDT--;
	if (mST > 0)
		mST--;


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

