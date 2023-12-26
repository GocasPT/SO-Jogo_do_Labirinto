#include "NamePipe.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

int createNamePipe() {
    int result;

    result = mkfifo(FIFO_MOTOR, 0666);
    if (result == -1) {
        printf("Erro ao criar o FIFO do motor\n");
        return -1;
    }

    return 0;
}

// TODO: docs
void* readNamePipe(void* lpram) {
    ThreadData* dados = (ThreadData*)lpram;
    CommandToServer cmd;
    int fd;

    // Abrir os fifos
    // TODO: caso de erro, fechar o processo
    fd = open(FIFO_MOTOR, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o FIFO do motor\n");
        dados->endThread = 1;
        close(fd);
        unlink(FIFO_MOTOR);
        pthread_exit(NULL);
    }

    int nBytes;

    while (dados->endThread != 1) {
        nBytes = read(fd, &cmd, sizeof(CommandToServer));
        if (nBytes == -1) {
            printf("Erro ao ler o FIFO do motor\n");
            dados->endThread = 1;
            close(fd);
            unlink(FIFO_MOTOR);
            pthread_exit(NULL);
        } else if (nBytes == 0)
            continue;
        else {
            // TODO: tratamento de dados
            printf("Lido %d bytes do FIFO do motor\n", nBytes);
            printf("[PID: %d] [CMD: %s] [ARG: %s]\n", cmd.PID, cmd.cmd, cmd.arg);

            char FIFO_USER[MAX];
            sprintf(FIFO_USER, FIFO_JOGOUI, cmd.PID);

            DataRecive data = {
                DATA_LEVEL,
                dados->level,
            };

            writeNamePipe(FIFO_USER, data);
        }
    }

    close(fd);

    pthread_exit(NULL);
}

// TODO: docs
void writeNamePipe(char FIFO_NAME[], DataRecive data) {
    int fd;

    /*fd = open(FIFO_NAME, O_WRONLY);
    // TODO: checkar erro
    if (fd == -1) {
        printf("Erro ao abrir o FIFO do servidor\n");
        return;
    }

    int nBytes;

    nBytes = write(fd, &cmd, sizeof(CommandToServer));
    if (nBytes == -1) {
        printf("Erro ao escrever no FIFO do servidor\n");
    } else if (nBytes == 0) {
        printf("Erro ao escrever no FIFO do servidor (nBytes a 0)\n");
    } else {
        printf("Escrito %d bytes no FIFO do servidor\n", nBytes);
    }

    close(fd);*/

    return;
}