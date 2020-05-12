#include "Map.h"

Map::Map()
{
    /** FONT RELATED STUFF **/
    TTF_Init();
    Font = TTF_OpenFont((char*)"./Fonts/joystix.ttf", 16);

    Color = new SDL_Color();
    Color->b = Color->g = Color->r = 255;

    Score = 0;
    /** ----------------- **/

    Lives = 3;

    Background = SurfaceHandler::Load((char*)"./Graphics/Background.png", false);
    Graphics = SurfaceHandler::Load((char*)"./Graphics/Graphics.png", true);

    Initialize();
}

void Map::Initialize()
{
    Reset = false;

    FILE* FileHandle = fopen("./Map/Map.txt", "r");
    if(FileHandle == NULL) return;

    for(int Line = 0; Line < 60; Line++){
        for(int Column = 0; Column < 54; Column++)
            fscanf(FileHandle, "%d ", &Tiles[Line][Column]);
        fscanf(FileHandle, "\n");
    }

    fclose(FileHandle);

    Pacman = new Player();

    if(Ghosts.empty() == false){
        for(std::size_t i = 0; i < Ghosts.size(); i++) delete Ghosts.at(i);
        Ghosts.clear();
    }

    Ghost* FirstGhost = new Ghost(0, 26, 21);
    FirstGhost->State = 2;
    Ghosts.push_back(FirstGhost);
    for(int i = 0; i < 3; i++){
        Ghosts.push_back(new Ghost(i+1, 22 + i*3, 27));
        Ghosts.at(i)->FreeTimeCounter = SDL_GetTicks() + i*5000;
    }
}

void Map::Update()
{
    if(Reset && ResetTime < SDL_GetTicks()){
        Lives -= 1;
        Initialize();
    }

    Pacman->Update();
    UpdatePlayerCoordinates();
    for(std::size_t i = 0; i < Ghosts.size(); i++){
        UpdateWalkableDirections(Ghosts.at(i), Ghosts.at(i)->TileX, Ghosts.at(i)->TileY);
        Ghosts.at(i)->Update();
    }
    ProcessMovements();
}

void Map::UpdateWalkableDirections(Ghost *CurrentGhost, int TileX, int TileY)
{
    CurrentGhost->WalkableDirections.clear();
    if(Tiles[TileY-1][TileX] != 1 && Tiles[TileY-1][TileX+2] != 1 && DOWN != CurrentGhost->MovingDirection)
        CurrentGhost->WalkableDirections.push_back(UP);
    if(Tiles[TileY+3][TileX] != 1 && Tiles[TileY+3][TileX+2] != 1 && UP != CurrentGhost->MovingDirection)
        CurrentGhost->WalkableDirections.push_back(DOWN);
    if(Tiles[TileY][TileX-1] != 1 && Tiles[TileY+2][TileX-1] != 1 && RIGHT != CurrentGhost->MovingDirection)
        CurrentGhost->WalkableDirections.push_back(LEFT);
    if(Tiles[TileY][TileX+3] != 1 && Tiles[TileY+2][TileX+3] != 1 && LEFT != CurrentGhost->MovingDirection)
        CurrentGhost->WalkableDirections.push_back(RIGHT);

    if(CurrentGhost->WalkableDirections.empty())
        CurrentGhost->WalkableDirections.push_back(CurrentGhost->GetOppositeDirection());

    if(CurrentGhost->State == 1){
        if(TileX >= 25 && TileX <= 28 && TileY <= 25 && TileY >= 26)
            CurrentGhost->WalkableDirections.push_back(UP);
    }
    if(CurrentGhost->State == 4){
        if(TileX >= 25 && TileX <= 27 && TileY >= 21 && TileY <= 24)
            CurrentGhost->WalkableDirections.push_back(DOWN);
    }
}

void Map::ProcessMovements()
{
    int MoveParam;
    MoveParam = CheckMoveValid(Pacman->MovingDirection, Pacman->TileX, Pacman->TileY);
    Pacman->Move(MoveParam);
    GetPacmanDots();
    CollisionCheck();

    for(std::size_t i = 0; i < Ghosts.size(); i++){
        MoveParam = CheckMoveValid(Ghosts.at(i)->MovingDirection, Ghosts.at(i)->TileX, Ghosts.at(i)->TileY);
        Ghosts.at(i)->Move(MoveParam);
    }
    CollisionCheck();

}

