#include "Fighter.h"
#include "TextureManager.h"
#include "SDL.h"
#include "InputHandler.h"
#include "Engine.h"
#include "Collision.h"

/**
 * Constructor. Initialize health, animations, side, rigidbody and collider.
 * 
 * \param p - properties
 * \param side - player's side (0 = left/1 = right)
 */
Fighter::Fighter(Properties* p, int side) : Character(p)
{
	SetSide(side);
	InitAnimationState();

	m_Health = 100;

	m_Animation = new SpriteAnimation();
	//m_Animation->SetProps(m_TextureID, 0, 2, 100, SDL_FLIP_NONE, true);

	m_RigitBody = new RigidBody();
	m_RigitBody->SetGravity(10.0f); // define how fast character fall down

	m_Collider = new Collider();
	m_Collider->SetBuffer(-50, -10, 130, 10); // changes collider's size

	m_JumpingTime = JUMPING_TIME;        // values in Fighter.h
	m_JumpingForce = JUMPING_FORCE;
}

void Fighter::Draw()
{
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
	
	SDL_Rect hitbox = m_Collider->Get();
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &hitbox);
}

/**
 * Listen for player's input. Updates character's position, animations depend on player's input.
 * 
 * \param deltaTime
 * \param collider - another player's collider (hitbox)
 * \param isGotHit - check if another player hitted this player
 * \param health
 */
