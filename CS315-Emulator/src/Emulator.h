#pragma once
#include <map>
#include <array>
#include "Disassembler.h"
#include "Input.h"
#include <time.h>
#include <bitset>

class Chip8
{
public:
	//Singleton stuff
	Chip8()
		: mRAM{}
		, mStack{}
		, mV{}
		, mFrameBuffer { {} }
		, mInputKeys { Key::Num1, Key::Num2, Key::Num3, Key::Num4,
					   Key::Q,    Key::W,    Key::E,    Key::R,
					   Key::A,    Key::S,    Key::D,    Key::F,
					   Key::Z,    Key::X,    Key::C,    Key::V,
					 }

	{ 
		std::srand(time(nullptr));
	}

	int LoadRom(std::string rom = "test_opcode.ch8");
	void Update();
	void Render();
	void HandleOpcode(const Opcode& op);

	void Ox8(const Opcode& op);
	void OxF(const Opcode& op);

	static const unsigned MEMORYSIZE = 4096;
	static const unsigned STACKSIZE = 32u;
	static const unsigned REGISTERCOUNT = 16u;
	static const unsigned WIDTH = 64u;
	static const unsigned HEIGHT = 32u;

	std::array<std::bitset<WIDTH>, HEIGHT> mFrameBuffer;
	std::array<Key, 16> mInputKeys;

private:
	Disassembler mDisassembler;
	unsigned char mRAM[MEMORYSIZE];
	unsigned short mStack[STACKSIZE];
	
	unsigned char mV[REGISTERCOUNT];
	unsigned short mPC = 0x200;//starting address
	unsigned short mI = 0;
	unsigned short mSP = 0;

	unsigned char mDT = 0;
	unsigned char mST = 0;

	void CLS();
	void RET();
	void JP(unsigned short addr);
	void JPV0(unsigned short addr);
	void CALL(unsigned short addr);
	void DRW(char vX, char vY, unsigned short size);//to do pa ti Martin juapo

	void SE_VAL(char v, char k);
	void SE_RGSTR(char v0, char v1);
	void SNE_VAL(char v, char k);
	void SNE_RGSTR(char v0, char v1);
	void SKP(char v); 
	void SKNP(char v);

	void LD_VAL(char v, char val);
	void LD_RGSTR(char vX, char vY);
	void LDIN(unsigned short addr);
	void LDVDT(char v);
	void LDVK(char v);
	void LDDTV(char v);
	void LvY(char v);
	void LDFV(char v);
	void LDBV(char v);
	void LDIV(char v);
	void LDVI(char v);

	void OR(char vX, char vY);
	void AND(char vX, char vY);
	void XOR(char vX, char vY);

	void ADD_VAL(char v, char val);
	void ADD_RGSTR(char vX, char vY);
	void ADDI(char v);
	void SUB(char vX, char vY);
	void SUBN(char vX, char vY);
	void RND(char v, char val);


	void SHR(char vX, char vY);
	void SHL(char vX, char vY);

};