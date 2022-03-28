#include <iostream>
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"
#include "Input.h"
#include "Editor.h"

#undef main
int main(void)
{
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

	return 0;
}