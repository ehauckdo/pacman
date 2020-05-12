#include "Player.h"

//--------------------------------------------
// Player::Player()
//--------------------------------------------
Player::Player()
{
    X = Y = 0;
    TileX = 25;
    TileY = 44;
    SpriteX = 0;
    SpriteY = 96;
    CurrentCol = 0;
    MovingDirection = NONE;
    Dead = false;
    AnimControl = new Animation();
}

//--------------------------------------------
// Player::Update()
//--------------------------------------------
void Player::Update()
{
    if(MovingDirection != NONE || Dead) AnimControl->Update();
}

//------------------------------------------------------------
// Player::Render(SDL_Surface* Display, SDL_Surface* Graphics)
//------------------------------------------------------------
void Player::Render(SDL_Surface* Display, SDL_Surface* Graphics)
{
    SurfaceHandler::Draw(Display, Graphics, 4 + TileX*8 + X, 30 + TileY*8 + Y, SpriteX +(AnimControl->GetCurrentFrame()%8)*24 +(CurrentCol*24), (AnimControl->GetCurrentFrame() > 7 ? (SpriteY)+24: SpriteY), 24, 24);
}

//------------------------------------------------------------
// Player::SetMoving(int Direction)
//------------------------------------------------------------
void Player::SetMoving(int Direction)
{
    MovingDirection = Direction;
}

//----------------------------------------------------------------------------------------
// Player::Move(int MoveParam)
//
// Important notes:
// -When turning left or right there's a special case, when the pacman reaches the
// end of one side and need to come out from the opposite side
//--------------------------------------------------------------------------------------
void Player::Move(int MoveParam)
{
    if(Dead) return;
    float Increment = 1.8 * FPS::FPSControl.GetSpeedFactor();
    switch(MovingDirection){

        case DOWN:
            CurrentCol = 6;
            if(MoveParam == CANT_MOVE){
                Y = 0;
                MovingDirection = NONE;
            }
            else{
                X = 0;
                if(MoveParam == GO_STRAIGHT){
                        if(Y + Increment < 8)
                            Y += Increment;
                        else{
                            TileY++;
                            Y = Y + Increment - 8;
                        }
                }
                else if(MoveParam == ADJUST_LEFT)
                        TileX--;
                else if(MoveParam == ADJUST_RIGHT)
                        TileX++;
            }
            break;

        case UP:
            CurrentCol = 2;
            if(MoveParam == CANT_MOVE){
                Y = 0;
                MovingDirection = NONE;
            }
            else{
                X = 0;
                if(MoveParam == GO_STRAIGHT){
                    if(Y - Increment >= 0)
                        Y -= Increment;
                    else{
                        TileY--;
                        Y = 8 + (Y - Increment);
                    }
                }
                else if(MoveParam == ADJUST_LEFT)
                    TileX--;
                else if(MoveParam == ADJUST_RIGHT)
                    TileX++;
            }
            break;

        case LEFT:
            CurrentCol = 0;
            if(MoveParam == CANT_MOVE){
                X = 0;
                MovingDirection = NONE;
            }
            else{
                Y = 0;
                if(MoveParam == GO_STRAIGHT){
                    if(X - Increment >= 0)
                        X -= Increment;
                    else{
                        TileX--;
                        X = 8 + (X - Increment);
                        if(TileX < -1) TileX = 54;
                    }
                }
                else if(MoveParam == ADJUST_UP)
                    TileY--;
                else if(MoveParam == ADJUST_DOWN)
                    TileY++;
            }
            break;

        case RIGHT:
            CurrentCol = 4;
            if(MoveParam == CANT_MOVE){
                X = 0;
                MovingDirection = NONE;
            }
            else{
                Y = 0;
                if(MoveParam == GO_STRAIGHT){
                    if(X + Increment < 8)
                        X += Increment;
                    else{
                        TileX++;
                        X = X + Increment - 8;
                        if(TileX > 54) TileX = -1;
                    }
                }
                else if(MoveParam == ADJUST_UP)
                    TileY--;
                else if(MoveParam == ADJUST_DOWN)
                    TileY++;
            }
            break;

        default: return;
    }
}

//---------------------------------------------
// Player::SetDead(bool IsDead)
//---------------------------------------------
void Player::SetDead(bool IsDead)
{
    if(IsDead){
        Dead = IsDead;
        AnimControl->TotalFrames = 14;
        AnimControl->Repeat = false;
        AnimControl->ResetCurrentFrame();
        SpriteY = 120;
        CurrentCol = 0;
        MovingDirection = NONE;
    }
    else{
        Dead = IsDead;
        SpriteY = 96;
        AnimControl->TotalFrames = 2;
        AnimControl->Repeat = true;
    }
}

//---------------------------------------------
// Player::SetPosition(int X, int Y)
//---------------------------------------------
void Player::SetPosition(int X, int Y)
{
    this->X = X;
    this->Y = Y;
}

//---------------------------------------------
// Player::~Player()
//---------------------------------------------
Player::~Player()
{
    delete AnimControl;
}
