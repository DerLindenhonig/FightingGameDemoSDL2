#pragma once

#include "GameObject.h"
#include <string>
#include "Collider.h"

class Character : public GameObject
{
public:
	//Character() {};
	Character(Properties *p) : GameObject(p) {}
	virtual void Draw() = 0;
	virtual void Update(float deltaTime, Collider* collider, bool isGotHit, int health) = 0;
	virtual void Clean() = 0;
protected:
	std::string n_Name;
};
