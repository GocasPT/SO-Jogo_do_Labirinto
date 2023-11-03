#include "jogoUI.h"

#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../motor/motor.h"

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

typedef struct {
    WINDOW* maze;
    WINDOW* info;
    WINDOW* notification;
    WINDOW* console;
} UI;

//* PLACEHOLDER [META 1]
void configLevel(Level* level) {
    level->level = 1;

    FILE* file = fopen("map/level1.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    char endLine[2];  // Para ler o \n do final da linha

    for (int i = 0; i < ROWS; i++) {
        // Lê uma linha do arquivo e coloca na board
        if (fgets(level->board[i], COLLUMN, file) == NULL) {
            printf("Erro ao ler o arquivo\n");
            exit(1);
        }

        // Lê o \n do final da linha e descarta
        if (fgets(endLine, 2, file) == NULL) {
            printf("Erro ao ler o arquivo\n");
            exit(1);
        }
    }

    fclose(file);
}
//* PLACEHOLDER [META 1]

void configUI(UI* ui);
void drawMaze(UI* ui, char board[ROWS][COLLUMN]);
int validateCommand(UI* ui, char* input);

int main(int argc, char* argv[]) {
    printf("Programa: jogoUI\n");

    // Verifica se o nome do jogador foi passado
    if (argc != 2) {
        printf("Falta o teu nome\n");
        return -1;
    }

    Level level;      // Estrutura que guarda o nivel
    UI ui;            // Estrutura que guarda as janelas
    char input[MAX];  // String que guarda o input do utilizador
    int key, exit;    // Variavel que guarda a tecla de movimento do utilizador

    //* PLACEHOLDER [META 1]
    configLevel(&level);
    //* PLACEHOLDER [META 1]

    initscr();
    configUI(&ui);

    //* PLACEHOLDER
    mvwprintw(ui.info, 1, 1, "Jogador: %s", argv[1]);
    wrefresh(ui.info);

    wprintw(ui.notification, "Jogo do Labirinto - SO\n");
    wrefresh(ui.notification);
    //* PLACEHOLDER

    exit = 0;
    while (exit != 1) {
        drawMaze(&ui, level.board);

        // TODO [duvida]: não mostrar as tecla na janela
        key = wgetch(ui.maze);
        switch (key) {
            case KEY_UP:
                wprintw(ui.notification, "Cima\n");
                break;
            case KEY_DOWN:
                wprintw(ui.notification, "Baixo\n");
                break;
            case KEY_LEFT:
                wprintw(ui.notification, "Esquerda\n");
                break;
            case KEY_RIGHT:
                wprintw(ui.notification, "Direita\n");
                break;
            case ' ':
                echo();
                nocbreak();
                curs_set(1);

                mvwprintw(ui.console, 1, 1, TAG);
                wrefresh(ui.console);
                wscanw(ui.console, " %100[^\n]", input);

                if (validateCommand(&ui, input) == 1)
                    exit = 1;

                wmove(ui.console, 1, 1);
                wclrtoeol(ui.console);
                wrefresh(ui.console);
                noecho();
                cbreak();
                curs_set(0);
                break;
        }
        wrefresh(ui.notification);
    }

    wgetch(ui.console);
    endwin();
    return 0;
}

void configUI(UI* ui) {
    ui->maze = newwin(H_MAZE, W_MAZE, Y_MAZE, X_MAZE);
    ui->info = newwin(H_INFO, W_INFO, Y_INFO, X_INFO);
    ui->notification = newwin(H_NOTIFICATION, W_NOTIFICATION, Y_NOTIFICATION, X_NOTIFICATION);
    ui->console = newwin(H_INPUT, W_INPUT, Y_INPUT, X_INPUT);

    box(ui->maze, 0, 0);
    box(ui->info, 0, 0);

    scrollok(ui->notification, TRUE);
    keypad(ui->maze, TRUE);

    noecho();
    cbreak();
    curs_set(0);

    wrefresh(ui->maze);
    wrefresh(ui->info);
    wrefresh(ui->notification);
    wrefresh(ui->console);
}

void drawMaze(UI* ui, char board[ROWS][COLLUMN]) {
    for (int i = 0; i < ROWS; i++)
        mvwprintw(ui->maze, i + 1, 1, "%s", board[i]);

    wrefresh(ui->maze);
}

int validateCommand(UI* ui, char* input) {
    char cmd[MAX];
    char argv[2][MAX];
    int argc;

    // TODO [DUVIDA]: argv[1] recebe lixo
    argc = sscanf(input, "%s %s %100c", cmd, argv[0], argv[1]);

    if (argc != 0) {
        if (!strcmp(cmd, "help"))
            wprintw(ui->notification, "Comando help: players, msg e exit\n");

        else if (!strcmp(cmd, "players"))
            wprintw(ui->notification, "Comando players\n");

        else if (!strcmp(cmd, "msg")) {
            if (argc < 3)
                wprintw(ui->notification, "Comando de msg invalido - Falta argumentos\n");
            else
                wprintw(ui->notification, "Comando msg\nDestinatario: %s\nMensagem: %s\n", argv[0], argv[1]);
        }

        else if (!strcmp(cmd, "exit")) {
            wprintw(ui->notification, "Comando exit\nPreciona 'Enter' para sair");
            wrefresh(ui->notification);
            return 1;
        }

        else
            wprintw(ui->notification, "Comnado invalido\n");
    }

    wrefresh(ui->notification);
    return 0;
}