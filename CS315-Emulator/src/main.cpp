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
	Chip8 emulator;
	if (emulator.LoadRom() < 0)
		return -1;

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


	return 0;
}