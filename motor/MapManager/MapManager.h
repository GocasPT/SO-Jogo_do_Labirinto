#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "../motor.h"

int readLevelMap(char* path, char board[ROWS][COLLUMN]);
int execBot(Motor* motor);

#endif  // MAPMANAGER_H
