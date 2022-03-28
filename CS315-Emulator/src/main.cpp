#include <iostream>
#include "Utils.h"
#include "Window.h"
#include "Emulator.h"

#undef main
int main(void)
{
	Utils::InitSDL();
	Window.Create();
	Utils::InitGL();

	while (!Window.IsClosed())
		Emulator.Update();

	return 0;
}