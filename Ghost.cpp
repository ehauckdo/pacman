#include "Ghost.h"

// ----------------------------------------------------------
// Ghost::Ghost(int Color, int TileX,int TileY)
// ----------------------------------------------------------
Ghost::Ghost(int Color, int TileX,int TileY)
{
    MovingDirection = NONE;
    CurrentCol = State = 0;
    SpriteX = 0;
    X = Y = 0;
    this->Color = Color;
    SpriteY = Color * 24;
    Weak = PacmanDead = false;
    this->TileX = TileX;
    this->TileY = TileY;
    AnimControl = new Animation();
    MovingDirection = rand() % 4 + 1;
    WeakTimeCounter = AiTimeCounter = SDL_GetTicks();
    FreeTimeCounter = SDL_GetTicks() + 5000;
    CanDoAI = true;
}

// ----------------------------------
// Ghost::Update()
// ----------------------------------
void Ghost::Update()
{
    CheckWeakness();

    if(CanDoAI == true && PacmanDead == false)
        DoAI();
    else if(AiTimeCounter + 200 < SDL_GetTicks()){
        AiTimeCounter = SDL_GetTicks();
        CanDoAI = true;
    }

    AnimControl->Update();

}

// ----------------------------------------------------------
// Ghost::Render(SDL_Surface* Display, SDL_Surface* Graphics)
// ----------------------------------------------------------
void Ghost::Render(SDL_Surface* Display, SDL_Surface* Graphics)
{
    SurfaceHandler::Draw(Display, Graphics, 4 + TileX * 8 + X, 30 + TileY * 8 + Y, SpriteX+(CurrentCol*24)+(AnimControl->GetCurrentFrame()*24), SpriteY, 24, 24);
}

