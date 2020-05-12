/*
==============================================================================================

Pacman Clone with C++/SDL 1.2

This is a work in progress developed by Hauck in April 2014
It's released under no license or copyright and was developed for no comercial use
It's for studies purpose only
All licensed material used here are owned by Namco and no violation is intended
You can get in touch with me through: eduardohauck@gmail.com

==============================================================================================
*/

#include "Pacman.h"

//--------------------------------------------
//  Pacman::Pacman()
//--------------------------------------------
Pacman::Pacman()
{
    Display = NULL;
    CurrentState = NULL;
}

//--------------------------------------------
//  Pacman::LoadResources()
//--------------------------------------------
bool Pacman::LoadResources()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    SDL_Surface *Icon;
    Icon = SDL_LoadBMP("./Graphics/Icon.bmp");
    Uint32 ColorKey;
    ColorKey = SDL_MapRGB(Icon->format, 255, 255, 255);
    SDL_SetColorKey(Icon, SDL_SRCCOLORKEY, ColorKey);
    SDL_WM_SetIcon(Icon,NULL);


    if((Display = SDL_SetVideoMode(440, 512, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
        return false;

    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

    SDL_WM_SetCaption("Pacman", "Pacman");
    CurrentState = new StartMenu();
    StateID = STATE_NULL;

    return true;
}

// ----------------------------------------------------------------------------------
// Pacman::Execute()
//
// Important notes:
// -Start will hold the time the loop actually began. With this, will can see how faster
// Everything was done, and how much we should sleep at the end, according to how much frames we want
// the game to go per second. This is calculated by FPS class.
// -Each state holds a Sleep boolean, which tells if the windows was being dragged or not
// (and thus if the timer was paused or not, which can mess FPS calculations)
// -----------------------------------------------------------------------------------
int Pacman::Execute()
{
        if(LoadResources() == false)
            return -1;

        while(StateID != STATE_EXIT){

            int Start = SDL_GetTicks();

            CurrentState->HandleEvent();

            CurrentState->Update();

            UpdateState();

            CurrentState->Render(Display);

            FPS::FPSControl.Update();

            SDL_Flip(Display);


            while(FPS::FPSControl.GetTimePerFrame() > SDL_GetTicks() - Start){
                SDL_Delay(FPS::FPSControl.GetTimePerFrame() - (SDL_GetTicks() - Start));
            }

        }

        return 0;
}

// ----------------------------------------------------------------------------------
// Pacman::UpdateState()
//
// Important ntoes:
// -Before deleting the CurrentState, we first check if the NextStateID IS NOT exit state
// that is done because we still have to complete our Update() loop, which includes rendering
// so we keep our CurrentState for one more loop before exiting the game
// ----------------------------------------------------------------------------------
void Pacman::UpdateState()
{
    if(CurrentState->NextState != STATE_NULL){

        int NextStateID = CurrentState->NextState;

        if(NextStateID != STATE_EXIT)
            delete CurrentState;

        switch(NextStateID){

            case STATE_START_MENU:
                CurrentState = new StartMenu();
                break;

            case STATE_MAIN_GAME:
                CurrentState = new MainGame();
                break;

        }

        StateID = NextStateID;

    }
}

//--------------------------------------------
//  Pacman::~Pacman()
//--------------------------------------------
Pacman::~Pacman()
{
    delete CurrentState;
    SDL_FreeSurface(Display);
}

//--------------------------------------------
//  main()
//  (couldn't find any better place for this)
//--------------------------------------------
int main(int argc, char* argv[]) {

    Pacman Game;

    return Game.Execute();
}
