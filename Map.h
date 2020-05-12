#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include "Player.h"
#include <SDL/SDL.h>
#include <vector>
#include <SDL/SDL_ttf.h>
#include "Ghost.h"
#include "SurfaceHandler.h"
#include "Entity.h"

class Map{

    public:
        Player* Pacman;

        std::vector<Ghost*> Ghosts;

        int Lives;

        SDL_Surface* Background;

        SDL_Surface* Graphics;

        int PlayerCoord[4];

        int Tiles[60][54];

        bool Reset;

        Uint32 ResetTime;


        /** FONT RELATED STUFF **/
        int Score;

        SDL_Surface* TextLoader;

        TTF_Font* Font;

        SDL_Color* Color;
        /** ----------------- **/

    public:
        Map();

        void Initialize();

        int GetTile(int Line, int Column);

        void Update();

        void Render(SDL_Surface* Display);

        void ProcessMovements();

        int CheckMoveValid(int Direction, int TileX, int TileY);

        bool CollisionCheck();

        void KillPlayer();

        void GetPacmanDots();

        void UpdatePlayerCoordinates();

        void UpdateWalkableDirections(Ghost *CurrentGhost, int TileX, int TileY);

        void KeyPress(int Direction);

        ~Map();

};

#endif // MAP_H_INCLUDED
