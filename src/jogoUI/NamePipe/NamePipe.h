#ifndef NAME_PIPE_H
#define NAME_PIPE_H

#include "../../utils/utils.h"
#include "../jogoUI.h"

int createNamePipe(UI* ui, char FIFO_NAME[]);
void* readNamePipe(void* lpram);
void writeMotor(UI* ui, CommandToServer cmd);
void writeMessage(UI* ui, char* usernmae, char* msg);

#endif  // NAME_PIPE_H