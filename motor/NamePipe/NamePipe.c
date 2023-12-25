#include "NamePipe.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

int createNamePipe(char FIFO_NAME[]) {
    int result;

    result = mkfifo(FIFO_NAME, 0666);
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
    fd = open(dados->FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        // wprintw(dados->ui->notification, "Erro ao abrir o FIFO do jogador\n");
        printf("Erro ao abrir o FIFO do motor\n");
        dados->endThread = 1;
        close(fd);
        unlink(dados->FIFO_NAME);
        pthread_exit(NULL);
    }

    int nBytes;

    while (dados->endThread != 1) {
        // nBytes = read(fd, dados->data, sizeof(DataRecive));
        nBytes = read(fd, &cmd, sizeof(CommandToServer));
        if (nBytes == -1) {
            // wprintw(dados->ui->notification, "Erro ao ler o FIFO do jogador\n");
            printf("Erro ao ler o FIFO do motor\n");
            dados->endThread = 1;
            close(fd);
            unlink(dados->FIFO_NAME);
            pthread_exit(NULL);
        } else if (nBytes == 0)
            continue;
        else {
            // TODO: tratamento de dados
            // wprintw(dados->ui->notification, "Lido %d bytes do FIFO do jogador\n", nBytes);
            printf("Lido %d bytes do FIFO do motor\n", nBytes);
        }
    }

    close(fd);

    pthread_exit(NULL);
}

// TODO: docs
void writeNamePipe(char FIFO_NAME[], CommandToServer cmd) {
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