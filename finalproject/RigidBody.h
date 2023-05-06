#pragma once

#include "Vector2D.h"

#define UNI_MASS 1.0f
#define GRAVITY 10.0f

#define FORWARD_P1 1      // movement for player 1 (left side)
#define BACKWARD_P1 -1

#define FORWARD_P2 1      // movement for player 2 (right side)
#define BACKWARD_P2 -1

#define UPWARD -1		  // jumping
#define DOWNWARD 1

/**
 * Rigidbody class handle physics of the game.
 */
class RigidBody
{
public:
	RigidBody() 
	{
		m_Mass = UNI_MASS;
		m_Gravity = GRAVITY;
	}

	inline void SetMass(float mass) { m_Mass = mass; }
	inline void SetGravity(float gravity) { m_Gravity = gravity; }

	inline void ApplyForce(Vector2D force) { m_Force = force; }
	inline void ApplyForceX(float x) { m_Force.X = x; }
	inline void ApplyForceY(float y) { m_Force.Y = y; }
	inline void UnsetForce() { m_Force = Vector2D(0,0); }

	inline void ApplyFriction(Vector2D v) { m_Friction = v; }
	inline void UnsetFriction() { m_Friction = Vector2D(0, 0); }

	inline float GetMass() { return m_Mass; }
	inline Vector2D GetPosition() { return m_Position; }
	inline Vector2D GetVelocity() { return m_Velocity; }
	inline Vector2D GetAccelaration() { return m_Accelaration; }

	void Update(float deltaTime)
	{
		m_Accelaration.X = (m_Force.X + m_Friction.X) / m_Mass;
		m_Accelaration.Y = m_Gravity + m_Force.Y / m_Mass;
		m_Velocity = m_Accelaration * deltaTime;
		m_Position = m_Velocity * deltaTime;
	}

private:
	float m_Mass, m_Gravity;
	Vector2D m_Force, m_Friction, m_Position, m_Velocity, m_Accelaration; // friction - the effect of something on the character (ice)
};
