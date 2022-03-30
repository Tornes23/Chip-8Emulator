#pragma once
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

	Chip8(Chip8 const&) = delete;
	void operator=(Chip8 const&) = delete;

	void Update();
	void Render();

	static const unsigned MEMORYSIZE = 4096;
	static const unsigned STACKSIZE = 32;
	static const unsigned WIDTH = 64;
	static const unsigned HEIGHT = 32;

private:

	unsigned char mRAM[MEMORYSIZE];
	unsigned short mStack[STACKSIZE];
	
	unsigned char mV[16];
	unsigned short mPC = 0;
	unsigned short mI = 0;
	unsigned short mSP = 0;

	unsigned char mDT = 0;
	unsigned char mST = 0;

	std::array<std::array<bool, WIDTH>, HEIGHT> mFrameBuffer;

	void SYS(short addr);
	void CLS();
	void RET();//to do
	void JP(short addr);
	void JP(char v0, short addr);//to do
	void CALL(short addr);//to do
	void DRW(char vX, char vY, short size);//to do

	void SE(char v, char k);//to do
	void SE(char v0, char v1);//to do
	void SNE(char v, char k);//to do
	void SNE(char v0, char v1);//to do
	void SKP(char v);//to do
	void SKNP(char v);//to do

	void LD(char v, char val);
	void LD(char srcV, char dstV);
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

	void ADD(char v, short val);
	void ADD(char srcV, char dstV);
	void ADDI(char v);//to do
	void SUB(char srcV, char dstV);
	void SUBN(char srcV, char dstV);
	void RND(char v, short val);//to do


	void SHR(char srcV, char dstV);
	void SHL(char srcV, char dstV);

};