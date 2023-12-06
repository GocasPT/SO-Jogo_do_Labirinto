#include "jogoUI.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int configThreads(JogoUI* JogoUI) {
}

int main(int argc, char* argv[]) {
    // Verifica se o nome do jogador foi passado
    if (argc != 2) {
        printf("Falta o teu nome\n");
        return -1;
    }

    // Inicializa a estrutura de dados do jogoUI
    JogoUI jogoUI;
    strcpy(jogoUI.user.username, argv[1]);
    jogoUI.user.PID = getpid();

    // Inicializa as threads
    configThreads(&jogoUI);

    // pthread_join();

    readInput(&jogoUI);

    return 0;
}