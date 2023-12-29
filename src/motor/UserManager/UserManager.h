#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "../motor.h"

int validateUser(char* username, Player* list, int listSize);
int addUser(int PID, char* username, User* list, int* listSize);
int removeUser(int PID, char* username, User* list, int* listSize);
int addPlayer(char* username, Player* list, int* listSize);
int removePlayer(char* username, Player* list, int* listSize);

#endif  // USERMANAGER_H
