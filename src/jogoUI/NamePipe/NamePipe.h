#ifndef NAME_PIPE_H
#define NAME_PIPE_H

#include "../../utils/utils.h"
#include "../UI/UI.h"

typedef struct {
    int* endThread;       //
    char FIFO_NAME[MAX];  //
    Level* level;         //
    UI* ui;               //
} ThreadData;

int createNamePipe(char FIFO_NAME[]);
void* readNamePipe(void* lpram);
void writeNamePipe(char FIFO_NAME[], CommandToServer cmd);

#endif  // NAME_PIPE_H