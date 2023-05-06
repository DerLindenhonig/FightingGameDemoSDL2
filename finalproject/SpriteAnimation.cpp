#include "SpriteAnimation.h"
#include "TextureManager.h"

SpriteAnimation::SpriteAnimation(bool repeat) : Animation(repeat)
{
}

/**
 * Update animation depending if it is looped or not.
 * 
 * \param deltaTime
 */
void SpriteAnimation::Update(float deltaTime)
{
	if (m_Repeat || !m_IsEnded) {
		m_IsEnded = false;
		m_CurrentFrame = (SDL_GetTicks() / m_AnimationSpeed) % m_FrameCount;
	}

	if (!m_Repeat && m_CurrentFrame == (m_FrameCount - 1)) {
		m_IsEnded = true;
		m_CurrentFrame = (m_FrameCount - 1);
	}
}

void SpriteAnimation::Draw(float x, float y, int spriteWidth, int spriteHeight)
{
	//TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, spriteWidth, spriteHeight, m_SpriteRow, m_SpriteFrame, m_Flip);
	TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, spriteWidth, spriteHeight, m_SpriteRow, m_CurrentFrame, m_Flip);
}

void SpriteAnimation::SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed, SDL_RendererFlip flip, bool repeat)
{
	m_TextureID = textureID;
	m_SpriteRow = spriteRow;
	m_FrameCount = frameCount;
	m_AnimationSpeed = animSpeed;
	m_Flip = flip;
	m_Repeat = repeat;
}