// -----------------------------------------------------------------------------
// Ghost::DoAI()
//
// Important notes:
// -There are four states for a ghost, which tells them which kind of AI they should do
//  The description of each one are above the conditionals
// -----------------------------------------------------------------------------
void Ghost::DoAI()
{
    int OldDirection = MovingDirection;

    // locked in the cage
    if(State == 0){
        if(FreeTimeCounter < SDL_GetTicks())
            State = 1;
    }

    // going out of the cage
    else if(State == 1){

        if(TileY == 21){
            if(Weak == false) State = 2;
            else State = 3;
            return;
        }

        if(TileX < 25)
            MovingDirection = RIGHT;
        else if(TileX > 25)
            MovingDirection = LEFT;
        else
            MovingDirection = UP;
        return;
    }

    // normal state, running to pacman
    else if(State == 2){
        if(TileX > PlayerCoord[0]){
            //Pacman is by the left
            if(TileY > PlayerCoord[1]){
                //Pacman is by the up-left
                int DifferenceX = TileX - PlayerCoord[0];

                if(DifferenceX > 25 && TileY == 26){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end())
                        MovingDirection = RIGHT;
                   /* else if(TileY < 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end())
                            MovingDirection = DOWN;
                    }
                    else if(TileY > 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end())
                            MovingDirection = UP;
                    }
                    else MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );*/

                }

                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end()){
                        //dá pra mover pra cima e pra esquerda
                        int Directions[2] = {UP, LEFT};
                        MovingDirection = Directions[rand()%2];
                    }
                    else
                        //dá pra mover pra cima
                        MovingDirection = UP;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end())
                    //dá pra mover pra esquerda
                    MovingDirection = LEFT;
                else
                    //não dá para mover nem para esquerda nem para cima: random
                    MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );
            }
            else{
                //Pacman is by the down-left
                int DifferenceX = TileX - PlayerCoord[0];

                if(DifferenceX > 35 && TileY == 26){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end())
                        MovingDirection = RIGHT;
                    /*else if(TileY < 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end())
                            MovingDirection = DOWN;
                    }
                    else if(TileY > 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end())
                            MovingDirection = UP;
                    }
                    else MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );*/

                }

                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end()){
                        //dá pra mover para baixo e para esquerda
                        int Directions[2] = {DOWN, LEFT};
                        MovingDirection = Directions[rand()%2];
                    }
                    else
                        //dá pra mover para baixo
                        MovingDirection = DOWN;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end())
                    //dá para mover para esquerda
                    MovingDirection = LEFT;
                else
                    //não dá para mover nem para esquerda nem para cima: random
                     MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );
            }
        }
        else{
            //Pacman is by the right
            if(TileY > PlayerCoord[1]){
                //Pacman is by the up-right
                int DifferenceX = PlayerCoord[0] - TileX;

                if(DifferenceX > 35 && TileY == 26){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end())
                        MovingDirection = LEFT;
                    /*else if(TileY < 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end())
                            MovingDirection = DOWN;
                    }
                    else if(TileY > 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end())
                            MovingDirection = UP;
                    }
                    else MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );*/
                }

                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end()){
                        //dá pra mover para cima e para direita
                        int Directions[2] = {UP, RIGHT};
                        MovingDirection = Directions[rand() % 2];
                    }
                    else
                        //dá pra mover para cima
                        MovingDirection = UP;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end())
                    //dá pra mover pra direita
                    MovingDirection = RIGHT;
                else
                    //não dá para mover nem para direita nem para cima: random
                    MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );
            }
            else{
                //Pacman is by the down-right
                int DifferenceX = PlayerCoord[0] - TileX;

                if(DifferenceX > 35 && TileY == 26){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end())
                        MovingDirection = LEFT;
                   /* else if(TileY < 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end())
                            MovingDirection = DOWN;
                    }
                    else if(TileY > 26){
                        if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end())
                            MovingDirection = UP;
                    }
                    else MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );*/
                }

                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end()){
                        //dá para mover para baixo e para direita
                        int Directions[2] = {DOWN, RIGHT};
                        MovingDirection = Directions[rand() % 2];
                    }
                    else
                        //dá para mover para baixo
                        MovingDirection = DOWN;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end())
                    //dá para mover para direita
                    MovingDirection = RIGHT;
                else
                    //não dá para mover nem para baixo nem para direita: random
                    MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());

            }
        }

    }

    // running from pacman
    else if(State == 3){

        //if(DifferenceX > 10 && DifferenceY > 10)
        //    MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );

        if(TileX > PlayerCoord[0]){
            //Pacman is by the left
            if(TileY > PlayerCoord[1]){
                //Pacman is by the up-left
                int DifferenceX = TileX - PlayerCoord[0];

                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end()){
                        //can move right-down
                        int Directions[2] = {RIGHT, DOWN};
                        MovingDirection = Directions[rand()%2];
                    }
                    else
                        //can move right
                        if(TileY >= 25 && TileY <= 27){
                            if(DifferenceX <= 15)
                                MovingDirection = RIGHT;
                            else
                                MovingDirection = LEFT;
                        }

                        else MovingDirection = RIGHT;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end())
                    //can move to down
                    MovingDirection = DOWN;
                else
                    //can't move down nor right: rand
                    MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );

            }
            else{
                //Pacman is by the down-left
                int DifferenceX = TileX - PlayerCoord[0];

                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end()){
                        //can move up-right
                        int Directions[2] = {RIGHT, UP};
                        MovingDirection = Directions[rand()%2];
                    }
                    else
                        //can move right
                        if(TileY >= 25 && TileY <= 27){
                            if(DifferenceX <= 15)
                                MovingDirection = RIGHT;
                            else
                                MovingDirection = LEFT;
                        }
                        else MovingDirection = RIGHT;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end())
                    //can move up
                    MovingDirection = UP;
                else
                    //can't move up nor right: rand
                     MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );
            }
        }
        else{
            //Pacman is by the right
            if(TileY > PlayerCoord[1]){
                //Pacman is by the up-right
                int DifferenceX = PlayerCoord[0] - TileX;

                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end()){
                        //can move down-left
                        int Directions[2] = {DOWN, LEFT};
                        MovingDirection = Directions[rand() % 2];
                    }
                    else
                        //can move left
                        if(TileY >= 25 && TileY <= 27){
                            if(DifferenceX <= 15)
                                MovingDirection = LEFT;
                            else
                                MovingDirection = RIGHT;
                        }
                        else MovingDirection = LEFT;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end())
                    //can move down
                    MovingDirection = DOWN;
                else
                    //não dá para mover nem para direita nem para cima: random
                    MovingDirection = WalkableDirections.at( rand() % WalkableDirections.size() );
            }
            else{
                //Pacman is by the down-right
                int DifferenceX = PlayerCoord[0] - TileX;

                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end()){
                    if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end()){
                        //can move up-left
                        int Directions[2] = {UP, LEFT};
                        MovingDirection = Directions[rand() % 2];
                    }
                    else
                        //can move LEFT

                        if(TileY >= 25 && TileY <= 27){
                            if(DifferenceX <= 15)
                                MovingDirection = LEFT;
                            else
                                MovingDirection = RIGHT;
                        }
                        else MovingDirection = LEFT;
                }
                else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end())
                    //dá para mover para cima
                    MovingDirection = UP;
                else
                    //não dá para mover nem para cima nem para direita: random
                    MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
            }
        }

    }

    // invisible state, returning to cage
    else if(State == 4){
        //checks if it's in the right place
        if(TileX >= 25 && TileX <= 27 && TileY >= 20 && TileY <= 26){
            //sets it to goes down
            if(TileY >= 21 && TileY <= 24){
                MovingDirection = DOWN;
                return;
            }
            //if it's insire the cell already, return to state 0
            if(TileY == 25){
                SetReturnCell(false);
                return;
            }
        }
        //gate is to the down-right
        else if(TileX <= 26 && TileY <= 21){
            //move down-right
            if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end()){
                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end()){
                    int Directions[2] = {DOWN, RIGHT};
                    MovingDirection = Directions[rand()%2];
                }
                else
                    MovingDirection = DOWN;
            }
            else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end())
                MovingDirection = RIGHT;
            else
                MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
        }

        //gate is to the down-left
        else if(TileX >= 26 && TileY <= 21){
            //move down-left
            if(std::find(WalkableDirections.begin(), WalkableDirections.end(), DOWN) != WalkableDirections.end()){
                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end()){
                    int Directions[2] = {DOWN, LEFT};
                    MovingDirection = Directions[rand() % 2];
                }
                else
                    MovingDirection = DOWN;
            }
            else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end())
                MovingDirection = LEFT;
            else
               MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
        }

        //gate is to the up-right
        else if(TileX <= 26 && TileY >= 21){
            //move up-right
            if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end()){
                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end()){
                    int Directions[2] = {UP, RIGHT};
                    MovingDirection = Directions[rand() % 2];
                }
                else
                    MovingDirection = UP;
            }
            else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), RIGHT) != WalkableDirections.end())
                MovingDirection = RIGHT;
            else
                MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
        }

        //gate is to the up-left
        else if(TileX >= 26 && TileY >= 21){
            //move up-left
            if(std::find(WalkableDirections.begin(), WalkableDirections.end(), UP) != WalkableDirections.end()){
                if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end()){
                    int Directions[2] = {UP, LEFT};
                    MovingDirection = Directions[rand() % 2];
                }
                else
                    MovingDirection = UP;
            }
            else if(std::find(WalkableDirections.begin(), WalkableDirections.end(), LEFT) != WalkableDirections.end())
                MovingDirection = LEFT;
            else
                MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
        }

    }

    if(OldDirection != MovingDirection){
        CanDoAI = false;
        AiTimeCounter = SDL_GetTicks();
    }
}

