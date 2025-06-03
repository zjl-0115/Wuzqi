#ifndef STDSHARED_H
#define STDSHARED_H

enum ROLE
{
    EMPTY=0,      /*没有人下*/
    PLAYER=1,     /*玩家落子*/
    PLAYER2=2,    /*玩家2落子*/
    COMPUTER=2,   /*人机落子*/
};

enum MODE {
    SINGLE_PLAYER,
    DOUBLE_PLAYER
};

#endif // STDSHARED_H
