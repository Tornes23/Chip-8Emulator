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
	Disassembler disassembler;
	Utils::InitSDL();
	Window.Create(glm::ivec2{Chip8::WIDTH * 5, Chip8::HEIGHT * 5});
	Utils::InitGL();
	InputManager.Initialize();
	Editor.Initialize();
	Chip8 Emulator;

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

	if (disassembler.LoadRom("test_opcode.ch8") < 0)
		return -1;

	while (disassembler.seekpos < disassembler.romSize)
	{
		if (disassembler.GetInstruction().mOpcode == 0x0000)
			break;
	}

	return 0;
}