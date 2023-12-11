#include "NamePipe.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>

int createNamePipe(char FIFO_NAME[]) {
    int result;

    result = mkfifo(FIFO_NAME, 0666);
    if (result == -1) {
        printf("Erro ao criar o FIFO do jogador\n");
        return -1;
    }

    return 0;
}

int closeNamePipe(char FIFO_NAME[], int* playerFD, int* serverFD) {
    int result;

    // TODO: sair do programa caso de erro
    result = close(*playerFD);
    if (result == -1) {
        printf("Erro ao fechar o FIFO do jogador\n");
        return -1;
    }

    // TODO: descomentar
    /*result = close(*serverFD);
    if (result == -1) {
        printf("Erro ao fechar o FIFO do servidor\n");
        return -1;
    }*/

    // TODO: mudar o nome do FIFO com o PID do jogador
    unlink(FIFO_NAME);

    return 0;
}

void* readNamePipe(void* lpram) {
    ThreadData* dados = (ThreadData*)lpram;

    // Abrir os fifos
    // TODO: caso de erro, fechar o processo
    // TODO: descomentar
    /*dados->fifo->playerFD = open(dados->fifo->FIFO_NAME, O_RDONLY);
    if (dados->fifo->playerFD == -1) {
        printf("\n\tErro ao abrir o FIFO do jogador\n");
        closeNamePipe(dados->fifo->FIFO_NAME, &dados->fifo->playerFD, &dados->fifo->serverFD);
        // exit(EXIT_FAILURE);
    }*/

    // TODO: descomentar + testar
    /*dados->serverFD = open(FIFO_MOTOR, O_WRONLY);
    if (dados->serverFD == -1) {
        printf("\n\tErro ao abrir o FIFO do servidor\n");
        closeNamePipe(&dados->playerFD, &dados->serverFD);
        // exit(EXIT_FAILURE);
    }*/

    // Ler as informações do name pipe
    // TODO: logica de leitura do name pipe

    pthread_exit(NULL);
}

// TODO: verificar se a escritar tá a ser feita corretamente e caso de erro, fechar o processo
void writeNamePipe(NamePipe* fifo, CommandToServer* cmd) {
    int result;
    result = write(fifo->serverFD, cmd, sizeof(CommandToServer));
    if (result == -1) {
        printf("\n\tErro ao escrever no FIFO\n");
        closeNamePipe(fifo->FIFO_NAME, &fifo->playerFD, &fifo->serverFD);
        // exit(EXIT_FAILURE);
    }
}