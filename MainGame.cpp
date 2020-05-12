#include "MainGame.h"

//--------------------------------------------
// MainGame::MainGame()
//--------------------------------------------
MainGame::MainGame()
{
    Background = SurfaceHandler::Load((char*)"./Graphics/Background.png", false);
    Graphics = SurfaceHandler::Load((char*)"./Graphics/Graphics.png", true);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512);
    SoundControl = new SoundBank();
    SoundControl->Load((char*)"./Gfx/pacman_beginning.wav");
    SoundControl->Load((char*)"./Gfx/pacman_chomp.wav");
    SoundControl->Load((char*)"./Gfx/pacman_intermission.wav");
    SoundControl->Load((char*)"./Gfx/pacman_eatghost.wav");
    SoundControl->Load((char*)"./Gfx/pacman_death.wav");
    SoundControl->Load((char*)"./Gfx/pacman_siren.wav");
    SoundControl->Load((char*)"./Gfx/pacman_finished.wav");

    Lives = 3;

    LoadScore();

    TTF_Init();
    Font = TTF_OpenFont((char*)"./Data/Joystix.ttf", 16);

    Color = new SDL_Color();
    Color->b = Color->g = Color->r = 255;

    DotsEaten = 0;
    HoldTime = 0;

    RestartGame();
}

//------------------------------------------------------------------
// Pacman::TryAgain()
//
// Called when you dies with Pacman. Reset the entities position and
// decrease your lives by one. If no lives remaining, set NextState to
// Start Menu.
//------------------------------------------------------------------
void MainGame::TryAgain()
{
    if(HoldTime < SDL_GetTicks()){

        Lives -= 1;

        if(Lives < 0){
            if(Score > TopScore) SaveScore();
            NextState = STATE_START_MENU;
        }

        ResetEntities();

        Reset = false;

        WaitTime = SDL_GetTicks() + 4200;
    }
}

//------------------------------------------------------------------
// MainGame::RestartGame()
//
// It's called whenever the game needed to be started (just started or player finisihed
// the level). It will reset entities and reload the map.
//------------------------------------------------------------------
void MainGame::RestartGame()
{
        if(HoldTime > SDL_GetTicks()) return;

        DotsEaten = 0;
        ResetEntities();

        // Initialize bidimensional array used as the Map. Values will be read from file
        FILE* FileHandle = fopen("./Data/Map", "r");
        if(FileHandle == NULL) return;

        for(int Line = 0; Line < 60; Line++){
            for(int Column = 0; Column < 54; Column++)
                fscanf(FileHandle, "%d ", &Map[Line][Column]);
            fscanf(FileHandle, "\n");
        }

        fclose(FileHandle);

        WaitTime = SDL_GetTicks() + 4200;

}

//------------------------------------------------------------------
// MainGame::Update()
//
// Important notes:
// -The beginning sound is set to play whenever WaitTime is initialized
// If new functions with sound needs to be implemented in the future,
// make sure on using HoldTime for holding the state while the sound is playing
//------------------------------------------------------------------
void MainGame::Update()
{
    if(WaitTime < SDL_GetTicks()){

        if(Pacman->Dead) TryAgain();

        if(DotsEaten >= 276){
            RestartGame();
            return;
        }

        //update player and ghosts
        Pacman->Update();
        for(std::size_t i = 0; i < Ghosts.size(); i++){
            UpdateMoveDirections(Ghosts.at(i), Ghosts.at(i)->TileX, Ghosts.at(i)->TileY);
            Ghosts.at(i)->UpdatePlayerCoordinates(Pacman->TileX, Pacman->TileY);
            Ghosts.at(i)->Update();
        }

        if(BonusMessageTime > SDL_GetTicks()){
            for(std::size_t i = 0; i < Bonus.size(); i++){
                Bonus.at(i)->Update();
            }
        }
        else if(Bonus.empty() == false) Bonus.clear();

        // process moves set by entities
        ProcessMoves();
    }

    else SoundControl->Play(SOUND_BEGINNING);
}