// ----------------------------------------------
// Ghost::StopAI()
// ----------------------------------------------
void Ghost::StopAI()
{
    MovingDirection = NONE;
    PacmanDead = true;
}

// ----------------------------------------------
// Ghost::CheckWeakness()
// ----------------------------------------------
void Ghost::CheckWeakness()
{
    if(Weak == true && State != 4){
        if(WeakTimeCounter + 5000 < SDL_GetTicks())
            SetWeakness(false);
        else if(WeakTimeCounter + 3000 < SDL_GetTicks())
            SpriteY = 192;
    }
}

//----------------------------------------------
// Ghost::SetWeakness(bool Weak)
//----------------------------------------------
void Ghost::SetWeakness(bool Weak)
{
    if(State != 4){
        this->Weak = Weak;
        if(Weak == true){
            WeakTimeCounter = SDL_GetTicks();
            SpriteY = 168;
            if(State != 0){ State = 3; MovingDirection = NONE; CanDoAI = true;}
        }
        else{
            SpriteY = Color * 24;
            if(State != 0) { State = 2; MovingDirection = NONE; CanDoAI = true;}
        }
    }
}

//----------------------------------------------
// Ghost::SetReturnCell(bool Return)
//----------------------------------------------
void Ghost::SetReturnCell(bool Return)
{
    if(Return == true){
        State = 4;
        SpriteY = 216;
        MovingDirection = NONE;
        CanDoAI = true;
    }
    else{
        State = 0;
        FreeTimeCounter = SDL_GetTicks() + 3000;
        SpriteY = Color * 24;
    }
}

