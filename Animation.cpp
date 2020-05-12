#include "Animation.h"

// ------------------------------------------
// Animation::Animation()
// ------------------------------------------
Animation::Animation()
{
    TotalFrames = 2;
    CurrentFrame = 0;
    FrameRate = 100;
    TimeCounter = SDL_GetTicks();
    Repeat = true;
}

// ------------------------------------------------------------------------------------
// Animation::Update()
//
// Important notes:
// -Repeat feature considers your animation is made of two sprites only
//  If an animation with more sprites is needed, this function need to be changed.
// -------------------------------------------------------------------------------------
void Animation::Update()
{
    if(TimeCounter + FrameRate < SDL_GetTicks()){
        TimeCounter = SDL_GetTicks();

            CurrentFrame += 1;

        if(CurrentFrame >= TotalFrames){
            if(Repeat)
                CurrentFrame = 0;
            else
                CurrentFrame -= 1;
        }

    }

}

// ------------------------------------------
// Animation::SetFrameRate(int FrameRate)
// ------------------------------------------
void Animation::SetFrameRate(int FrameRate)
{
    this->FrameRate = FrameRate;
}

// ------------------------------------------
// Animation::ResetCurrentFrame()
// ------------------------------------------
void Animation::ResetCurrentFrame()
{
    CurrentFrame = 0;
}

// ------------------------------------------
// Animation::GetCurrentFrame()
// ------------------------------------------
int Animation::GetCurrentFrame()
{
    return CurrentFrame;
}