void Map::Render(SDL_Surface* Display){

    SurfaceHandler::Draw(Display, Background, 0, 0);

     for(int Line = 0; Line < 60; Line++){
        for(int Column = 0; Column < 54; Column++)
            if(Tiles[Line][Column] == 2)
                SurfaceHandler::Draw(Display, Graphics, 4 + Column * 8, 30 + Line * 8, 144, 144, 8, 8);
            else if(Tiles[Line][Column] == 3)
                SurfaceHandler::Draw(Display, Graphics, 4 + Column * 8, 30 + Line * 8, 152, 144, 8, 8);
    }

    Pacman->Render(Display, Graphics);
    for(std::size_t i = 0; i < Ghosts.size(); i++)
        Ghosts.at(i)->Render(Display, Graphics);

    /** TEXT STUFF **/

    char buffer[15];
    sprintf(buffer, "%d", Score);
    TextLoader = TTF_RenderText_Blended(Font, buffer, *Color);

    SurfaceHandler::Draw(Display, TextLoader, 80, 0);

    for(int i = 0; i < Lives; i++)
        SurfaceHandler::Draw(Display, Graphics, 50-i*25, 0, 0, 96, 24, 24);
    /** ------- **/
}

int Map::CheckMoveValid(int Direction, int Column, int Line)
{
    if(Direction == DOWN){
        if(Tiles[Line+3][Column] != 1 && Tiles[Line+3][Column+2] != 1)
            return GO_STRAIGHT;
        else if(Tiles[Line+3][Column+1] != 1 && Tiles[Line+3][Column+3] != 1)
            return ADJUST_RIGHT;
        else if(Tiles[Line+3][Column-1] != 1 && Tiles[Line+3][Column+1] != 1)
            return ADJUST_LEFT;
    }
    else if(Direction == UP){
        if(GetTile(Line-1, Column) != 1 && GetTile(Line-1, Column+2) != 1)
            return GO_STRAIGHT;
        else if(GetTile(Line-1, Column+1) != 1 && GetTile(Line-1, Column+3) != 1)
            return ADJUST_RIGHT;
        else if(GetTile(Line-1, Column-1) != 1 && GetTile(Line-1, Column+1) != 1)
            return ADJUST_LEFT;
    }
    else if(Direction == RIGHT){
        if(GetTile(Line, Column+3) != 1 && GetTile(Line+1, Column+3) != 1 && GetTile(Line+2, Column+3) != 1)
            return GO_STRAIGHT;
        else if(GetTile(Line-1, Column+3) != 1 && GetTile(Line+1, Column+3) != 1)
            return ADJUST_UP;
        else if(GetTile(Line+1, Column+3) != 1 && GetTile(Line+3, Column+3) != 1)
            return ADJUST_DOWN;
    }
    else if(Direction == LEFT){
        if(GetTile(Line, Column-1) != 1 && GetTile(Line+2, Column-1) != 1)
            return GO_STRAIGHT;
        else if(GetTile(Line+1, Column-1) != 1 && GetTile(Line+3, Column-1) != 1)
            return ADJUST_DOWN;
        else if(GetTile(Line-1, Column-1) != 1 && GetTile(Line+1, Column-1) != 1)
            return ADJUST_UP;
    }
    return CANT_MOVE;
}

void Map::GetPacmanDots()
{
    for(int Line = 0; Line < 3; Line++)
        for(int Column = 0; Column < 3; Column++)
            if(Tiles[Pacman->TileY+Line][Pacman->TileX+Column] == 2){
                Tiles[Pacman->TileY+Line][Pacman->TileX+Column] = 0;
                Score += 10;
            }
            else if(Tiles[Pacman->TileY+Line][Pacman->TileX+Column] == 3){
                Tiles[Pacman->TileY+Line][Pacman->TileX+Column] = 0;
                Score += 50;
                for(std::size_t i = 0; i < Ghosts.size(); i++)
                    Ghosts.at(i)->SetWeakness(true);
            }
}

