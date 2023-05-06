#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

/**
 * Engine class.
 * Singelton class, can be only one Engine at the same time. 
 * Class makes game loop for handling all game events.
 * Updates. Checks if there was any event happening. Render.
 */
class Engine
{
public:
	static Engine* GetInstance()    // gives access to the class, returns instance of this class
	{
		//if already created -> return instance, if not -> create instance
		return s_Instanse = (s_Instanse != nullptr) ? s_Instanse : new Engine();
	}
	void Update(/*float deltaTime*/);
	void Render();
	void Events();
	bool Init();
	bool Clean();
	void Quit(); // kills all processes

	inline bool isRunning() { return m_IsRunning; }
	inline SDL_Renderer* GetRenderer() { return m_Renderer; }
	inline bool isGameEnded() { return m_IsGameEnded; }
	inline std::string GetWinString() { return m_WinString; }
private:
	Engine() = default;
	~Engine() {};
	static Engine* s_Instanse;
	bool m_IsRunning;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	float m_DeltaTime;
	bool m_IsGameEnded;
	std::string m_WinString;
};