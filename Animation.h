#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

// --------------------------------------------------------------------------------------------
// This class is used for animating sprites of Entities
// It can be reusable on any similar game to switch between the sprites
// --------------------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include "FPS.h"

class Animation{
    public:
        int TotalFrames;
        bool Repeat; // should de animation repeat/oscilate or not

    private:
        int FrameRate;
        int CurrentFrame;
        int CurrentColumn;
        Uint32 TimeCounter;

    public:
        Animation();

        void Update();

        void SetFrameRate(int FrameRate);

        void ResetCurrentFrame();

        int GetCurrentFrame();
};

#endif // ANIMATION_H_INCLUDED
