#ifndef FPS_H_INCLUDED
#define FPS_H_INCLUDED

// --------------------------------------------------------------------------------------------
// This class is responsible for defining the Frames per Second and passing it for the main class
// It can be easily reused for other games
// --------------------------------------------------------------------------------------------
#include <SDL/SDL.h>

class FPS{
    public:
        static FPS FPSControl; // static object of the class, so it can be acessible by all entities

    private:
        unsigned int OldTime;
        Uint32 LastTime;

        int Frames;
        int FramesPerSecond;

        float SpeedFactor;

        int PreviousLastTime;

    public:
        FPS();

        void Update();

        float GetSpeedFactor();

        int GetFPS();

        unsigned int GetTimePerFrame();

};

#endif // FPS_H_INCLUDED
