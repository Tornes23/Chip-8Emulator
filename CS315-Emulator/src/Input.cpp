#include "Input.h"
#include "Window.h"

#pragma region // General Input Set Up //
void InputHandler::Initialize()
{
	for (unsigned i = 0; i < KEYBOARD_KEY_AMOUNT; ++i)
	{
		mKeyCurrent[i] = 0;
		mKeyPrevious[i] = 0;
	}
}
void InputHandler::StartFrame()
{
	/* Reset the Current and Previous arrays */
	for (unsigned i = 0; i < KEYBOARD_KEY_AMOUNT; ++i)
		mKeyPrevious[i] = mKeyCurrent[i];
}

#pragma endregion

void InputHandler::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		case SDL_QUIT:
			Window.CloseWindow();
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			InputManager.HandleKeyEvent(event);
			break;
		}
	}
}

#include "imgui/imgui.h"

void InputHandler::HandleKeyEvent(SDL_Event event)
{
	SDL_Keycode ScanCode = event.key.keysym.scancode;

	if (ScanCode > 0 && ScanCode < KEYBOARD_KEY_AMOUNT)
		mKeyCurrent[ScanCode] = event.key.state ? true : false;
}

bool InputHandler::KeyIsDown(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;

	return mKeyCurrent[static_cast<unsigned>(index)];
}
bool InputHandler::KeyIsUp(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;
	return !KeyIsDown(index);
}
bool InputHandler::KeyIsTriggered(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;

	if (mKeyCurrent[static_cast<unsigned>(index)] == true)
	{
		if (mKeyCurrent[static_cast<unsigned>(index)] != mKeyPrevious[static_cast<unsigned>(index)])
			return true;
	}
	return false;
}
bool InputHandler::KeyIsReleased(Key index)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;

	if (mKeyCurrent[static_cast<unsigned>(index)] == false)
	{
		if (mKeyCurrent[static_cast<unsigned>(index)] != mKeyPrevious[static_cast<unsigned>(index)])
			return true;
	}
	return false;
}


