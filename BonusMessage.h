#ifndef BONUSMESSAGE_H_INCLUDED
#define BONUSMESSAGE_H_INCLUDED
#include <SDL/SDL.h>

class BonusMessage{

    public:
        bool Show;
        int X, Y;
        Uint32 StartTime;

    public:
        BonusMessage(int X, int Y){
            this->X = X;
            this->Y = Y;
            StartTime = SDL_GetTicks()+1000;
        }

        void Update(){
            if(StartTime + 1000 < SDL_GetTicks())
                Show = false;
        }


};

#endif // BONUSMESSAGE_H_INCLUDED