//--------------------------------------------
// MainGame::Render(SDL_Surface* Display)
//--------------------------------------------
void MainGame::Render(SDL_Surface* Display)
{
    SurfaceHandler::Draw(Display, Background, 0, 0);

    // Map related stuff
    for(int Line = 0; Line < 60; Line++){
        for(int Column = 0; Column < 54; Column++)
            if(Map[Line][Column] == 2)
                SurfaceHandler::Draw(Display, Graphics, 4 + Column * 8, 30 + Line * 8, 144, 144, 8, 8);
            else if(Map[Line][Column] == 3)
                SurfaceHandler::Draw(Display, Graphics, 4 + Column * 8, 30 + Line * 8, 152, 144, 8, 8);
    }

    // Entities
    Pacman->Render(Display, Graphics);
    for(std::size_t i = 0; i < Ghosts.size(); i++)
        Ghosts.at(i)->Render(Display, Graphics);

    // Score related stuff
    for(int i = 0; i < Lives; i++)
        SurfaceHandler::Draw(Display, Graphics, 50-i*25, 0, 0, 96, 24, 24);
    char buffer[15];
    sprintf(buffer, "%d", Score);
    TextLoader = TTF_RenderText_Blended(Font, buffer, *Color);
    SurfaceHandler::Draw(Display, TextLoader, 80, 0);
    SDL_FreeSurface(TextLoader);
    sprintf(buffer, "%d", TopScore);
    TextLoader = TTF_RenderText_Blended(Font, buffer, *Color);
    SurfaceHandler::Draw(Display, TextLoader, 440-16*TopScorePos, 0);
    SDL_FreeSurface(TextLoader);
    sprintf(buffer, "%s", "HIGHSCORE");
    TextLoader = TTF_RenderText_Blended(Font, buffer, *Color);
    SurfaceHandler::Draw(Display, TextLoader, 315-16*TopScorePos, 0);
    SDL_FreeSurface(TextLoader);

    // Messages from eating ghosts
    for(std::size_t i = 0; i < Bonus.size(); i++){
        if(Bonus.at(i)->Show)
            SurfaceHandler::Draw(Display, Graphics, Bonus.at(i)->X, Bonus.at(i)->Y, 72+i*24, 240, 24, 24);
    }
}

//-------------------------------------------------------------------------------------
// MainGame::UpdateMoveDirections(Ghost* CurrentGhost, int TileX, int TileY)
//
// Important notes:
// -If no directions were found, it meant the ghost reached a deadend, so puts the opposite
// direction on the walkable ways
// -Ghost entities have special states where the walkable directions need to be treated
// separately, that's why the two last if conditions. Until a better approach is implemented,
// they must be kept there
//-------------------------------------------------------------------------------------
void MainGame::UpdateMoveDirections(Ghost* CurrentGhost, int TileX, int TileY)
{
    CurrentGhost->WalkableDirections.clear();

    // Check availability of up-right-down-left directions, if positive, tells ghost about it
    if(Map[TileY-1][TileX] != 1 && Map[TileY-1][TileX+2] != 1 && DOWN != CurrentGhost->MovingDirection)
        CurrentGhost->WalkableDirections.push_back(UP);
    if(Map[TileY+3][TileX] != 1 && Map[TileY+3][TileX+2] != 1 && UP != CurrentGhost->MovingDirection)
        CurrentGhost->WalkableDirections.push_back(DOWN);
    if( (Map[TileY][TileX-1] != 1 && Map[TileY+2][TileX-1] != 1 && RIGHT != CurrentGhost->MovingDirection) || TileX-1 < 0)
        CurrentGhost->WalkableDirections.push_back(LEFT);
    if( (Map[TileY][TileX+3] != 1 && Map[TileY+2][TileX+3] != 1 && LEFT != CurrentGhost->MovingDirection) || TileX+3 > 53)
        CurrentGhost->WalkableDirections.push_back(RIGHT);

    // if none direction was found, means it reached a dead end, so tells ghost to turn backwards
    if(CurrentGhost->WalkableDirections.empty())
        CurrentGhost->WalkableDirections.push_back(CurrentGhost->GetOppositeDirection());

    // if ghost is about to leave the cage, tells him he can goes up
    if(CurrentGhost->State == 1){
        if(TileX >= 25 && TileX <= 28 && TileY <= 25 && TileY >= 26)
            CurrentGhost->WalkableDirections.push_back(UP);
    }

    // if ghost is about to enter the cage, tells him he can goes down
    if(CurrentGhost->State == 4){
        if(TileX >= 25 && TileX <= 27 && TileY >= 21 && TileY <= 24)
            CurrentGhost->WalkableDirections.push_back(DOWN);
    }
}

