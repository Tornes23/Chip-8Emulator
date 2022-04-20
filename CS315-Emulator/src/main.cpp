#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"
#include "Disassembler.h"
#include "Input.h"
#include "Editor.h"
#include "Shader.h"
#include "Renderer.h"

#undef main
int main(void)
{
	Chip8 emulator;
	if (emulator.LoadRom() < 0)
		return -1;

	Utils::InitSDL();
	Window.Create(glm::ivec2{ Chip8::WIDTH * 8, Chip8::HEIGHT * 8});
	Utils::InitGL();

	Renderer GFX;
	GFX.initialize();
	
	InputManager.Initialize();
	//Editor.Initialize();
	Chip8 Emulator;

	Emulator.mFrameBuffer[Chip8::HEIGHT / 2][Chip8::WIDTH / 2] = true;

	while (!Window.IsClosed())
	{
		//Editor.StartFrame();
		InputManager.StartFrame();
		InputManager.HandleEvents();
		
		//Window.Update();
		Emulator.Update();
		GFX.render();
		GFX.update(Emulator.mFrameBuffer);
	
		Window.Clear();
		//Editor.Render();
		Window.SwapBuffers();
	
	}
	
	Window.ShutDown();
	//Editor.ShutDown();
	SDL_Quit();

	return 0;
}