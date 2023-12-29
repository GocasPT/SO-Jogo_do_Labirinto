#ifndef NAME_PIPE_H
#define NAME_PIPE_H

#include "../../utils/utils.h"

int createNamePipe(char FIFO_NAME[]);
void* readNamePipe(void* lpram);
void writeNamePipe(char FIFO_NAME[], CommandToServer cmd);

#endif  // NAME_PIPE_H