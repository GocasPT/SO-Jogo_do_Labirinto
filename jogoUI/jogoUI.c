#include "jogoUI.h"

#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int endFlag;  // TODO: docs

// TODO: docs
void signalHandler(int sig, siginfo_t* info, void* context) {
    endFlag = 1;
}

/**
 * Função que veririca se o motor está a correr
 */
int checkMotorOpen() {
    int fd;
    fd = open(FIFO_MOTOR, O_WRONLY);
    if (fd == -1) {
        printf("\n%s O motor nao esta a correr\n", PRINT_TAG);
        return -1;
    }
    close(fd);

    return 0;
}

// TODO: mudar para int (melhor sintaxe)
/**
 * Função que configuras as threads
 * \param jogoUI Ponteiro da estrutura principal do jogoUI
 */
void configThreads(JogoUI* jogoUI, ThreadData* threadData) {
    int result;

    // Thread de leitura dos name pipes (servidor e player)
    result = pthread_create(&jogoUI->threadReadPipe, NULL, readNamePipe, threadData);
    if (result != 0) {
        wprintw(jogoUI->ui.notification, "Erro ao criar a thread de leitura dos name pipes\n");
        unlink(threadData->FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    wprintw(jogoUI->ui.notification, "Thread 'readNamePipe' criada\n");
    wrefresh(jogoUI->ui.notification);
}

int main(int argc, char* argv[]) {
    // Verifica se o username foi passado
    if (argc != 2) {
        printf("Falta o teu username\n");
        return -1;
    }

    // Verificar se o motor está a correr
    if (checkMotorOpen() == -1)
        return -1;

    // Inicializa a estrutura de dados do jogoUI
    JogoUI jogoUI;
    strcpy(jogoUI.user.username, argv[1]);
    jogoUI.user.PID = getpid();

    // Configura o signal handler
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    // Inicializa o ncurses e configura as janelas
    initscr();
    configUI(&jogoUI.ui);

    // Inicializa os name pipes
    sprintf(jogoUI.FIFO_NAME, FIFO_JOGOUI, getpid());
    createNamePipe(jogoUI.FIFO_NAME);  // TODO: if

    // Inicializa a estrutura de dados para a thread de leitura
    ThreadData threadData;
    threadData.endThread = &endFlag;
    strcpy(threadData.FIFO_NAME, jogoUI.FIFO_NAME);
    threadData.level = &jogoUI.level;
    threadData.ui = &jogoUI.ui;
    endFlag = 0;

    // Inicializa as threads
    configThreads(&jogoUI, &threadData);

    // Inicializa o level
    CommandToServer connectCmd = {
        getpid(),
        CMD_CONNECT,
    };
    strcpy(connectCmd.arg, jogoUI.user.username);
    writeNamePipe(FIFO_MOTOR, connectCmd);

    // Loop principal do jogoUI
    readInput(&jogoUI.ui, &jogoUI.level);

    // TODO: docs
    endFlag = 1;

    // Espera que a thread termine
    pthread_join(jogoUI.threadReadPipe, NULL);

    // TODO: docs
    unlink(jogoUI.FIFO_NAME);

    // Fecha o ncurses
    endwin();

    return 0;
}