#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "../motor.h"

int readLevelMap(char* path, char board[ROWS][COLLUMN]);
int loadLevelMap(char board[ROWS][COLLUMN]);  // TODO: complete func
int execBot(Motor* motor);
void* readBots(void* arg);  // TODO: fazer thread de leitura do unnamed pipe

#endif  // MAPMANAGER_H
