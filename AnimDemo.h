#ifndef ANIMDEMO_H_INCLUDED
#define ANIMDEMO_H_INCLUDED

// --------------------------------------------------------------------------------------------
// This class is used only for generating the little animation on the Start menu screen
// Not reusable at all, and uses some workarounds for playing with the Player and Ghost
// classes to make them move correctly there.
// --------------------------------------------------------------------------------------------

#include <vector>
#include <SDL/SDL.h>
#include "Player.h"
#include "Ghost.h"
#include "SurfaceHandler.h"

class AnimDemo{

    private:
        Player* Pacman;

        std::vector<Ghost*> Ghosts;

        SDL_Surface* Graphics;

    public:
        AnimDemo();

        void Update();

        void Render(SDL_Surface* Display);

        ~AnimDemo();

};


#endif // ANIMDEMO_H_INCLUDED
