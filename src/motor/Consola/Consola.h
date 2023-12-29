#ifndef CONSOLA_H
#define CONSOLA_H

#include "../motor.h"

void readConsola(Motor* motor, int* endFlag);
int validateCommand(char* input, Motor* motor);
void showInfo(Motor motor, char type);

#endif  // CONSOLA_H
