/*
==============================================================================================

Pacman Clone with C++/SDL 1.2

This is a work in progress developed by Hauck in April 2014.
It's released under no license or copyright and was developed for no comercial use.
It's for studies purpose only.
All licensed material used here are owned by Namco and no copyright violation is intended.
You can get in touch with me through: eduardohauck@gmail.com

This is a work in progress. Many bugs are still known and need to be fixed. This description
may contain regulary updates and fixes.

23th April - First version code release

Known bugs:

#FIXED @Sometimes the FPS::GetTimePerFrame gets messed, making the game too slow. This is due to the
implementations made to guarantee it won't bug if the window is dragged, but it's still not okay.

Future releases:

#FIXED @Separation between public and private atributes. Almost everything is set as public now;

@Fruits and Life bonus need to be implemented;

@Improvements on the AI need to be made. If one understands the algorithm,
can easily trick the ghosts.
==============================================================================================
*/


#ifndef PACMAN_H_INCLUDED
#define PACMAN_H_INCLUDED

// --------------------------------------------------------------------------------------------
//  This is the main class of the game. It holds the current state of the game
//  and updates it at every loop.
//  It's working pretty ok and it's based on LazyFoo's state machine tutorials
//  http://lazyfoo.net/articles/article06/index.php
//  No changes should be made here
// --------------------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include "Define.h"
#include "GameState.h"
#include "StartMenu.h"
#include "MainGame.h"
#include "FPS.h"

class Pacman{
    private:
        SDL_Surface* Display;

        GameState* CurrentState;

        int StateID;

    public:
        Pacman();

        int Execute();

        bool LoadResources();

        void UpdateState();

        ~Pacman();
};

#endif // PACMAN_H_INCLUDED
