#include "UserManager.h"

#include <stdio.h>
#include <string.h>

// TODO: docs
int validateUser(char* username, User* list, int listSize) {
    for (int i = 0; i < listSize; i++) {
        if (strcmp(username, list[i].username) == 0) {
            printf("%s Já existe um %s\n", TAG_MOTOR, username);
            return 1;
        }
    }

    return 0;
}

// TODO: docs
int addUser(int PID, char* username, User* list, int* listSize, Player* playerList, int* playerListSize) {
    if (validateUser(username, list, *listSize))
        return 1;

    User newUser = {
        .PID = PID,
    };
    strcpy(newUser.username, username);

    list[*listSize] = newUser;
    (*listSize)++;

    printf("%s %s [%d] adicionado\n", TAG_MOTOR, newUser.username, newUser.PID);

    // TODO: if true addPlayer
    addPlayer(username, playerList, playerListSize);

    return 0;
}

// TODO: docs
int removeUser(int PID, char* username, User* list, int* listSize, Player* playerList, int* playerListSize) {
    for (int i = 0; i < *listSize; i++) {
        if (list[i].PID == PID) {
            for (int j = i; j < *listSize - 1; j++)
                list[j] = list[j + 1];
            (*listSize)--;
            return 0;
        }
    }

    removePlayer(username, playerList, playerListSize);

    return 1;
}

// TODO: addPlayer
// TODO: docs
int addPlayer(char* username, Player* list, int* listSize) {
    // TODO: coor random
    Player newPlayer = {
        .x = 1,
        .y = 1};

    strcpy(newPlayer.username, username);
    list[*listSize] = newPlayer;
    (*listSize)++;

    return 0;
}

// TODO: removePlayer
// TODO: docs
int removePlayer(char* username, Player* list, int* listSize) {
    for (int i = 0; i < *listSize; i++) {
        if (strcmp(list[i].username, username) == 0) {
            for (int j = i; j < *listSize - 1; j++)
                list[j] = list[j + 1];
            (*listSize)--;
            return 0;
        }
    }

    return 0;
}

// TODO: docs
Player* getPlayer(Player* list, int listSize, char* username) {
    for (int i = 0; i < listSize; i++) {
        if (strcmp(list[i].username, username) == 0)
            return &list[i];
    }

    return NULL;
}

// TODO: docs
User* getUser(User* list, int listSize, int PID) {
    for (int i = 0; i < listSize; i++) {
        if (list[i].PID == PID)
            return &list[i];
    }

    return NULL;
}

// TODO: docs
User* getUserByUsername(User* list, int listSize, char* username) {
    for (int i = 0; i < listSize; i++) {
        if (strcmp(list[i].username, username) == 0)
            return &list[i];
    }

    return NULL;
}