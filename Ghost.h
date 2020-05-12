#ifndef GHOST_H_INCLUDED
#define GHOST_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Entitie for the ghosts, inherit from Entity
// Has a lot of important atributes, each of them described bellow
// --------------------------------------------------------------------------------------------

#include "Entity.h"
#include "Define.h"
#include <algorithm>
#include <ctime>
#include <vector>

const int WeaknessTime = 5000;

class Ghost: public Entity{

    public:
        int MovingDirection;

        bool PacmanDead, Weak;

        int State; // 0 - Locked, 1 - Freeing,  2 - Normal, 3 - Running from Pacman, 4 - running to cell

        std::vector<int> WalkableDirections; //stores directions where the ghost can move in an exact moment

        Uint32 FreeTimeCounter; // Counts how long the ghost should be locked;

    private:

        int Color;

        Uint32 WeakTimeCounter; //counts for how long a ghost should be weak after pacman gets a big dot

        Uint32 AiTimeCounter; //counts whenever ghost should AI to change its direction

        int PlayerCoord[2]; // holds the player coordinate at the current time

        bool CanDoAI; // Tells ghost if he should set a new MovingDireciton

    public:

        Ghost(int Color, int TileX,int TileY);

        void Update();

        void Render(SDL_Surface* Display, SDL_Surface* Graphics);

        void Move(int MoveParam);

        void DoAI();

        void StopAI();

        void SetWeakness(bool Weak);

        void CheckWeakness();

        void SetReturnCell(bool Return);

        void UpdatePlayerCoordinates(int TileX, int TileY);

        int GetOppositeDirection();

        void SetPosition(int X, int Y);

        ~Ghost();
};

#endif // GHOST_H_INCLUDED
