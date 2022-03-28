#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"
#include "Disassembler.h"
#include "Input.h"
#include "Editor.h"

#undef main
int main(void)
{
	/*
	Utils::InitSDL();
	Window.Create();
	Utils::InitGL();
	InputManager.Initialize();
	Editor.Initialize();

	while (!Window.IsClosed())
	{
		Editor.StartFrame();
		InputManager.StartFrame();
		InputManager.HandleEvents();
		
		Window.Update();
		Emulator.Update();

		Window.Clear();
		Editor.Render();
		Window.SwapBuffers();

	}

	Window.ShutDown();
	Editor.ShutDown();
	SDL_Quit();
	*/

	std::ifstream f;
	f.open("test_opcode.ch8", std::ios::binary | std::ios::in);

	if (f.bad())
	{
		return 1;
	}

	char buf[2];
	f.seekg(0, f.end);
	int size = f.tellg();
	f.seekg(0, f.beg);
	int seekpos = 0;

	while (seekpos < size)
	{
		f.read(buf, 2);
		std::swap(buf[0], buf[1]); // little endian conversor
		uint16_t opcode = *reinterpret_cast<uint16_t*>(buf);
		
		std::cout << std::hex << seekpos << ": ";
		Disassembler::PrintOpcode(opcode);
		std::cout << std::endl;

		seekpos += 2;
	}

	return 0;
}