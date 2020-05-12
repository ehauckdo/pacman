#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

enum GameStates{
    STATE_NULL,
    STATE_START_MENU,
    STATE_MAIN_GAME,
    STATE_EXIT,
};

enum Entities{
    ENTITY_GENERIC,
    ENTITY_PLAYER,
    ENTITY_GHOST,
};

enum MovePositions{
    CANT_MOVE,
    GO_STRAIGHT,
    ADJUST_LEFT,
    ADJUST_RIGHT,
    ADJUST_UP,
    ADJUST_DOWN,
};

enum MoveDirections{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum Sound{
    SOUND_BEGINNING,
    SOUND_CHOMP,
    SOUND_INTERMISSION,
    SOUND_EATGHOST,
    SOUND_DEATH,
    SOUND_SIREN,
    SOUND_FINISHED,
};
#endif // DEFINE_H_INCLUDED
