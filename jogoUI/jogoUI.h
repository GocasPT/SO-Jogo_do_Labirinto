#ifndef JOGOUI_H
#define JOGOUI_H

#include "./NamePipe/NamePipe.h"
#include "./UI/UI.h"

#define PRINT_TAG "<JOGOUI> "

// Estrutura principal do jogoUI
typedef struct {
    Level level;               // Estrutura do nível
    UI ui;                     // Estrutura das janelas
    User user;                 // Estrutura do jogador
    pthread_t threadReadPipe;  // Thread de leitura dos name pipes
    char FIFO_NAME[MAX];       //
} JogoUI;

#endif  // JOGOUI_H