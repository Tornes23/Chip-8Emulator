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
#include <bitset>
#include <thread>

#undef main
int main(void)
{
	Chip8 emulator;
	if (emulator.LoadRom("petdog.ch8") < 0)
		return -1;

	Utils::InitSDL();
	Window.Create(glm::ivec2{ Chip8::WIDTH * 8, Chip8::HEIGHT * 8});
	Utils::InitGL();

	Renderer GFX;
	GFX.initialize();
	
	InputManager.Initialize();
	//Editor.Initialize();

	while (!Window.IsClosed())
	{
		//Editor.StartFrame();
		InputManager.StartFrame();
		InputManager.HandleEvents();
		
		//Window.Update();
		emulator.Update();
		GFX.update(emulator.mFrameBuffer);
		Window.Clear();
		GFX.render();
	
		Window.SwapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		//Editor.Render();
	}
	
	Window.ShutDown();
	//Editor.ShutDown();
	SDL_Quit();

	return 0;
}