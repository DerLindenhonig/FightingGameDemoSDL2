#pragma once

#include "SDL.h"

class Collider
{
public:
	inline SDL_Rect Get() { return m_Box; }
	inline void SetBuffer(int x, int y, int w, int h) // function sets object collider (hitbox), if it changes during animation
	{
		m_Buffer = { x,y,w,h };
	}
	void Set(int x, int y, int w, int h)
	{
		/*x = m_Buffer.x;
		y = m_Buffer.y;
		w = m_Buffer.w;
		h = m_Buffer.h;*/
		m_Box = {
			x - m_Buffer.x,
			y - m_Buffer.y,
			w - m_Buffer.w,
			h - m_Buffer.h
		};
	}

private:
	SDL_Rect m_Box;
	SDL_Rect m_Buffer; // increase space around sprite (for example: attack animation increase character hitbox/hurtbox)
};
