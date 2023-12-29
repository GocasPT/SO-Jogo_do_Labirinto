#ifndef NAMEPIPE_H
#define NAMEPIPE_H

#include "../../utils/utils.h"

int createNamePipe();
void* readNamePipe(void* lpram);
void writeNamePipe(char FIFO_NAME[], DataRecive data);

#endif  // NAMEPIPE_H
