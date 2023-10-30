#include "jogoUI.h"

#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../motor/motor.h"

#define TAG ">> "

#define X 0
#define Y_MAZE 3
#define Y_NOTIFICATION Y_MAZE + ROWS + 3
#define Y_INPUT Y_NOTIFICATION + 2

// META 1
void configLevel(Level* level) {
    level->level = 1;

    FILE* file = fopen("map/level1.txt", "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    char endLine[2];  // Para ler o \n do final da linha

    for (int i = 0; i < ROWS; i++) {
        // Lê uma linha do arquivo e coloca na board
        if (fgets(level->board[i], COLLUMN, file) == NULL) {
            printf("Erro ao ler o arquivo\n");
            return;
        }

        // Lê o \n do final da linha e descarta
        if (fgets(endLine, 2, file) == NULL) {
            printf("Erro ao ler o arquivo\n");
            return;
        }
    }

    fclose(file);
}

// TODO: check ultimate
void clearInput() {
    move(Y_INPUT, 0);
    clrtoeol();
}

// Função Extra para escrever as mensagens das notificações mais bonito
void writeNotification(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[MAX];
    vsnprintf(buffer, sizeof(buffer), format, args);

    mvprintw(Y_NOTIFICATION, X, "%s", buffer);

    va_end(args);
}

void drawMaze(char board[ROWS][COLLUMN]) {
    for (int i = 0; i < ROWS; i++)
        mvprintw(Y_MAZE + i, X, "%s", board[i]);
}

int validateCommand(char* input) {
    char cmd[MAX];
    char argv[2][MAX];
    int argc;

    // TODO: [DUVIDA] argv[1] recebe lixo
    if (argc = sscanf(input, "%s %s %100c", cmd, argv[0], argv[1])) {
        if (!strcmp(cmd, "help"))
            writeNotification("Comando help: players, msg e exit\n");

        else if (!strcmp(cmd, "players"))
            writeNotification("Comando players\n");

        else if (!strcmp(cmd, "msg")) {
            if (argc < 3)
                writeNotification("Comando de msg invalido - Falta argumentos\n");
            else
                writeNotification("Comando msg Destinatario: %s Mensagem: %s\n", argv[0], argv[1]);
        }

        else if (!strcmp(cmd, "exit")) {
            writeNotification("Comando exit\n");
            return 1;
        }

        else
            writeNotification("Comnado invalido\n");
    }

    return 0;
}

// TODO: configurar as janelas (maze, info, notificação e input)
int main(int argc, char* argv[]) {
    printf("Programa: jogoUI\n");

    if (argc != 2) {
        printf("Falta o teu nome\n");
        return -1;
    }

    char input[MAX];
    Level level;

    // META 1
    configLevel(&level);

    initscr();
    printw("Hello World !!!\n");
    printw("User: %s\n", argv[1]);

    while (1) {
        drawMaze(level.board);
        mvprintw(Y_INPUT, X, TAG);
        mvgetstr(Y_INPUT, X + strlen(TAG), input);

        if (validateCommand(input))
            break;

        refresh();
        clearInput();
    }

    endwin();

    return 0;
}