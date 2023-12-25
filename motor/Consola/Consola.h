#ifndef CONSOLA_H
#define CONSOLA_H

#include "../motor.h"

int validateCommand(char* input, Motor* motor);
void showInfo(Motor motor, char type);

#endif  // CONSOLA_H
