#include "Healthbar.h"
#include "TextureManager.h"
#include <iostream>

Healthbar::Healthbar(int x, int y, int h)
{
	TextureManager::GetInstance()->ParseTextures("assets/textures.xml");
	m_Health = 100;
	m_X = x;
	m_Y = y;
	m_H = h;
}

void Healthbar::Draw()
{
	TextureManager::GetInstance()->Draw("healthbar", m_X, m_Y, 405, m_H);
	TextureManager::GetInstance()->Draw("health", m_X, m_Y, m_Health*4, m_H);
}

void Healthbar::Update(float deltaTime, int health)
{
	if (m_Health > health) 
	{
		m_Health = health;
		TextureManager::GetInstance()->Draw("health", 10, 10, m_Health * 4, 36);
	}
}

void Healthbar::Clean()
{
	TextureManager::GetInstance()->Clean();
}
