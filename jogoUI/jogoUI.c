#include "jogoUI.h"

#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define TAG ">> "

#define X 0
#define Y_INPUT 6
#define Y_NOTIFICATION Y_INPUT - 2

// TODO: verificar se isto funcionar como deve ser
void clearInput() {
    move(Y_INPUT, 0);
    clrtoeol();
}

// Função Extra para escrever as mensagems mais bonito
void writeNotification(const char* msg, ...) {
    va_list args;
    va_start(args, msg);

    char buffer[MAX];
    vsnprintf(buffer, sizeof(buffer), msg, args);

    mvprintw(Y_NOTIFICATION, X, "%s", buffer);

    va_end(args);
}

int validateCommand(char* input) {
    char cmd[MAX];
    char argv[2][MAX];
    int argc;

    // TODO: argv[1] recebe lixo
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

int main(int argc, char* argv[]) {
    printf("Programa: jogoUI\n");

    if (argc != 2) {
        printf("Falta o teu nome\n");
        return -1;
    }

    char input[MAX];

    initscr();
    printw("Hello World !!!\n");
    printw("User: %s\n", argv[1]);

    while (1) {
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