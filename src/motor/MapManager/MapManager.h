#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "../motor.h"

int readLevelMap(char* path, char board[ROWS][COLLUMN]);
int loadLevelMap(char board[ROWS][COLLUMN]);                                                                                                                     // TODO: complete func
Level exportLevel(Level level, Player* playerList, int playerListSize /*, Rock* rockList, int rockListSize, MoveBlock* moveBlockList, int moveBlockListSize*/);  // TODO: complete func
int execBot(Motor* motor);
void* readBots(void* arg);  // TODO: fazer thread de leitura do unnamed pipe
void movePlayer(char board[ROWS][COLLUMN], Player* player, char* direction);

#endif  // MAPMANAGER_H
