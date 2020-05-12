#include "SurfaceHandler.h"

//------------------------------------------------------------------------------------
// SurfaceHandler::Load(char* File, bool Transparency)
//
// Important notes:
// -The boolean parameter Transparency sets wether transparency should be applied to the surface or not
// ATENTION: The transparency applied here is for the black color, useful on the Pacman game
// If you are to use Alpha Transparency, go for LoadWithTransparency function
//------------------------------------------------------------------------------------
SDL_Surface* SurfaceHandler::Load(char* File, bool Transparency)
{
    SDL_Surface* Surf_Temp = NULL;

    if((Surf_Temp = IMG_Load(File)) == NULL)
        return NULL;

    if(Transparency){
        SurfaceHandler::Transparent(Surf_Temp, 0, 0, 0);
    }

    return Surf_Temp;
}

//--------------------------------------------
// SurfaceHandler::LoadWithTransparency(char* File)
//--------------------------------------------
SDL_Surface* LoadWithTransparency(char* File)
{
    SDL_Surface* Surf_Temp = NULL;
    SDL_Surface* Surf_Return = NULL;

    if((Surf_Temp = IMG_Load(File)) == NULL)
        return NULL;

    Surf_Return = SDL_DisplayFormatAlpha(Surf_Temp);
    SDL_FreeSurface(Surf_Temp);
    return Surf_Return;
}

//-----------------------------------------------------------------------------------
// SurfaceHandler::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y)
//
// Important notes:
// -Draw directly on the (0,0) position of the Surface
//-----------------------------------------------------------------------------------
bool SurfaceHandler::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y)
{
    if(Surf_Dest == NULL || Surf_Src == NULL)
        return false;

    SDL_Rect DestR;
    DestR.x = X;
    DestR.y = Y;

    SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);
    return true;
}

//---------------------------------------------------------------------------------------------------------------
// SurfaceHandler::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H)
//
// Important notes:
// -X and Y refers to where in the Surface your image will be drawn
// and X2, Y2, refers which part of your source surface you want to get
//---------------------------------------------------------------------------------------------------------------
bool SurfaceHandler::Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H)
{
    if(Surf_Dest == NULL || Surf_Src == NULL)
        return false;

    SDL_Rect DestR;
    DestR.x = X;
    DestR.y = Y;

    SDL_Rect SrcR;
    SrcR.x = X2;
    SrcR.y = Y2;
    SrcR.h = H;
    SrcR.w = W;

    SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);
    return true;
}

//-----------------------------------------------------------------------------------
// SurfaceHandler::Transparent(SDL_Surface* Surf_Dest, int R, int G, int B)
//
// Sets transparency of a RGB color on the Surface passed
//-----------------------------------------------------------------------------------
bool SurfaceHandler::Transparent(SDL_Surface* Surf_Dest, int R, int G, int B){
    if(Surf_Dest == NULL)
        return NULL;
    SDL_SetColorKey(Surf_Dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Dest->format, R, G, B));
    return Surf_Dest;
}
