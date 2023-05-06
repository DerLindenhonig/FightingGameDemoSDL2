#include "InputHandler.h"
#include "Engine.h"

InputHandler* InputHandler::s_Instanse = nullptr; // initialize s_Instanse, because it is static

InputHandler::InputHandler()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void InputHandler::Listen()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Engine::GetInstance()->Quit();
			break;
		case SDL_KEYDOWN:
			KeyDown();
			break;
		case SDL_KEYUP:
			KeyUp();
			break;
		default:
			break;
		}
	}
}

bool InputHandler::GetKeyDown(SDL_Scancode key)
{
	return (m_KeyStates[key] == 1);
}

int InputHandler::GetAxisKey(AXIS axis)
{
	return 0;
}

void InputHandler::KeyUp()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void InputHandler::KeyDown()
{
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}
