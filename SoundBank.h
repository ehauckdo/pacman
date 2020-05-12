#ifndef SOUNDBANK_H_INCLUDED
#define SOUNDBANK_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Simply class for handling Sounds, it's based on SDLTutorial's example:
// http://www.sdltutorials.com/sdl-soundbank
// Stores a vector of Mix_Chunk objects (basically stores a .wav file)
// and have some functions for playing them
// --------------------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <vector>
#include "Define.h"


class SoundBank{
    public:
        std::vector<Mix_Chunk*> SoundList; // array of sounds

    public:

        int Load(char* File);

        void Cleanup();

        void Play(std::size_t ID);

        void PlayLoop(std::size_t ID);

        void Stop(std::size_t ID);

        void StopAll();

};

#endif // CSOUNDBANK_H_INCLUDED
