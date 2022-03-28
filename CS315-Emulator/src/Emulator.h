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

	unsigned char mFrameBuffer[64][32];

	Chip8() {}
};


#define Emulator (Chip8::GetInstance())