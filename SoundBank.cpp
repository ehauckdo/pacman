#include "SoundBank.h"

//---------------------------------------------------
// SoundBank::Load(char* File)
//
// Load a .wav file from disk and puts it on the vector
// Returns its position
//---------------------------------------------------
int SoundBank::Load(char* File) {
    Mix_Chunk* TempSound = NULL;

    if((TempSound = Mix_LoadWAV(File)) == NULL) {
        return -1;
    }

    SoundList.push_back(TempSound);

    return (SoundList.size() - 1);
}

//--------------------------------------------------------------------
// SoundBank::Play(std::size_t ID)
//
// Important notes:
// -It will play the sound ONCE. If another request for playing arrives before
// the current one finishing, will be ignored
//--------------------------------------------------------------------
void SoundBank::Play(std::size_t ID) {
    if(ID < 0 || ID >= SoundList.size()) return;
    if(SoundList[ID] == NULL) return;

    if(SoundList[ID] && Mix_Playing(ID) == 0){
        Mix_PlayChannel(ID, SoundList[ID], 0);
    }

}

//--------------------------------------------------------------------
// SoundBank::PlayLoop(std::size_t ID)
//--------------------------------------------------------------------
void SoundBank::PlayLoop(std::size_t ID)
{
    if(ID < 0 || ID >= SoundList.size()) return;
    if(SoundList[ID] == NULL) return;

    if(SoundList[ID] && Mix_Playing(ID) == 0){
        Mix_PlayChannel(ID, SoundList[ID], -1);
    }
}

//--------------------------------------------------------------------
// SoundBank::Stop(std::size_t ID)
//--------------------------------------------------------------------
void SoundBank::Stop(std::size_t ID)
{
    Mix_HaltChannel(ID);
}

//--------------------------------------------------------------------
// SoundBank::StopAll()
//--------------------------------------------------------------------
void SoundBank::StopAll()
{
    for(int ID = 0; ID < Soundlist.size(); ID++)
        Mix_HaltChannel(ID);
}

//--------------------------------------------------------------------
// SoundBank::Cleanup()
//--------------------------------------------------------------------
void SoundBank::Cleanup() {
    StopAll();

    for(std::size_t i = 0;i < SoundList.size();i++) {
        Mix_FreeChunk(SoundList[i]);
    }

    SoundList.clear();
}
