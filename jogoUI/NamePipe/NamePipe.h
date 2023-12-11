#ifndef NAME_PIPE_H
#define NAME_PIPE_H

#include <fcntl.h>

#include "../../utils/utils.h"

typedef struct {
    char FIFO_NAME[MAX];
    int playerFD;
    int serverFD;
} NamePipe;

typedef struct {
    Level* level;
    NamePipe* fifo;
} ThreadData;

int createNamePipe(char FIFO_NAME[]);
int closeNamePipe(char FIFO_NAME[], int* playerFD, int* serverFD);
void* readNamePipe(void* lpram);
void writeNamePipe(NamePipe* fifo, CommandToServer* cmd);

#endif  // NAME_PIPE_H