#pragma once
#include <vector>
#include <string>
#include "glm/vec2.hpp"
#include "sdl2/SDL.h"

class WindowClass
{
public:
	//Singleton stuff
	WindowClass(WindowClass const&) = delete;
	void operator=(WindowClass const&) = delete;
	static WindowClass& GetInstance()
	{
		static WindowClass instance;
		return instance;
	}

	//member functions
	void Create(glm::ivec2 size = {1280, 720}, std::string title = "CS315 CHIP-8 EMULATOR");
	void Clear();
	void ShutDown();
	void SwapBuffers();
	//setters
	void CloseWindow();
	//getters
	bool IsClosed() const;
	SDL_Window* GetSDLWindow() const;
	SDL_GLContext GetSDLContext() const;
	glm::ivec2 GetViewport() const;

private:
	SDL_Window* mWindow;
	SDL_GLContext mContext;
	std::string mTitle;
	glm::ivec2 mSize;
	bool mClosed;
	WindowClass() {}
};

#define Window (WindowClass::GetInstance())