void Fighter::Update(float deltaTime, Collider* collider, bool isGotHit, int health) // not got hit, but 2nd player hitted this player = true
{
	// if nothing pressed or key was relesed -> set idle animation + unset movement:
	m_IsGoingForward = false;
	m_IsGoingBackward = false;
	m_IsCrouching = false;

	m_RigitBody->UnsetForce(); // don't move

	if (m_Side == 0 && !m_IsKO) 
	{
		//std::cout << "P1 healthbar: " << m_Health /*<< ", P2 return health:" << health*/ << "\n";
		if (m_Health <= 0) { m_IsKO = true; }

		// P1 moving forward / backward using "A" and "D" keys:
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_A) && !m_IsAttacking && !m_IsGotHit)
		{
			m_RigitBody->ApplyForceX(5 * BACKWARD_P1); // moving backward + speed of moving
			m_IsGoingBackward = true;
		}

		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_D) && !m_IsAttacking && !m_IsGotHit)
		{
			m_RigitBody->ApplyForceX(7 * FORWARD_P1); // moving forward + speed of moving
			m_IsGoingForward = true;
		}

		// jumping:
		// see gravity in this file, in Fighter::Fighter(Properties* p) : Character(p) function
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsGrounded && !m_IsGotHit)
		{
			m_IsJumping = true;
			m_IsGrounded = false;
			m_RigitBody->ApplyForceY(UPWARD * m_JumpingForce);
		}
		// falling after jump
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsJumping && m_JumpingTime > 0)
		{
			m_JumpingTime -= deltaTime;
			m_RigitBody->ApplyForceY(UPWARD * m_JumpingForce);
		}
		else {
			m_IsJumping = false;
			m_JumpingTime = JUMPING_TIME;
		}

		// falling
		if (m_RigitBody->GetVelocity().Y > 0 && !m_IsGrounded) { m_IsFalling = true; }
		else { m_IsFalling = false; }

		// attacking
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_Q) && !m_IsJumping && !m_IsFalling && !m_IsGotHit)
		{
			m_RigitBody->UnsetForce();
			m_Collider->SetBuffer(-50, -10, 60, 10);
			m_IsAttacking = true;
		}

		// time for attack
		if (m_IsAttacking && m_AttackTime > 0)
		{
			m_AttackTime -= deltaTime;
		}
		else
		{
			//ok = false;
			m_IsAttacking = false;
			m_Collider->SetBuffer(-50, -10, 130, 10);
			m_AttackTime = ATTACK_TIME;
		}

		// crouching
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_S))
		{
			m_RigitBody->UnsetForce();
			m_IsCrouching = true;
		}

		// move on X-axis
		m_RigitBody->Update(deltaTime);
		m_LastSavedPosition.X = m_Transform->X;
		m_Transform->X += m_RigitBody->GetPosition().X;
		m_Collider->Set(m_Transform->X, m_Transform->Y, 295, 325);

		// if touching ground -> return last saved position
		//if (Collision::GetInstance()->CheckCollision(m_Collider->Get(), m_GroundCollider->Get())) // for 2 objects
		if (Collision::GetInstance()->GroundCollision(m_Collider->Get()))
		{
			m_Transform->X = m_LastSavedPosition.X;
		}

		// if touching 2 player -> return last saved position
		if (Collision::GetInstance()->CheckCollision(m_Collider->Get(), collider->Get()))
		{
			m_Transform->X = m_LastSavedPosition.X;
		}

		// print SDL_Rect (x,y,w,h) of object for testing it's location:
		//std::cout << "fighter.cpp m_Collider->Get().x: " << m_Collider->Get().x << " y:" << m_Collider->Get().y << " w: " << m_Collider->Get().w << " h: " << m_Collider->Get().h << "\n";

		// move on Y-axis    
		m_RigitBody->Update(deltaTime);
		m_LastSavedPosition.Y = m_Transform->Y;
		m_Transform->Y += m_RigitBody->GetPosition().Y;
		m_Collider->Set(m_Transform->X, m_Transform->Y, 295, 325);

		// check if touching ground, if true -> stop moving down
		if (Collision::GetInstance()->GroundCollision(m_Collider->Get()))
		{
			m_IsGrounded = true;
			m_Transform->Y = m_LastSavedPosition.Y;
		}
		else { m_IsGrounded = false; }

		// check if touching 2 player, if true -> stop moving down
		if (Collision::GetInstance()->CheckCollision(m_Collider->Get(), collider->Get()))
		{
			//m_IsGrounded = true;
			m_Transform->X = m_LastSavedPosition.X;

			if (m_IsAttacking)
			{
				SetIsHitted(true);
			}
		}
	}


	if (m_Side == 1 && !m_IsKO)
	{
		//std::cout << "                    P2 healthbar: " << m_Health /*<< ", P1 return health:" << health*/ << "\n";
		if (m_Health <= 0) { m_IsKO = true; }

		// P1 moving forward / backward using "A" and "D" keys:
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT) && !m_IsAttacking && !m_IsGotHit)
		{
			m_RigitBody->ApplyForceX(5 * BACKWARD_P1); // moving backward + speed of moving
			//m_Animation->SetProps(m_TextureID, 2, 3, 100/*, SDL_FLIP_HORIZONTAL*/);
			m_IsGoingForward = true;
		}

		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT) && !m_IsAttacking && !m_IsGotHit)
		{
			m_RigitBody->ApplyForceX(7 * FORWARD_P1); // moving forward + speed of moving
			//m_Animation->SetProps(m_TextureID, 1, 3, 100/*, SDL_FLIP_HORIZONTAL*/);
			m_IsGoingBackward = true;
		}

		// jumping:
		// see gravity in this file, in Fighter::Fighter(Properties* p) : Character(p) function
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_UP) && m_IsGrounded && !m_IsGotHit)
		{
			m_IsJumping = true;
			m_IsGrounded = false;
			m_RigitBody->ApplyForceY(UPWARD * m_JumpingForce);
			//m_Animation->SetProps(m_TextureID, 1, 3, 100/*, SDL_FLIP_HORIZONTAL*/);
		}
		// falling after jump
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_UP) && m_IsJumping && m_JumpingTime > 0)
		{
			m_JumpingTime -= deltaTime;
			m_RigitBody->ApplyForceY(UPWARD * m_JumpingForce);
		}
		else {
			m_IsJumping = false;
			m_JumpingTime = JUMPING_TIME;
		}

		// falling
		if (m_RigitBody->GetVelocity().Y > 0 && !m_IsGrounded) { m_IsFalling = true; }
		else { m_IsFalling = false; }

		// attacking
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_M) && !m_IsJumping && !m_IsFalling && !m_IsGotHit)
		{
			m_RigitBody->UnsetForce();
			m_IsAttacking = true;
			m_Collider->SetBuffer(-10, -10, 130, 10);
		}

		// time for attack
		if (m_IsAttacking && m_AttackTime > 0)
		{
			m_AttackTime -= deltaTime;
		}
		else
		{
			m_IsAttacking = false;
			m_Collider->SetBuffer(-50, -10, 130, 10);
			m_AttackTime = ATTACK_TIME;
		}

		// crouching
		if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN))
		{
			m_RigitBody->UnsetForce();
			m_IsCrouching = true;
		}

		// move on X-axis
		m_RigitBody->Update(deltaTime);
		m_LastSavedPosition.X = m_Transform->X;
		m_Transform->X += m_RigitBody->GetPosition().X;
		m_Collider->Set(m_Transform->X, m_Transform->Y, 295, 325);

		// if touching ground -> return last saved position
		if (Collision::GetInstance()->GroundCollision(m_Collider->Get()))
		{
			m_Transform->X = m_LastSavedPosition.X;
		}

		// if touching 2 player -> return last saved position
		if (Collision::GetInstance()->CheckCollision(m_Collider->Get(), collider->Get()))
		{
			m_Transform->X = m_LastSavedPosition.X;
		}

		// print SDL_Rect (x,y,w,h) of object for testing it's location:
		//std::cout << "fighter.cpp m_Collider->Get().x: " << m_Collider->Get().x << " y:" << m_Collider->Get().y << " w: " << m_Collider->Get().w << " h: " << m_Collider->Get().h << "\n";

		// move on Y-axis    
		m_RigitBody->Update(deltaTime);
		m_LastSavedPosition.Y = m_Transform->Y;
		m_Transform->Y += m_RigitBody->GetPosition().Y;
		m_Collider->Set(m_Transform->X, m_Transform->Y, 295, 325);

		// check if touching ground, if true -> stop moving down
		if (Collision::GetInstance()->GroundCollision(m_Collider->Get()))
		{
			m_IsGrounded = true;
			m_Transform->Y = m_LastSavedPosition.Y;
		}
		else { m_IsGrounded = false; }

		// check if touching 2 player, if true -> stop moving down
		if (Collision::GetInstance()->CheckCollision(m_Collider->Get(), collider->Get()))
		{
			//m_IsGrounded = true;
			m_Transform->X = m_LastSavedPosition.X;

			if (m_IsAttacking) 
			{  
				SetIsHitted(true);
			}
		}
	}

	// check if got hit:
	if (isGotHit == true)
	{
		if (m_IsGoingBackward == true)
		{
			m_IsBlocked = true;
			if (ok == false) { ok = true; }
		}
		else if (m_IsGoingBackward == false) {
			m_IsGotHit = true;
			if (ok == false) { m_Health = m_Health - 10; ok = true; }
		}
		//std::cout << "P2 Got Hit!     m_IsHitted: " << m_IsHitted << ", isGotHit: " << isGotHit << "\n";
	}
	else {
		m_IsGotHit = false;
		m_IsBlocked = false;
		ok = false;
		//std::cout << "P2 not got hit! m_IsHitted: " << m_IsHitted << ", isGotHit: " << isGotHit << "\n";
	}

	// time for on hit animation
	if (m_IsHitted && m_OnHitRecoveryTime > 0)
	{
		m_OnHitRecoveryTime -= deltaTime;
	}
	else
	{
		m_OnHitRecoveryTime = 6.0f;
		SetIsHitted(false);
	}

	//m_Transform->TranslateX(m_RigitBody->GetPosition().X); // moving forward by x-axis
	//m_Transform->TranslateY(m_RigitBody->GetPosition().Y); // moving down by y-axis

	AnimationState();
	
	m_Animation->Update(deltaTime);
}

