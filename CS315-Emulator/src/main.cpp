#include <iostream>
#include "Utils.h"
#include "Window.h"

int main(void)
{
	Utils::InitSDL();
	Window.Create();
	Utils::InitGL();


	return 0;
}