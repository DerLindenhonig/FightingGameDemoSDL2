#pragma once

/**
 * Healthbar holds player's health.
 */
class Healthbar
{
public:
	Healthbar(int x, int y, int h);
	void Draw();
	void Update(float deltaTime, int health);
	void Clean();
private:
	int m_Health;
	void SetHealth(int health) { m_Health = health; }
	int m_X = 0, m_Y = 0, m_H = 0;
};

