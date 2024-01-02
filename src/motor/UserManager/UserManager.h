#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "../motor.h"

int validateUser(char* username, User* list, int listSize);
int addUser(int PID, char* username, User* list, int* listSize, Player* playerList, int* playerListSize);
int removeUser(int PID, char* username, User* list, int* listSize, Player* playerList, int* playerListSize);
int addPlayer(char* username, Player* list, int* listSize);
int removePlayer(char* username, Player* list, int* listSize);
Player* getPlayer(Player* list, int listSize, char* username);
User* getUser(User* list, int listSize, int PID);

#endif  // USERMANAGER_H
