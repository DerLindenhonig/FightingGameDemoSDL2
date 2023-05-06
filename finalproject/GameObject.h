#pragma once
#include "IObject.h"
#include "Transform.h"
#include "SDL.h"
#include "Collider.h"

struct Properties
{
public:
	Properties(std::string textureID, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		X = x;
		Y = y;
		Flip = flip;
		Width = width;
		Height = height;
		TextureID = textureID;
	}
	std::string TextureID;
	int Width, Height;
	float X, Y;
	SDL_RendererFlip Flip;
};

class GameObject : public IObject
{
public:
	//GameObject() {};
	GameObject(Properties* p) : m_TextureID(p->TextureID), m_Width(p->Width), m_Height(p->Height), m_Flip(p->Flip)
	{
		m_Transform = new Transform(p->X, p->Y);
	}
	virtual void Draw() = 0;
	virtual void Update(float deltaTime, Collider* collider, bool isGotHit, int health) = 0;
	virtual void Clean() = 0;
protected:
	Transform* m_Transform;
	int m_Width, m_Height;
	std::string m_TextureID;
	SDL_RendererFlip m_Flip;
};
