#include "jogoUI.h"

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GlobalData gD;

/**
 * Função que veririca se o motor está a correr
 */
void checkMotorOpen() {
    int fd = open(FIFO_MOTOR, O_WRONLY);
    if (fd == -1) {
        printf("\n\tO motor não está a correr\n");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

void signalHandler() {
    endwin();
    closeNamePipe(gD.fifo->FIFO_NAME, &gD.fifo->playerFD, &gD.fifo->serverFD);
    pthread_kill(*gD.thread, SIGINT);
    pthread_join(*gD.thread, NULL);
    exit(EXIT_SUCCESS);
}

/**
 * Função que configuras as threads
 * \param jogoUI Ponteiro da estrutura principal do jogoUI
 */
void configThreads(JogoUI* jogoUI) {
    ThreadData threadData;
    threadData.level = &jogoUI->level;
    threadData.fifo = &jogoUI->fifo;

    // Thread de leitura dos name pipes (servidor + player)
    if (pthread_create(&jogoUI->threadFD, NULL, readNamePipe, &threadData)) {
        // printf("\n\tErro ao criar a thread de leitura dos name pipes\n");
        wprintw(jogoUI->ui.notification, "Erro ao criar a thread de leitura dos name pipes\n");
        closeNamePipe(jogoUI->fifo.FIFO_NAME, &jogoUI->fifo.playerFD, &jogoUI->fifo.serverFD);
        exit(EXIT_FAILURE);
    }

    wprintw(jogoUI->ui.notification, "Thread 'readNamePipe' criada\n");
    wrefresh(jogoUI->ui.notification);
}

int main(int argc, char* argv[]) {
    // Verifica se o nome do jogador foi passado
    if (argc != 2) {
        printf("Falta o teu nome\n");
        return -1;
    }

    // Verificar se o motor está a correr
    // TODO: descomentar
    // checkMotorOpen();

    // Configura o signal handler
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sa.sa_flags = SIGINT;
    sigemptyset(&sa.sa_mask);

    // Inicializa a estrutura de dados do jogoUI
    JogoUI jogoUI;
    strcpy(jogoUI.user.username, argv[1]);
    jogoUI.user.PID = getpid();
    gD.thread = &jogoUI.threadFD;
    gD.fifo = &jogoUI.fifo;

    // Inicializa o ncurses e configura as janelas
    initscr();
    configUI(&jogoUI.ui);  // Configura as janelas

    // Inicializa os name pipes
    sprintf(jogoUI.fifo.FIFO_NAME, FIFO_JOGOUI, getpid());
    createNamePipe(jogoUI.fifo.FIFO_NAME);

    // Inicializa as threads
    configThreads(&jogoUI);

    // Loop principal do jogoUI
    readInput(&jogoUI.ui, &jogoUI.level);

    // Fecha o ncurses
    endwin();

    // Espera que a thread termine
    pthread_join(jogoUI.threadFD, NULL);

    // Fecha os name pipes
    closeNamePipe(jogoUI.fifo.FIFO_NAME, &jogoUI.fifo.playerFD, &jogoUI.fifo.serverFD);

    return 0;
}