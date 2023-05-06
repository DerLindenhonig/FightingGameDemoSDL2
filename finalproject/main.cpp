/*****************************************************************//**
 * \file   main.cpp
 * \brief  Simple fighting game demo using C++ and SDL2-library.
 * Sources: Madsycode - https://www.youtube.com/watch?v=1KD4Ae0tX0g&list=PL-K0viiuJ2RctP5nlJlqmHGeh66-GOZR_&index=1
 *          Lazyfoo - https://lazyfoo.net/tutorials/SDL/
 *          ArchieG - https://www.youtube.com/watch?v=FIjj6UVXtXk&t=180s
 * 
 * \author Anna Raevskaia
 * \date   May 2023
 *********************************************************************/

// finalproject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Engine.h"
#include "Timer.h"


int main(int argc, char *argv[])
{
    Engine::GetInstance()->Init();

    while (Engine::GetInstance()->isRunning())
    {
        Engine::GetInstance()->Events(); // checks if any event is happening (example: player press the button)
        Engine::GetInstance()->Update();
        Engine::GetInstance()->Render();

        Timer::GetInstance()->Tick();
    }
    // clean all pointers and textures, that stay in the memory
    Engine::GetInstance()->Clean();

    return 0;
}



