#pragma once

#include "Collider.h"

class IObject
{
public:
	virtual void Draw() = 0;
	virtual void Update(float deltaTime, Collider* collider, bool isGotHit, int health) = 0;
	virtual void Clean() = 0;
};
