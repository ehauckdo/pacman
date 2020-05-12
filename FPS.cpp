#include "FPS.h"

// --------------------------------------------
// FPS::FPSControl
// Static object needed by everyone on the game
// --------------------------------------------
FPS FPS::FPSControl;

//--------------------------------------------
// FPS::FPS()
//--------------------------------------------
FPS::FPS()
{
    OldTime = 0;
    LastTime = 0;
    Frames = 0;
    FramesPerSecond = 60;
    SpeedFactor = 0;
}

//------------------------------------------------------------------------
// StartMenu::Update()
//
// Important notes:
// -SpeedFactor is calculated based on the time passed (on seconds) and how much
// we want to move per frame. So if we want to move 30 pixels per second, and
// the update took 1000ms to run, we have 1000/1000 = 1 * 30 = 30, which means
// we need to move 30 pixels for each frame, since each frame take 1 second
//------------------------------------------------------------------------
void FPS::Update()
{
    if(LastTime + 100 < SDL_GetTicks()){
        LastTime = OldTime = SDL_GetTicks();
        Frames = 0;
        return;
    }

    if(OldTime + 1000 < SDL_GetTicks()){
        OldTime = SDL_GetTicks();
        FramesPerSecond = Frames > 32 ? 32 : Frames;
        Frames = 0;
    }

    SpeedFactor = (SDL_GetTicks() - LastTime) / 1000.0f * 60.0f;

    LastTime = SDL_GetTicks();

    Frames++;
}

//--------------------------------------------
// FPS::GetSpeedFactor()
//--------------------------------------------
float FPS::GetSpeedFactor()
{
    return SpeedFactor;
}

//--------------------------------------------
// FPS::GetFPS()
//--------------------------------------------
int FPS::GetFPS()
{
    return FramesPerSecond;
}

//--------------------------------------------
// FPS::GetTimePerFrame()
//--------------------------------------------
unsigned int FPS::GetTimePerFrame()
{
    return 1000/FramesPerSecond;
}