//-------------------------------------------------------------------------------------
// MainGame::IsMoveValid(int Direction, int Column, int Line)
//
// Receives a moving direction and a position and checks wether the move is possible
// or not. Returns a parameter defined on Define.h
//-------------------------------------------------------------------------------------
int MainGame::IsMoveValid(int Direction, int Column, int Line)
{
    if(Direction == DOWN){
        if(Map[Line+3][Column] != 1 && Map[Line+3][Column+2] != 1)
            return GO_STRAIGHT;
        else if(Map[Line+3][Column+1] != 1 && Map[Line+3][Column+3] != 1)
            return ADJUST_RIGHT;
        else if(Map[Line+3][Column-1] != 1 && Map[Line+3][Column+1] != 1)
            return ADJUST_LEFT;
    }

    else if(Direction == UP){
        if(Map[Line-1][Column] != 1 && Map[Line-1][Column+2] != 1)
            return GO_STRAIGHT;
        else if(Map[Line-1][Column+1] != 1 && Map[Line-1][Column+3] != 1)
            return ADJUST_RIGHT;
        else if(Map[Line-1][Column-1] != 1 && Map[Line-1][Column+1] != 1)
            return ADJUST_LEFT;
    }

    else if(Direction == RIGHT){
        if( (Map[Line][Column+3] != 1 && Map[Line+2][Column+3] != 1) || Column+3 > 53 )
            return GO_STRAIGHT;
        else if(Map[Line-1][Column+3] != 1 && Map[Line+1][Column+3] != 1)
            return ADJUST_UP;
        else if(Map[Line+1][Column+3] != 1 && Map[Line+3][Column+3] != 1)
            return ADJUST_DOWN;
    }

    else if(Direction == LEFT){
        if( (Map[Line][Column-1] != 1 && Map[Line+2][Column-1] != 1) || Column-1 < 0 )
            return GO_STRAIGHT;
        else if(Map[Line+1][Column-1] != 1 && Map[Line+3][Column-1] != 1)
            return ADJUST_DOWN;
        else if(Map[Line-1][Column-1] != 1 && Map[Line+1][Column-1] != 1)
            return ADJUST_UP;
    }

    return CANT_MOVE;
}

//-------------------------------------------------------------------------------------
// MainGame::ProcessMoves()
//
// Moves the player and the entities and check if some collision has happened
//-------------------------------------------------------------------------------------
void MainGame::ProcessMoves()
{
    int MoveParam = IsMoveValid(Pacman->MovingDirection, Pacman->TileX, Pacman->TileY);
    Pacman->Move(MoveParam);
    PacmanEats();

    for(std::size_t i = 0; i < Ghosts.size(); i++){
        MoveParam = IsMoveValid(Ghosts.at(i)->MovingDirection, Ghosts.at(i)->TileX, Ghosts.at(i)->TileY);
        Ghosts.at(i)->Move(MoveParam);
    }
    CollisionCheck();
}

