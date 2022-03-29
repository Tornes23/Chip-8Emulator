#pragma once


class Chip8
{
public:
	//Singleton stuff
	Chip8(Chip8 const&) = delete;
	void operator=(Chip8 const&) = delete;
	static Chip8& GetInstance()
	{
		static Chip8 instance;
		return instance;
	}

	void Update();
	void Render();

private:

	unsigned char mRAM[4096];
	unsigned short mStack[32];
	
	unsigned char mV[16];
	unsigned short mPC = 0;
	unsigned short mI = 0;
	unsigned short mSP = 0;

	unsigned char mDT = 0;
	unsigned char mST = 0;

	bool mFrameBuffer[64][32];

	void SYS(short addr);
	void CLS();
	void RET();
	void JP(short addr);
	void JP(char v0, short addr);
	void CALL(short addr);

	void SE(char v, char k);
	void SE(char v0, char v1);
	void SNE(char v, char k);
	void SNE(char v0, char v1);
	void SKP(char v);
	void SKNP(char v);

	void LD(char v, char val);
	void LD(char srcV, char dstV);
	void LDIN(unsigned short addr);
	void LDVDT(char v);
	void LDVK(char v, char key);
	void LDDTV(char v);
	void LDSTV(char v);
	void LDFV(char v);
	void LDBV(char v);
	void LDIV(char v);
	void LDVI(char v);

	void OR(char srcV, char dstV);
	void AND(char srcV, char dstV);
	void XOR(char srcV, char dstV);

	void ADD(char v, short val);
	void ADD(char srcV, char dstV);
	void ADDI(char v);
	void SUB(char srcV, char dstV);
	void SUBN(char srcV, char dstV);

	void RND(char v, short val);
	void DRW(char vX, char vY, short size);

	void SHR(char srcV, char dstV);
	void SHL(char srcV, char dstV);


	Chip8() {}
};


#define Emulator (Chip8::GetInstance())