#ifndef JOGOUI_H
#define JOGOUI_H

#include "../utils/utils.h"
#include "./UI/UI.h"

#define PRINT_TAG "<JOGOUI> "

typedef struct {
    int* endThread;       //
    char FIFO_NAME[MAX];  //
    Level* level;         //
    UI* ui;               //
} ThreadData;

// Estrutura principal do jogoUI
typedef struct {
    Level level;               // Estrutura do nível
    UI ui;                     // Estrutura das janelas
    User user;                 // Estrutura do jogador
    char FIFO_NAME[MAX];       //
    pthread_t threadReadPipe;  // Thread de leitura do name pipe
} JogoUI;

#endif  // JOGOUI_H