//-------------------------------------------------------------------------------------
// MainGame::CollisionCheck()
//
// Checks if some collision happened based on the coordinates of Player and Ghosts
// If a collision happened three things can happen:
// -The ghost is in the invisible state, so just ignore
// -The ghost has weakness set to true, so kill the ghost and do stuff related
// -The ghost is on it's normal state so the player is killed, AI is immediately stopped
//-------------------------------------------------------------------------------------
void MainGame::CollisionCheck()
{
    if(Pacman->Dead == false){
        int PlayerX1 = 4 + Pacman->TileX * 8 + Pacman->X + 1;
        int PlayerX2 = 4 + Pacman->TileX * 8 + 24 + Pacman->X - 1;
        int PlayerY1 = 30 + Pacman->TileY * 8 + Pacman->Y + 1;
        int PlayerY2 = 30 + Pacman->TileY * 8 + 24 + Pacman->Y - 1;

        for(std::size_t i = 0; i < Ghosts.size(); i++){
            int GhostX1 = 4 + Ghosts.at(i)->TileX * 8 + Ghosts.at(i)->X + 1;
            int GhostX2 = 4 + Ghosts.at(i)->TileX * 8 + 24 + Ghosts.at(i)->X - 1;
            int GhostY1 = 30 + Ghosts.at(i)->TileY * 8 + Ghosts.at(i)->Y + 1;
            int GhostY2 = 30 + Ghosts.at(i)->TileY * 8 + 24 + Ghosts.at(i)->Y - 1;


            if(GhostY1 == PlayerY1 || GhostY2 == PlayerY2){
                if(GhostX1 >= PlayerX1 && GhostX1 <= PlayerX2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true){
                        Bonus.push_back(new BonusMessage(4+Pacman->TileX*8+Pacman->X, 30+Pacman->TileY*8+Pacman->Y));
                        Ghosts.at(i)->SetReturnCell(true);
                        SoundControl->Stop(SOUND_EATGHOST);
                        SoundControl->Play(SOUND_EATGHOST);
                        Score += (pow(2, Bonus.size())) * 100;
                    }
                    else{
                        SoundControl->Play(SOUND_DEATH);
                        Pacman->SetDead(true);
                        for(std::size_t j = 0; j < Ghosts.size(); j++)
                                Ghosts.at(j)->StopAI();
                        Reset = true;
                        HoldTime = SDL_GetTicks() + 3000;
                    }
                }
                else if(GhostX2 >= PlayerX1 && GhostX2 <= PlayerX2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true){
                        Bonus.push_back(new BonusMessage(4+Pacman->TileX*8+Pacman->X, 30+Pacman->TileY*8+Pacman->Y));
                        Ghosts.at(i)->SetReturnCell(true);
                        SoundControl->Stop(SOUND_EATGHOST);
                        SoundControl->Play(SOUND_EATGHOST);
                        Score += (pow(2, Bonus.size())) * 100;
                    }
                    else{
                        SoundControl->Play(SOUND_DEATH);
                        Pacman->SetDead(true);
                        for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                        Reset = true;
                        HoldTime = SDL_GetTicks() + 3000;
                    }
                }
            }
            else if(GhostX1 == PlayerX1 || GhostX2 == PlayerX2){
                if(GhostY2 >= PlayerY1 && GhostY2 <= PlayerY2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true){
                        Bonus.push_back(new BonusMessage(4+Pacman->TileX*8+Pacman->X, 30+Pacman->TileY*8+Pacman->Y));
                        Ghosts.at(i)->SetReturnCell(true);
                        SoundControl->Stop(SOUND_EATGHOST);
                        SoundControl->Play(SOUND_EATGHOST);
                        Score += (pow(2, Bonus.size())) * 100;
                    }
                    else{
                        SoundControl->Play(SOUND_DEATH);
                        Pacman->SetDead(true);
                        for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                        Reset = true;
                        HoldTime = SDL_GetTicks() + 3000;
                    }
                }
                else if(GhostY1 <= PlayerY2 && GhostY1 >= PlayerY1){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true){
                        Bonus.push_back(new BonusMessage(4+Pacman->TileX*8+Pacman->X, 30+Pacman->TileY*8+Pacman->Y));
                        Ghosts.at(i)->SetReturnCell(true);
                        SoundControl->Stop(SOUND_EATGHOST);
                        SoundControl->Play(SOUND_EATGHOST);
                        Score += (pow(2, Bonus.size())) * 100;
                    }
                    else{
                        SoundControl->Play(SOUND_DEATH);
                        Pacman->SetDead(true);
                        for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                        Reset = true;
                        HoldTime = SDL_GetTicks() + 3000;
                    }
                }
            }
            else if( (GhostX1 > PlayerX1 && GhostX1 < PlayerX2) || (GhostX2 > PlayerX1 && GhostX2 < PlayerX2) ){
                if(GhostY1 > PlayerY1 && GhostY1 < PlayerY2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true){
                        Bonus.push_back(new BonusMessage(4+Pacman->TileX*8+Pacman->X, 30+Pacman->TileY*8+Pacman->Y));
                        Ghosts.at(i)->SetReturnCell(true);
                        SoundControl->Stop(SOUND_EATGHOST);
                        SoundControl->Play(SOUND_EATGHOST);
                        Score += (pow(2, Bonus.size())) * 100;
                    }
                    else{
                        SoundControl->Play(SOUND_DEATH);
                        Pacman->SetDead(true);
                        for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                        Reset = true;
                        HoldTime = SDL_GetTicks() + 3000;
                    }
                }
            }
            else if( (GhostX2 > PlayerX1 && GhostX2 < PlayerX2) || (GhostX1 > PlayerX1 && GhostX1 < PlayerX2) ){
                if(GhostY2 > PlayerY1 && GhostY2 < PlayerY2){
                    if(Ghosts.at(i)->State == 4)
                        continue;
                    if(Ghosts.at(i)->Weak == true){
                        Bonus.push_back(new BonusMessage(4+Pacman->TileX*8+Pacman->X, 30+Pacman->TileY*8+Pacman->Y));
                        Ghosts.at(i)->SetReturnCell(true);
                        SoundControl->Stop(SOUND_EATGHOST);
                        SoundControl->Play(SOUND_EATGHOST);
                        Score += (pow(2, Bonus.size())) * 100;
                    }
                    else{
                        SoundControl->Play(SOUND_DEATH);
                        Pacman->SetDead(true);
                        for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                        Reset = true;
                        HoldTime = SDL_GetTicks() + 3000;
                    }
                }
            }
        }
    }

}

