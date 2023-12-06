#ifndef UI_H
#define UI_H

#include <ncurses.h>

#include "../../utils/utils.h"

#define TAG ">> "

#define W_MAZE COLLUMN + 2
#define H_MAZE ROWS + 2
#define W_INFO 55 + 2
#define H_INFO 10 + 2
#define W_NOTIFICATION W_INFO
#define H_NOTIFICATION 5 + 2
#define W_INPUT W_MAZE
#define H_INPUT 1 + 2

#define X_MAZE 1
#define Y_MAZE 1
#define X_INFO X_MAZE + COLLUMN + 2
#define Y_INFO Y_MAZE
#define X_NOTIFICATION X_INFO
#define Y_NOTIFICATION Y_INFO + H_INFO
#define X_INPUT X_MAZE
#define Y_INPUT Y_MAZE + ROWS + 2

// Estrutura que guarda as janelas
typedef struct {
    WINDOW* maze;   // Janela do tabuleiro
    WINDOW* info;   // Janela das informações
    WINDOW* notification;   // Janela das notificações
    WINDOW* console;    // Janela do console
} UI;

// Estrutura principal do jogoUI
typedef struct {
    Level level;    // Estrutura do nível
    UI ui;  // Estrutura das janelas
    User user;  // Estrutura do jogador
} JogoUI;

void configUI(UI* ui);
void drawMaze(UI* ui, char board[ROWS][COLLUMN]);
int validateCommand(UI* ui, char* input);
void readInput(JogoUI* jogoUI);

#endif  // UI_H
