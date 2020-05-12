#include "StartMenu.h"

//--------------------------------------------
// StartMenu::StartMenu()
//--------------------------------------------
StartMenu::StartMenu()
{
    TTF_Init();
    Font = TTF_OpenFont((char*)"./Data/Joystix.ttf", 16);

    Color = new SDL_Color();
    Color->b = Color->g = Color->r = 255;

    Graphics = SurfaceHandler::Load((char*)"./Graphics/StartMenu.png", false);
    TextLoader = TTF_RenderText_Blended(Font, (char*)"Press Start", *Color);

    IntroAnimation = new AnimDemo();

    ShowMessage = true;
    TimeCounter = SDL_GetTicks();

}

//--------------------------------------------
// StartMenu::Update()
//--------------------------------------------
void StartMenu::Update()
{
    IntroAnimation->Update(); // updates the animation on start menu screen

    if(TimeCounter + 500 < SDL_GetTicks()){
        if(ShowMessage)
            ShowMessage = false;
        else
            ShowMessage = true;
        TimeCounter = SDL_GetTicks();
    }
}

//--------------------------------------------
// StartMenu::StartMenu()
//--------------------------------------------
void StartMenu::HandleEvent()
{
    SDL_Event Event;
    while(SDL_PollEvent(&Event)){
        switch(Event.type){
            case SDL_QUIT:
                NextState = STATE_EXIT;
                break;
            case SDL_KEYDOWN:
                if(Event.key.keysym.sym == SDLK_RETURN || Event.key.keysym.sym == SDLK_KP_ENTER)
                    NextState = STATE_MAIN_GAME;
                break;
        }
    }
    return;
}

//--------------------------------------------
// StartMenu::Render()
//--------------------------------------------
void StartMenu::Render(SDL_Surface* Display)
{
    SurfaceHandler::Draw(Display, Graphics, 0, 0);
    if(ShowMessage) SurfaceHandler::Draw(Display, TextLoader, 150, 250);
    IntroAnimation->Render(Display);

}

//--------------------------------------------
// StartMenu::~StartMenu()
//--------------------------------------------
StartMenu::~StartMenu()
{
    delete IntroAnimation;
    SDL_FreeSurface(Graphics);
    SDL_FreeSurface(TextLoader);
    delete Color;
    TTF_CloseFont(Font);
    TTF_Quit();
}

