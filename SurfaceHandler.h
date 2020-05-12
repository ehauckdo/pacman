#ifndef SURFACEHANDLER_H_INCLUDED
#define SURFACEHANDLER_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Class used for handling SDL_Surface objects, it'll do the main work of Loading images into
// Surfaces, Drawing a Surface into another, etc
// Can easily be reused for other games
// --------------------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class SurfaceHandler{
    public:
        static SDL_Surface* Load(char* File, bool Transparency);

        static SDL_Surface* LoadWithTransparency(char* File);

        static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);

        static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);

        static bool Transparent(SDL_Surface* Surf_Dest, int R, int G, int B);

};

#endif // SURFACEHANDLER_H_INCLUDED
