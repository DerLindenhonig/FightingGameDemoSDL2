#pragma once

#include "Character.h"
#include "SpriteAnimation.h"
#include "RigidBody.h"
#include "Collider.h"

#define JUMPING_TIME 12.0f;
#define JUMPING_FORCE 50.0f;
#define ATTACK_TIME 22.0f;

/**
* Fighter class. Playable character.
* The class is responsible for character's movement, animations, controllers (=user's input), hitbox (=collider).
*/
class Fighter : public Character
{
public:
	Fighter(Properties* p, int side); // 0 - left side, 1 - right side
	virtual void Draw();
	virtual void Update(float deltaTime, Collider* collider, bool isGotHit, int health);
	virtual void Clean();
	virtual Collider* GetCollider() { return m_Collider; }
	/**
	 * Returns true if this player hitted another player.
	 * 
	 * \return m_IsHitted
	 */
	bool GetIsHitted() { return m_IsHitted; }
	/**
	 * Returns health of another player.
	 * 
	 * \return m_Health
	 */
	int GetHealth() { return m_Health; }
private:
	SpriteAnimation* m_Animation;
	RigidBody* m_RigitBody;
	/**
	 * This player's collider.
	 */
	Collider* m_Collider;
	/**
	 * 2nd player's collider.
	 */
	Collider* m_2PCollider;
	/**
	 * Saves player's last position, so if character hitted another object, we can find out his position before hit.
	 */
	Vector2D m_LastSavedPosition;

	float m_JumpingForce, m_JumpingTime, m_AttackTime, m_OnHitRecoveryTime;

	int m_Health = 100;
	bool ok = false;   // after on hit -health only one time

	void SetHealth(int health) { m_Health = health; }

	void AnimationState();
	void InitAnimationState();

	bool m_IsJumping;
	bool m_IsFalling;
	bool m_IsGrounded;
	bool m_IsCrouching;
	bool m_IsGoingForward;
	bool m_IsGoingBackward;
	bool m_IsAttacking;
	bool m_IsGotHit;
	bool m_IsBlocked;
	bool m_IsHitted;
	/**
	 * The player is knockout then his health = 0.
	 */
	bool m_IsKO;

	void SetIsHitted(bool isHitted) { m_IsHitted = isHitted; }

	int m_Side;
	void SetSide(int side) { m_Side = side; };
	/**
	 * GetSide return player's side:
	 * 0 - left side
	 * 1 - right side
	 * 
	 * \return player's side
	 */
	int GetSide() { return m_Side; }
};