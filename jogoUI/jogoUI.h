#ifndef JOGOUI_H
#define JOGOUI_H

#include "./NamePipe/NamePipe.h"
#include "./UI/UI.h"

// Estrutura principal do jogoUI
typedef struct {
    Level level;         // Estrutura do nível
    UI ui;               // Estrutura das janelas
    User user;           // Estrutura do jogador
    pthread_t threadFD;  // Thread de leitura dos name pipes
    NamePipe fifo;
} JogoUI;

typedef struct {
    pthread_t* thread;
    NamePipe* fifo;
} GlobalData;

#endif  // JOGOUI_H