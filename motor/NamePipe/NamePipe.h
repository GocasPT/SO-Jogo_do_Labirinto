#ifndef NAMEPIPE_H
#define NAMEPIPE_H

#include "../../utils/utils.h"

// TODO: verificar esta struct
typedef struct {
    int* endThread;  //
    Level* level;    //
} ThreadData;

int createNamePipe();
void* readNamePipe(void* lpram);
void writeNamePipe(char FIFO_NAME[], DataRecive data);

#endif  // NAMEPIPE_H
