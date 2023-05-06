#pragma once
#include "SDL.h"

enum AXIS {HORIZONTAL, VERTICAL};

/**
 * InputHandler class.
 * Singelton class, can be only one Engine at the same time. 
 */
class InputHandler
{
public:
	static InputHandler* GetInstance()    // gives access to the class, returns instance of this class
	{
		//if already created -> return instance, if not -> create instance
		return s_Instanse = (s_Instanse != nullptr) ? s_Instanse : new InputHandler();
	}
	void Listen();
	bool GetKeyDown(SDL_Scancode key);
	int GetAxisKey(AXIS axis);
private:
	InputHandler();
	~InputHandler() {};
	static InputHandler* s_Instanse;
	void KeyUp();
	void KeyDown();
	const Uint8* m_KeyStates; // Array stores key values. 1 = key was pressed, 0 = no. 
};