void Fighter::Clean()
{
	TextureManager::GetInstance()->Clean();
}

/**
* AnimationState function is responsible for switching animations depending on fighter's state.
* 
 * In separate animation function, we pass these parameters:
 * m_Row - row in animation sheet (count from 0 - ...)
 * m_FrameCount - how mamy frames in animation (count from 1 - ...)
 * m_Speed - delay between frames (= animation speed)
 * m_Flip - SDL_FLIP_NONE / SDL_FLIP_HORIZONTAL
 * m_Repeat - is animation looped or not 
 */
void Fighter::AnimationState()
{
	if (m_Side == 0) {
		// idle animation:
		m_Animation->SetProps(m_TextureID, 0, 2, 100, SDL_FLIP_NONE, true);

		// states:
		if (m_IsGoingForward) { m_Animation->SetProps(m_TextureID, 1, 3, 150, SDL_FLIP_NONE, true); }
		if (m_IsGoingBackward) { m_Animation->SetProps(m_TextureID, 2, 3, 150, SDL_FLIP_NONE, true); }
		if (m_IsJumping) { m_Animation->SetProps(m_TextureID, 4, 1, 150, SDL_FLIP_NONE, true); }
		if (m_IsFalling) { m_Animation->SetProps(m_TextureID, 4, 1, 150, SDL_FLIP_NONE, true); }
		if (m_IsCrouching) { m_Animation->SetProps(m_TextureID, 3, 1, 150, SDL_FLIP_NONE, true); }
		if (m_IsAttacking) { m_Animation->SetProps(m_TextureID, 6, 3, 150, SDL_FLIP_NONE, false); }
		if (m_IsBlocked) { m_Animation->SetProps(m_TextureID, 7, 1, 150, SDL_FLIP_NONE, false); }
		if (m_IsGotHit) { m_Animation->SetProps(m_TextureID, 8, 3, 150, SDL_FLIP_NONE, false); }
		if (m_IsKO) { m_Animation->SetProps(m_TextureID, 9, 3, 150, SDL_FLIP_NONE, false); }
	}
	else if (m_Side == 1) {
		// idle animation:
		m_Animation->SetProps(m_TextureID, 0, 2, 100, SDL_FLIP_HORIZONTAL, true);

		// states:
		if (m_IsGoingForward) { m_Animation->SetProps(m_TextureID, 1, 3, 150, SDL_FLIP_HORIZONTAL, true); }
		if (m_IsGoingBackward) { m_Animation->SetProps(m_TextureID, 2, 3, 150, SDL_FLIP_HORIZONTAL, true); }
		if (m_IsJumping) { m_Animation->SetProps(m_TextureID, 4, 1, 150, SDL_FLIP_HORIZONTAL, true); }
		if (m_IsFalling) { m_Animation->SetProps(m_TextureID, 4, 1, 150, SDL_FLIP_HORIZONTAL, true); }
		if (m_IsCrouching) { m_Animation->SetProps(m_TextureID, 3, 1, 150, SDL_FLIP_HORIZONTAL, true); }
		if (m_IsAttacking) { m_Animation->SetProps(m_TextureID, 6, 3, 150, SDL_FLIP_HORIZONTAL, false); }
		if (m_IsGotHit) { m_Animation->SetProps(m_TextureID, 8, 3, 150, SDL_FLIP_HORIZONTAL, false); }
		if (m_IsBlocked) { m_Animation->SetProps(m_TextureID, 7, 1, 150, SDL_FLIP_HORIZONTAL, false); }
		if (m_IsKO) { m_Animation->SetProps(m_TextureID, 9, 3, 150, SDL_FLIP_HORIZONTAL, false); }
	}
}

/**
 * Initialize states.
 */
void Fighter::InitAnimationState()
{
	m_IsJumping = false;
	m_IsFalling = false;
	m_IsGrounded = false;
	m_IsCrouching = false;
	m_IsGoingForward = false;
	m_IsGoingBackward = false;
	m_IsAttacking = false;
	m_IsGotHit = false;
	m_IsKO = false;
	m_IsGotHit = false;
	m_IsBlocked = false;
	m_IsHitted = false;
	ok = false;

	if (m_Side == 0) {
		m_Flip = SDL_FLIP_NONE;
	}
	else if (m_Side == 1) {
		m_Flip = SDL_FLIP_HORIZONTAL;
	}
	
	m_JumpingForce = JUMPING_FORCE;
	m_JumpingTime = JUMPING_TIME;
	m_AttackTime = ATTACK_TIME;
}
