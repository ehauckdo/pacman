#include "AnimDemo.h"

//---------------------------------------------------------------------------
// AnimDemo::AnimDemo()
//---------------------------------------------------------------------------
AnimDemo::AnimDemo()
{
    Graphics = SurfaceHandler::Load((char*)"./Graphics/Graphics.png", false);

    Pacman = new Player();
    Pacman->SetPosition(-130, 175);
    Pacman->TileX = Pacman->TileY = 0;
    Pacman->SetMoving(RIGHT);
    Pacman->CurrentCol = 4;


    for(int i = 0; i < 4; i++){
        Ghosts.push_back(new Ghost(i, 0, 0));
        Ghosts.at(i)->SetPosition(-80 + i*30, 175);
        Ghosts.at(i)->PacmanDead = true;
    }

}

//------------------------------------------------------------------------------
// AnimDemo::Update()
//
// Important notes:
// -This class is not using the Move functions of the Entities, since they are
//  too coupled with the game engine to work here. Instead, the function moves
//  the entities by itself.
//-------------------------------------------------------------------------------
void AnimDemo::Update()
{
    Pacman->Update();
    if(Pacman->X < 440)
        Pacman->X += 1.8 * FPS::FPSControl.GetSpeedFactor();
    else
        Pacman->X = -30;

     for(int i = 0; i < 4; i++){
        Ghosts.at(i)->Update();
        if(Ghosts.at(i)->X < 440)
            Ghosts.at(i)->X += 1.8 * FPS::FPSControl.GetSpeedFactor();
        else
            Ghosts.at(i)->X = -30;
     }


}

//---------------------------------------------------------------------------
// AnimDemo::Render(SDL_Surface* Display)
//---------------------------------------------------------------------------
void AnimDemo::Render(SDL_Surface* Display)
{
    Pacman->Render(Display, Graphics);
    for(int i = 0; i < 4; i++)
        Ghosts.at(i)->Render(Display, Graphics);
}

//---------------------------------------------------------------------------
// AnimDemo::~AnimDemo()
//---------------------------------------------------------------------------
AnimDemo::~AnimDemo()
{
    SDL_FreeSurface(Graphics);
    delete Pacman;
    for(std::size_t i = 0; i < Ghosts.size(); i++)
        delete Ghosts.at(i);
    Ghosts.clear();
}
