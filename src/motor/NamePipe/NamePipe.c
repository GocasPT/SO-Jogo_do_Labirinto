#include "NamePipe.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../UserManager/UserManager.h"
#include "../motor.h"

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
    ThreadData_ReadPipe* dados = (ThreadData_ReadPipe*)lpram;
    CommandToServer cmd;
    int fd;

    // Abrir o fifo do motor
    // TODO: Nota: ele fica à espera de um cliente por definição do FIFO [se tiver 'O_NONBLOCK' nao acontecia]
    // TODO: caso de erro, fechar o processo
    fd = open(FIFO_MOTOR, O_RDONLY);
    if (fd == -1) {
        printf("\nErro ao abrir o FIFO do motor\n");
        *(dados->endThread) = 1;
        close(fd);
        unlink(FIFO_MOTOR);
        pthread_exit(NULL);
    }

    int nBytes;

    while (*(dados->endThread) != 1) {
        nBytes = read(fd, &cmd, sizeof(CommandToServer));
        if (nBytes == -1) {
            printf("Erro ao ler o FIFO do motor\n");
            *(dados->endThread) = 1;
            close(fd);
            unlink(FIFO_MOTOR);
            pthread_exit(NULL);
        } else if (nBytes == 0)
            continue;
        else {
            // TODO: tratamento de dados
            printf("Lido %d bytes do FIFO do motor\n", nBytes);
            printf("[PID: %d] [CMD: %s] [ARG: %s]\n", cmd.PID, cmd.cmd, cmd.arg);

            if (strcmp(cmd.cmd, CMD_CONNECT) == 0) {
                if (*(dados->nUserOn) >= MAX_USER) {
                    printf("O servidor esta cheio\n");
                    continue;
                } else {
                    printf("Pedido de ligacao de %s [%d]\n", cmd.arg, cmd.PID);
                    if (addUser(cmd.PID, cmd.arg, dados->userList, dados->nUserOn) == 0)
                        printf("Utilizador adicionado\n");
                    else
                        printf("Erro ao adicionar o utilizador\n");
                }
            }

            char FIFO_USER[MAX];
            sprintf(FIFO_USER, FIFO_JOGOUI, cmd.PID);

            DataRecive data = {
                .dataType = DATA_LEVEL,
                .data.level = *dados->level,
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

    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o FIFO do servidor\n");
        return;
    }

    int nBytes;

    nBytes = write(fd, &data, sizeof(DataRecive));
    if (nBytes == -1) {
        printf("Erro ao escrever no FIFO do servidor\n");
    } else if (nBytes == 0) {
        printf("Erro ao escrever no FIFO do servidor (nBytes a 0)\n");
    } else {
        printf("Escrito %d bytes no FIFO do servidor\n", nBytes);
    }

    close(fd);

    return;
}