//-------------------------------------------------------------------------------------
// MainGame::PacmanEats()
//
// Will check if player is walking over a dot, doing the appropriate stuff accordingly
// If he has eaten the 276 dots of the level, restart it
//-------------------------------------------------------------------------------------
void MainGame::PacmanEats()
{
    for(int Line = 0; Line < 3; Line++)
        for(int Column = 0; Column < 3; Column++)
            if(Map[Pacman->TileY+Line][Pacman->TileX+Column] == 2){
                Map[Pacman->TileY+Line][Pacman->TileX+Column] = 0;
                Score += 10;
                DotsEaten += 1;
                if(DotsEaten < 276)
                    SoundControl->Play(SOUND_CHOMP);
                else{
                    HoldTime = SDL_GetTicks() + 2000;
                    SoundControl->StopAll();
                    SoundControl->Play(SOUND_FINISHED);
                    for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                }
                return;
            }
            else if(Map[Pacman->TileY+Line][Pacman->TileX+Column] == 3){
                Map[Pacman->TileY+Line][Pacman->TileX+Column] = 0;
                Score += 50;
                for(std::size_t i = 0; i < Ghosts.size(); i++)
                    Ghosts.at(i)->SetWeakness(true);
                DotsEaten += 1;
                if(DotsEaten < 276){
                    SoundControl->Stop(SOUND_INTERMISSION);
                    SoundControl->Play(SOUND_INTERMISSION);
                    BonusMessageTime = SDL_GetTicks() + 6000;
                }
                else{
                    HoldTime = SDL_GetTicks() + 2000;
                    SoundControl->StopAll();
                    SoundControl->Play(SOUND_FINISHED);
                    for(std::size_t j = 0; j < Ghosts.size(); j++)
                            Ghosts.at(j)->StopAI();
                }
                return;
            }

}

