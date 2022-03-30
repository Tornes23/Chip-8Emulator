#pragma once
#include <map>
#include "Disassembler.h"
#include <array>

class Chip8
{
public:
	//Singleton stuff
	Chip8()
		: mRAM{}
		, mStack{}
		, mV{}
		, mFrameBuffer { {} }
	{}

	int LoadRom(std::string rom = "test_opcode.ch8");
	void Update();
	void Render();
	void HandleOpcode(const Opcode& op);

	void Ox8(const Opcode& op);
	void OxF(const Opcode& op);

	static const unsigned MEMORYSIZE = 4096;
	static const unsigned STACKSIZE = 32;
	static const unsigned WIDTH = 64;
	static const unsigned HEIGHT = 32;

private:
	Disassembler mDisassembler;
	unsigned char mRAM[MEMORYSIZE];
	unsigned short mStack[STACKSIZE];
	
	unsigned char mV[16];
	unsigned short mPC = 0x200;//starting address
	unsigned short mI = 0;
	unsigned short mSP = 0;

	unsigned char mDT = 0;
	unsigned char mST = 0;

	std::array<std::array<bool, WIDTH>, HEIGHT> mFrameBuffer;

	void SYS(short addr);
	void CLS();
	void RET();//to do
	void JP(short addr);
	void JPV0(short addr);//to do
	void CALL(short addr);//to do
	void DRW(char vX, char vY, short size);//to do

	void SE_VAL(char v, char k);//to do
	void SE_RGSTR(char v0, char v1);//to do
	void SNE_VAL(char v, char k);//to do
	void SNE_RGSTR(char v0, char v1);//to do
	void SKP(char v);//to do
	void SKNP(char v);//to do

	void LD_VAL(char v, char val);
	void LD_RGSTR(char srcV, char dstV);
	void LDIN(unsigned short addr);
	void LDVDT(char v);
	void LDVK(char v, char key);
	void LDDTV(char v);
	void LDSTV(char v);
	void LDFV(char v);//to do
	void LDBV(char v);//to do
	void LDIV(char v);//to do
	void LDVI(char v);//to do

	void OR(char srcV, char dstV);
	void AND(char srcV, char dstV);
	void XOR(char srcV, char dstV);

	void ADD_VAL(char v, char val);
	void ADD_RGSTR(char srcV, char dstV);
	void ADDI(char v);//to do
	void SUB(char srcV, char dstV);
	void SUBN(char srcV, char dstV);
	void RND(char v, char val);//to do


	void SHR(char srcV, char dstV);
	void SHL(char srcV, char dstV);

};