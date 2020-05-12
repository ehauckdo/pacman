#ifndef MAINGAME_H_INCLUDED
#define MAINGAME_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Main game class, inherits from GameState class
// It will hold the bidimensional array used as map (will be loaded from file)
// Map was not made into a separate class to make easier the communication between it and
// the entities
// --------------------------------------------------------------------------------------------

#include "GameState.h"
#include <vector>
#include <SDL/SDL_ttf.h>
#include "math.h"
#include "SoundBank.h"
#include "SurfaceHandler.h"
#include "Player.h"
#include "Ghost.h"
#include "BonusMessage.h"

class MainGame: public GameState{
    private:

    // graphics related stuff
        SDL_Surface* Background;

        SDL_Surface* Graphics;

        SDL_Surface* TextLoader;

        SoundBank* SoundControl;

    // font related stuff
        TTF_Font* Font;

        SDL_Color* Color;

    // entities
        Player* Pacman;

         std::vector<Ghost*> Ghosts;

    // map
        int Map[60][54];

    // player related stuff
        int Lives, Score, TopScore, TopScorePos, DotsEaten;

        std::vector<BonusMessage*> Bonus;

    // game system related
        bool Reset;

        Uint32 WaitTime, HoldTime, WeaknessTime, BonusMessageTime;

    public:

        MainGame();

        void TryAgain();

        void RestartGame();

        void ResetEntities();

        void HandleEvent();

        void KeyPress(SDLKey sym, SDLMod mod, Uint16 unicode);

        void Update();

        void Render(SDL_Surface* Display);

        void UpdateMoveDirections(Ghost* CurrentGhost, int TileX, int TileY);

        int IsMoveValid(int Direction, int TileX, int TileY);

        void ProcessMoves();

        void CollisionCheck();

        void PacmanEats();

        void LoadScore();

        void SaveScore();

        ~MainGame();
};

#endif // MAINGAME_H_INCLUDED
