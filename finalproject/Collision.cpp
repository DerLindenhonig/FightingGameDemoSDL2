#include "Collision.h"
#include <iostream>

/**
 * Checks if SDL_Rect "a" touches another SDL_Rect "b".
 * 
 * \param a
 * \param b
 * \return true/false
 */
Collision* Collision::s_Instanse = nullptr; // initialize s_Instanse, because it is static

bool Collision::CheckCollision(SDL_Rect a, SDL_Rect b)
{
    return SDL_HasIntersection(&a, &b);
}

/**
 * Checks if SDL_Rect "a" touches ground.
 * 
 * \param a
 * \return true/false
 */
bool Collision::GroundCollision(SDL_Rect a)
{
    m_GroundCollider = new Collider();
    m_GroundCollider->Set(0, 568, 960, 72); // create ground collider
    m_GroundCollider->SetBuffer(0, 0, 0, 0);

    //std::cout << "Hit " << SDL_HasIntersection(&a, &m_GroundCollider->Get()) << "\n";
    //std::cout << "m_GroundCollider->Get().x: " << m_GroundCollider->Get().x << " y:" << m_GroundCollider->Get().y << " w: " << m_GroundCollider->Get().w << " h: " << m_GroundCollider->Get().h << "\n";

    return SDL_HasIntersection(&a, &m_GroundCollider->Get());
}
