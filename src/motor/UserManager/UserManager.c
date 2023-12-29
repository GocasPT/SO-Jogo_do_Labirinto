#include "UserManager.h"

// TODO: docs
int validateUser(char* username, Player* list, int listSize) {
    for (int i = 0; i < listSize; i++) {
        if (strcmp(username, list[i].username) == 0)
            return 1;
    }

    return 0;
}

// TODO: docs
int addUser(int PID, char* username, User* list, int* listSize) {
    printf("Adicionando o utilizador %s\n", username);

    if (validateUser(username, list, *listSize))
        return 1;

    User newUser = {
        PID,
        username,
    };

    list[*listSize] = newUser;
    (*listSize)++;

    // TODO: if true addPlayer

    return 0;
}

// TODO: docs
int removeUser(int PID, char* username, User* list, int* listSize) {
    for (int i = 0; i < *listSize; i++) {
        if (list[i].PID == PID) {
            for (int j = i; j < *listSize - 1; j++)
                list[j] = list[j + 1];
            (*listSize)--;
            return 1;
        }
    }

    return 0;
}

// TODO: addPlayer
// TODO: docs
int addPlayer(char* username, Player* list, int* listSize) {
    return 0;
}

// TODO: removePlayer
// TODO: docs
int removePlayer(char* username, Player* list, int* listSize) {
    return 0;
}