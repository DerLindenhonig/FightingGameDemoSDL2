#pragma once
#include <string>
#include "SDL.h"
#include "Animation.h"

/**
 * Animation class - for sprite sheet animation.
 */
class SpriteAnimation : public Animation
{
public:
	SpriteAnimation(bool repeat = true);
	virtual void Update(float deltaTime);
	void Draw(float x, float y, int spriteWidth, int spriteHeight);
	void SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed, SDL_RendererFlip flip, bool repeat);
private:
	int m_SpriteRow, m_SpriteFrame, m_AnimationSpeed, m_FrameCount;
	std::string m_TextureID;
	SDL_RendererFlip m_Flip;
};