//----------------------------------------------------------------------------------------
// Ghost::Move(int MoveParam)
//
// Important notes:
// -When turning left or right there's a special case, when the pacman reaches the
// end of one side and need to come out from the opposite side
//--------------------------------------------------------------------------------------
void Ghost::Move(int MoveParam)
{
    float Increment = (State == 4 ? 2.0 : Weak ? 1.2 : 1.8) * FPS::FPSControl.GetSpeedFactor();
    switch(MovingDirection){

        case DOWN:
            CurrentCol = 2;
            if(MoveParam == CANT_MOVE){

                if(State == 0)
                    MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
                else if(State == 4){
                    MovingDirection = DOWN;
                    if(Y + Increment < 8)
                        Y += Increment;
                    else{
                        TileY++;
                        Y = Y + Increment - 8;
                    }
                }
                else{
                    Y = 0;
                    MovingDirection = NONE;
                }
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
                else{
                    if(MoveParam == ADJUST_LEFT)
                            TileX--;
                    else if(MoveParam == ADJUST_RIGHT)
                            TileX++;
                    Y += Increment;
                }
            }
            break;

        case UP:
            CurrentCol = 6;
            if(MoveParam == CANT_MOVE){
                Y = 0;
                MovingDirection = NONE;
                if(State == 0)
                    MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
                if(State == 1){
                    if(Y - Increment >= 0)
                        Y -= Increment;
                    else{
                        TileY--;
                        Y = 8 + (Y - Increment);
                    }
                }

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
                else{
                    if(MoveParam == ADJUST_LEFT)
                        TileX--;
                    else if(MoveParam == ADJUST_RIGHT)
                        TileX++;
                    TileY--;
                    Y = 8 + (Y - Increment);
                }
            }
            break;

        case LEFT:
            CurrentCol = 4;
            if(MoveParam == CANT_MOVE){
                X = 0;
                MovingDirection = NONE;
                if(State == 0) MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
            }
            else{
                Y = 0;
                if(MoveParam == GO_STRAIGHT){
                    if(X - Increment >= 0){
                        X -= Increment;
                    }
                    else{
                        TileX--;
                        X = 8 + (X - Increment);
                    }
                }
                else{
                    if(MoveParam == ADJUST_UP)
                        TileY--;
                    else if(MoveParam == ADJUST_DOWN)
                        TileY++;
                    TileX--;
                    X = 8 + (X - Increment);
                }
                if(TileX < -1) TileX = 53;

            }
            break;

        case RIGHT:
            CurrentCol = 0;
            if(MoveParam == CANT_MOVE){
                X = 0;
                MovingDirection = NONE;
                if(State == 0) MovingDirection = WalkableDirections.at(rand() % WalkableDirections.size());
            }
            else{
                Y = 0;
                if(MoveParam == GO_STRAIGHT){
                    if(X + Increment < 8)
                        X += Increment;
                    else{
                        TileX++;
                        X = X + Increment - 8;
                    }
                }
                else{
                    if(MoveParam == ADJUST_UP)
                        TileY--;
                    else if(MoveParam == ADJUST_DOWN)
                        TileY++;
                    X += Increment;
                }
                if(TileX > 53) TileX = -1;
            }
            break;

        case NONE:
            return;
    }
}

//----------------------------------------------------------
// Ghost::UpdatePlayerCoordinates(int TileX, int TileY)
//----------------------------------------------------------
void Ghost::UpdatePlayerCoordinates(int TileX, int TileY)
{
    PlayerCoord[0] = TileX;
    PlayerCoord[1] = TileY;
}

//----------------------------------------------------------
// Ghost::GetOppositeDirection()
//----------------------------------------------------------
int Ghost::GetOppositeDirection()
{
    switch(MovingDirection){
        case UP: return DOWN;
        case DOWN: return UP;
        case LEFT: return RIGHT;
        case RIGHT: return LEFT;
        default: return NONE;
    }
}

//----------------------------------------------------------
// Ghost::SetPosition(int X, int Y)
//----------------------------------------------------------
void Ghost::SetPosition(int X, int Y)
{
    this->X = X;
    this->Y = Y;
}

//----------------------------------------------------------
// Ghost::~Ghost()
//----------------------------------------------------------
Ghost::~Ghost()
{
    delete AnimControl;
    WalkableDirections.clear();
}
