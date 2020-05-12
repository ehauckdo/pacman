#ifndef STARTMENU_H_INCLUDED
#define STARTMENU_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Start Menu class, inherits from GameState class
// --------------------------------------------------------------------------------------------

#include "GameState.h"
#include "SurfaceHandler.h"
#include "AnimDemo.h"
#include <SDL/SDL_ttf.h>

class StartMenu: public GameState{

    private:
    // Graphics related stuff
        SDL_Surface* Graphics;

        SDL_Surface* TextLoader;

    // Font related stuff
        TTF_Font* Font;

        SDL_Color* Color;

    //everything else
        AnimDemo* IntroAnimation; // contains the animation displayed on the menu

        bool ShowMessage; // holds wether the "Press Enter" will be displayed or not

        unsigned int TimeCounter; // how long "Press Enter" will be displayed


    public:
        StartMenu();

        void HandleEvent();

        void Update();

        void Render(SDL_Surface* Display);

        ~StartMenu();

};


#endif // STARTMENU_H_INCLUDED
