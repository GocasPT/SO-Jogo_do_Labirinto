#ifndef MOTOR_H
#define MOTOR_H

#include "../bot/bot.h"
#include "../jogoUI/jogoUI.h"

#define ROWS 16
#define COLLUMN 40 + 1  // +1 para o \0
#define MAX 100

#define MAX_LEVEL 3
#define MAX_USER 5
#define MAX_BOT 10
#define MAX_ROCK 50
#define MAX_MOVE_BLOCK5 5

typedef struct {
    char board[ROWS][COLLUMN];
    int level;
} Level;

typedef struct {
    int x, y;
    int dir;
    int step;
} MoveBlock;

typedef struct {
    int x, y;
    int time;
} Rock;

typedef struct {
    Level level;
    int timerBegin;
    int timerGame, stepTimerGame;
    int nUserMin;
    int nUserOn, nBotOn, nRockOn, nMoveBlockOn;
    User userList[MAX_USER];
    Bot botList[MAX_BOT];
    Rock rockList[MAX_ROCK];
    MoveBlock moveBlockList[MAX_MOVE_BLOCK5];
} Motor;

#endif  // MOTOR_H