bool Map::CollisionCheck()
{
    if(Pacman->Dead == false){
        int PlayerX1 = 4 + Pacman->TileX * 8 + Pacman->X;
        int PlayerX2 = 4 + Pacman->TileX * 8 + 24 + Pacman->X;
        int PlayerY1 = 30 + Pacman->TileY * 8 + Pacman->Y;
        int PlayerY2 = 30 + Pacman->TileY * 8 + 24 + Pacman->Y;

        for(std::size_t i = 0; i < Ghosts.size(); i++){
            int GhostX1 = 4 + Ghosts.at(i)->TileX * 8 + Ghosts.at(i)->X;
            int GhostX2 = 4 + Ghosts.at(i)->TileX * 8 + 24 + Ghosts.at(i)->X;
            int GhostY1 = 30 + Ghosts.at(i)->TileY * 8 + Ghosts.at(i)->Y;
            int GhostY2 = 30 + Ghosts.at(i)->TileY * 8 + 24 + Ghosts.at(i)->Y;


            if(GhostY1 == PlayerY1 || GhostY2 == PlayerY2){
                if(GhostX1 >= PlayerX1 && GhostX1 <= PlayerX2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true)
                        Ghosts.at(i)->SetReturnCell(true);
                    else{
                        KillPlayer();
                        for(std::size_t j; j < Ghosts.size(); j++)Ghosts.at(j)->State = 1;
                        Reset = true;
                        ResetTime = SDL_GetTicks() + 3000;
                    }
                    return true;
                }
                else if(GhostX2 >= PlayerX1 && GhostX2 <= PlayerX2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true)
                        Ghosts.at(i)->SetReturnCell(true);
                    else{
                        KillPlayer();
                        for(std::size_t j; j < Ghosts.size(); j++)Ghosts.at(j)->State = 1;
                        Reset = true;
                        ResetTime = SDL_GetTicks() + 3000;
                    }
                    return true;
                }
            }
            else if(GhostX1 == PlayerX1 || GhostX2 == PlayerX2){
                if(GhostY2 >= PlayerY1 && GhostY2 <= PlayerY2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true)
                        Ghosts.at(i)->SetReturnCell(true);
                    else{
                        KillPlayer();
                        for(std::size_t j; j < Ghosts.size(); j++)Ghosts.at(j)->State = 1;
                        Reset = true;
                        ResetTime = SDL_GetTicks() + 3000;
                    }
                    return true;
                }
                else if(GhostY1 <= PlayerY2 && GhostY1 >= PlayerY1){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true)
                        Ghosts.at(i)->SetReturnCell(true);
                    else{
                        KillPlayer();
                        for(std::size_t j; j < Ghosts.size(); j++)Ghosts.at(j)->State = 1;
                        Reset = true;
                        ResetTime = SDL_GetTicks() + 3000;
                    }
                    return true;
                }
            }
            else if( (GhostX1 > PlayerX1 && GhostX1 < PlayerX2) || (GhostX2 > PlayerX1 && GhostX2 < PlayerX2) ){
                if(GhostY1 > PlayerY1 && GhostY1 < PlayerY2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true)
                        Ghosts.at(i)->SetReturnCell(true);
                    else{
                        KillPlayer();
                        for(std::size_t j; j < Ghosts.size(); j++)Ghosts.at(j)->State = 1;
                        Reset = true;
                        ResetTime = SDL_GetTicks() + 3000;
                    }
                    return true;
                }
            }
            else if( (GhostX2 > PlayerX1 && GhostX2 < PlayerX2) || (GhostX1 > PlayerX1 && GhostX1 < PlayerX2) ){
                if(GhostY2 > PlayerY1 && GhostY2 < PlayerY2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true)
                        Ghosts.at(i)->SetReturnCell(true);
                    else{
                        KillPlayer();
                        for(std::size_t j; j < Ghosts.size(); j++)Ghosts.at(j)->State = 1;
                        Reset = true;
                        ResetTime = SDL_GetTicks() + 3000;
                    }
                    return true;
                }
            }
        }
        return false;
    }
    return true;
}

void Map::KillPlayer()
{
    Pacman->SetDead(true);
    for(std::size_t i = 0; i < Ghosts.size(); i++)
        Ghosts.at(i)->StopAI();
}

void Map::UpdatePlayerCoordinates()
{
    PlayerCoord[0] = Pacman->TileX;
    PlayerCoord[1] = Pacman->TileY;
    PlayerCoord[2] = Pacman->X;
    PlayerCoord[3] = Pacman->Y;
}

void Map::KeyPress(int Direction)
{
    int MoveParam = CheckMoveValid(Direction, Pacman->TileX, Pacman->TileY);
    if(MoveParam != CANT_MOVE)
        Pacman->SetMoving(Direction);
}

int Map::GetTile(int Line, int Column)
{
    if(Line < 0 || Line > 59 || Column < 0 || Column > 53)
        return -1;
    return Tiles[Line][Column];
}

Map::~Map()
{
    delete Pacman;
    for(std::size_t i = 0; i < Ghosts.size(); i++) delete Ghosts.at(i);
    Ghosts.clear();
    SDL_FreeSurface(Background);
    SDL_FreeSurface(Graphics);
    SDL_FreeSurface(TextLoader);
    delete Color;
    TTF_Quit();
}
