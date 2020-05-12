#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

// --------------------------------------------------------------------------------------------
// GameState class holds the structure of all game states, which will inherit from it
// All the functions in there must be virtual to make them usable through your update loop
// --------------------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include "Define.h"

class GameState
{
    public:
        int NextState; // the next state; does nothing if it's STATE_NULL

    public:
        GameState(){ NextState = STATE_NULL; }

        virtual void HandleEvent() = 0;

        virtual void Update() = 0;

        virtual void Render(SDL_Surface* Display) = 0;

        virtual ~GameState(){};

};


#endif // GAMESTATE_H_INCLUDED