//--------------------------------------------
// MainGame::ResetEntities()
//--------------------------------------------
void MainGame::ResetEntities()
{
    Pacman = new Player();
    // Cleans the Ghosts vector (if there's any) for a new game
    if(Ghosts.empty() == false){
        for(std::size_t i = 0; i < Ghosts.size(); i++) delete Ghosts.at(i);
        Ghosts.clear();
    }

    // Initialize the Ghosts again on the appropriate position
    Ghosts.push_back(new Ghost(0, 26, 21));
    Ghosts.at(0)->State = 2;
    for(int i = 0; i < 3; i++){
        Ghosts.push_back(new Ghost(i+1, 22 + i*3, 27));
        Ghosts.at(i)->FreeTimeCounter = SDL_GetTicks() + 4200 + i*5000;
    }
}

//--------------------------------------------
// MainGame::HandleEvent()
//--------------------------------------------
void MainGame::HandleEvent()
{
    SDL_Event Event;
    while(SDL_PollEvent(&Event)){
        switch(Event.type){
            case SDL_QUIT:
                NextState = STATE_EXIT;
                break;
            case SDL_KEYDOWN:
                KeyPress(Event.key.keysym.sym, Event.key.keysym.mod, Event.key.keysym.unicode);
                break;
        }
    }
    return;
}

//---------------------------------------------------------------
// MainGame::KeyPress(SDLKey sym, SDLMod mod, Uint16 unicode)
//
// Process the players input
//---------------------------------------------------------------
void MainGame::KeyPress(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    int Direction;
    switch(sym){
        case SDLK_DOWN:     Direction = DOWN;   break;
        case SDLK_UP:       Direction = UP;     break;
        case SDLK_RIGHT:    Direction = RIGHT;  break;
        case SDLK_LEFT:     Direction = LEFT;   break;
        default:            return;
    }

    int MoveParam = IsMoveValid(Direction, Pacman->TileX, Pacman->TileY);
    if(MoveParam != CANT_MOVE)
        Pacman->SetMoving(Direction);
}

//---------------------------------------------------------------
// MainGame::LoadScore()
//
// Important notes
// -The file must be a clean text with only the number at the first position
//---------------------------------------------------------------
void MainGame::LoadScore()
{
    Score = 0;

    FILE* FileHandle = fopen("./Data/Scores", "r");
    if(FileHandle == NULL) return;

    fscanf(FileHandle, "%d", &TopScore);

    fclose(FileHandle);

    int Digits = 0, Number;
    Number = TopScore;

    while(Number){
        Number /= 10;
        Digits++;
    }

    TopScorePos = Digits;
}

//---------------------------------------------------------------
// MainGame::SaveScore()
//
// Important notes
// -The file must be a clean text with only the number at the first position
//---------------------------------------------------------------
void MainGame::SaveScore()
{
    if(Score > TopScore){
        FILE* FileHandle = fopen("./Data/Scores", "w+");
        if(FileHandle == NULL) return;

        fprintf(FileHandle, "%d", Score);

        fclose(FileHandle);
    }
}

//---------------------------------------------------------------
// MainGame::~MainGame()
//---------------------------------------------------------------
MainGame::~MainGame()
{
    delete Pacman;
    for(std::size_t i = 0; i < Ghosts.size(); i++)
        delete Ghosts.at(i);
    Ghosts.clear();
    for(std::size_t i = 0; i < Bonus.size(); i++)
        delete Bonus.at(i);
    Bonus.clear();

    SoundControl->Cleanup();
    delete SoundControl;
    Mix_CloseAudio();

    SDL_FreeSurface(Graphics);
    SDL_FreeSurface(Background);

    TTF_CloseFont(Font);
    TTF_Quit();
    delete Color;
}
