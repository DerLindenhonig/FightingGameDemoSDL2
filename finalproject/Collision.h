#pragma once

#include "SDL.h"
#include "Engine.h"
#include <vector>
#include "Collider.h"

class Collision
{
public:
	bool CheckCollision(SDL_Rect a, SDL_Rect b);
	bool GroundCollision(SDL_Rect a);
	inline static Collision* GetInstance() 
	{
		return s_Instanse = (s_Instanse != nullptr) ? s_Instanse : new Collision();
	}
private:
	Collision() = default;
	static Collision* s_Instanse;
	Collider* m_GroundCollider;
};

