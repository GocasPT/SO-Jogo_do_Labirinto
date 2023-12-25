#ifndef NAMEPIPE_H
#define NAMEPIPE_H

#include "../../utils/utils.h"

typedef struct {
    int* endThread;       //
    char FIFO_NAME[MAX];  //
    Level* level;         //
} ThreadData;

int createNamePipe(char FIFO_NAME[]);
void* readNamePipe(void* lpram);
void writeNamePipe(char FIFO_NAME[], CommandToServer cmd);

#endif  // NAMEPIPE_H
