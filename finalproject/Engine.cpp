#include "Engine.h"
#include <iostream>
#include "TextureManager.h"
#include "Transform.h"
#include "Fighter.h"
#include "InputHandler.h"
#include "Timer.h"
#include "Healthbar.h"
#include <SDL_ttf.h>
#include "Text.h"

Engine* Engine::s_Instanse = nullptr; // initialize s_Instanse, because it is static
Fighter* fighter = nullptr;
Fighter* fighter2 = nullptr;
Healthbar* healthbar = nullptr;
Healthbar* healthbar2 = nullptr;

void Engine::Update()
{
	float deltaTime = Timer::GetInstance()->GetDeltaTime();
	fighter->Update(deltaTime, fighter2->GetCollider(), fighter2->GetIsHitted(), fighter2->GetHealth());
	fighter2->Update(deltaTime, fighter->GetCollider(), fighter->GetIsHitted(), fighter->GetHealth());

	healthbar->Update(deltaTime, fighter->GetHealth());
	healthbar2->Update(deltaTime, fighter2->GetHealth());

	if (fighter->GetHealth() <= 0)
	{
		m_IsGameEnded = true;
		m_WinString = "PLAYER 2 WIN";
	}
	if (fighter2->GetHealth() <= 0)
	{
		m_IsGameEnded = true;
		m_WinString = "PLAYER 1 WIN";
	}
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_Renderer, 204, 204, 204, 204);
	SDL_RenderClear(m_Renderer);

	TextureManager::GetInstance()->Draw("ground", 0, 568, 960, 72); // render static image

	fighter->Draw();
	fighter2->Draw();
	healthbar->Draw();
	healthbar2->Draw();

	// print text:
	if ((SDL_GetTicks() / 1000) < 10)
	{
		std::string timeString = "0" + std::to_string(SDL_GetTicks() / 1000);

		Text text(GetRenderer(), "assets/RobotoCondensed-Bold.ttf", 50, timeString, { 255, 0, 0, 255 });
		text.Display(453, 20, GetRenderer());
	}
	else if ((SDL_GetTicks() / 1000) > 15 && m_IsGameEnded == false)
	{
		Text text2(GetRenderer(), "assets/RobotoCondensed-Bold.ttf", 100, "TIME UP", { 255, 0, 0, 255 });
		text2.Display(350, 250, GetRenderer());

		if (fighter->GetHealth() < fighter2->GetHealth()) {
			m_WinString = "PLAYER 2 WIN";
		} else { m_WinString = "PLAYER 1 WIN"; }

		m_IsGameEnded = true;
	}
	else if ((SDL_GetTicks() / 1000) >= 10)
	{
		Text text(GetRenderer(), "assets/RobotoCondensed-Bold.ttf", 50, std::to_string(SDL_GetTicks() / 1000), { 255, 0, 0, 255 });
		text.Display(453, 20, GetRenderer());
	}


	if (m_IsGameEnded == true)
	{
		Text text3(GetRenderer(), "assets/RobotoCondensed-Bold.ttf", 100, GetWinString(), { 255, 0, 0, 255 });
		text3.Display(250, 250, GetRenderer());
	}

	SDL_RenderPresent(m_Renderer);
}

void Engine::Events()
{
	InputHandler::GetInstance()->Listen(); // listen for player inputs
}

bool Engine::Init()
{
	//initialize SDL:
	if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//create window:
	m_Window = SDL_CreateWindow("WindowTitle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0); // last one is fullscreen
	if (m_Window == nullptr)
	{
		SDL_Log("Failed to create a window: %s", SDL_GetError());
	}

	//create renderer:
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr)
	{
		SDL_Log("Failed to create a render: %s", SDL_GetError());
	}
	
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf\n";
	}

	TextureManager::GetInstance()->ParseTextures("assets/textures.xml");

	fighter = new Fighter(new Properties("character", 100, 200, 295, 325), 0);
	fighter2 = new Fighter(new Properties("character", 500, 200, 295, 325), 1);

	healthbar = new Healthbar(50, 10, 36);
	healthbar2 = new Healthbar(500, 10, 36);

	m_IsRunning = true;
	return m_IsRunning;
}

bool Engine::Clean()
{
	TextureManager::GetInstance()->Clean();
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
	IMG_Quit();
	SDL_Quit();
	return false;
}

void Engine::Quit()
{
	m_IsRunning = false; // stop the game loop
}

