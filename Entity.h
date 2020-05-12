#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

// --------------------------------------------------------------------------------------------
// Main entity class of the game
// All other entities will inherit from this one, as usual all functions must be virtual to make
// it functional. It may be used as well for other games, though the atributes may need to be
// slightly changed according to the game structure
// --------------------------------------------------------------------------------------------

#include <SDL/SDL.h>
#include "SurfaceHandler.h"
#include "Animation.h"
#include "FPS.h"

class Entity{
    protected:
        Animation* AnimControl;

        int SpriteX, SpriteY; // holds the X and Y position of the line where the sprite begins
                              // most SpriteX will be 0, considering most entities have a line for themselves

    public:
        int TileX, TileY; // which squares X and Y the entitiy are

        float X, Y; // offset that should be added to TileX, TileY

        int CurrentCol; // based on the direction the entity is moving
                        // current column tells which sprite to get on the image loaded


    public:

        Entity(){};

        virtual void Update() = 0;

        virtual void Render(SDL_Surface* Display, SDL_Surface* Graphics) = 0;

        virtual ~Entity(){};

};

#endif // ENTITY_H_INCLUDED
