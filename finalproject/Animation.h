#pragma once
class Animation
{
public:
	Animation(bool repeat = true) : m_Repeat(repeat) { m_IsEnded = false; }
	virtual void Update(float deltaTime) = 0;
	inline bool IsEnded() { return m_IsEnded; }
protected:
	bool m_Repeat; // loop animation or not
	bool m_IsEnded;
	int m_CurrentFrame = 0;
};