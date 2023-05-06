#pragma once

const int TARGET_FPS = 60; // FPS = 60 frames per second
const float TARGET_DELTATIME = 1.5f; // 1.5 milliseconds

/**
 * Timer class. Singelton class, can be only one Timer at the same time.
 * Deltatime = differens in time between events.
 * Deltatime value might be varied in different computers. 
 * Here we are checking that animation is consistent.
 */
class Timer
{
public:
	static Timer* GetInstance()    // gives access to the class, returns instance of this class
	{
		//if already created -> return instance, if not -> create instance
		return s_Instanse = (s_Instanse != nullptr) ? s_Instanse : new Timer();
	}
	void Tick();
	inline float GetDeltaTime() { return m_DeltaTime; }

private:
	Timer();
	static Timer* s_Instanse;
	float m_DeltaTime = 0.0f, m_LastTime = 0.0f;
};

