#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Player class, contains basically just the moving direction and wether the pacman is dead
// or not.
// --------------------------------------------------------------------------------------------

#include "Entity.h"
#include "Define.h"

class Player: public Entity{

    public:
        int MovingDirection;

        bool Dead;

    public:
        Player();

        void Update();

        void Render(SDL_Surface* Display, SDL_Surface* Graphics);

        void SetMoving(int Direction);

        void Move(int MoveParam);

        void CheckDead();

        void SetDead(bool IsDead);

        void SetPosition(int X, int Y);

        ~Player();

};


#endif // PLAYER_H_INCLUDED
