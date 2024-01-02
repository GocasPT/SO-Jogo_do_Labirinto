#include "NamePipe.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../MapManager/MapManager.h"
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
            printf("\nErro ao ler o FIFO do motor\n");
            *(dados->endThread) = 1;
            close(fd);
            unlink(FIFO_MOTOR);
            pthread_exit(NULL);
        } else if (nBytes == 0)
            continue;
        else {
            if (strcmp(cmd.cmd, CMD_CONNECT) == 0) {
                if (*(dados->nUserOn) >= MAX_USER) {
                    printf("%s O servidor esta cheio\n", TAG_MOTOR);
                    continue;
                } else {
                    printf("%s Pedido de conexao de %s [%d]\n", TAG_MOTOR, cmd.arg, cmd.PID);
                    if (addUser(cmd.PID, cmd.arg, dados->userList, dados->nUserOn, dados->playerList, dados->nPlayersOn) == 1)
                        printf("Erro ao adicionar o utilizador\n");  //* PLACE_HOLDER
                }

                printf("%s Enviar nivel a %s [%d]\n", TAG_MOTOR, cmd.arg, cmd.PID);

                char FIFO_USER[MAX];
                sprintf(FIFO_USER, FIFO_JOGOUI, cmd.PID);

                DataRecive data = {
                    .dataType = DATA_LEVEL,
                    .data.level = exportLevel(*(dados->level), dados->playerList, *(dados->nPlayersOn)),  // TODO: adicionar mais merdas
                };

                writeNamePipe(FIFO_USER, data);
            }

            else if (strcmp(cmd.cmd, CMD_DESCONNECT) == 0) {
                printf("%s Pedido de desconexao de %s [%d]\n", TAG_MOTOR, cmd.arg, cmd.PID);
                if (removeUser(cmd.PID, cmd.arg, dados->userList, dados->nUserOn, dados->playerList, dados->nPlayersOn) == 1)
                    printf("Erro ao remover o utilizador\n");  //* PLACE_HOLDER
            }

            // TODO: validar se é um jogador (caso contrario, ignorar o pedido)
            else if (strcmp(cmd.cmd, CMD_MOVE) == 0) {
                printf("%s Pedido de movimento de %s [%d]\n", TAG_MOTOR, cmd.arg, cmd.PID);
                User* user = getUser(dados->userList, *(dados->nUserOn), cmd.PID);
                Player* player = getPlayer(dados->playerList, *(dados->nPlayersOn), user->username);

                if (player == NULL)
                    continue;

                movePlayer(dados->level, player, cmd.arg);

                char FIFO_USER[MAX];
                sprintf(FIFO_USER, FIFO_JOGOUI, cmd.PID);

                DataRecive data = {
                    .dataType = DATA_LEVEL,
                    .data.level = exportLevel(*(dados->level), dados->playerList, *(dados->nPlayersOn)),  // TODO: adicionar mais merdas
                };

                writeNamePipe(FIFO_USER, data);
            }

            else if (strcmp(cmd.cmd, CMD_MSG) == 0) {
                printf("%s Pedido de mensagem de %s [%d]\n", TAG_MOTOR, cmd.arg, cmd.PID);
                /*User* user = getUser(dados->userList, *(dados->nUserOn), cmd.PID);

                char FIFO_USER[MAX];
                sprintf(FIFO_USER, FIFO_JOGOUI, cmd.PID);

                DataRecive data = {
                    .dataType = DATA_FEEDBACK,
                    .data.feedBack = {
                        .user =
                    }
                };

                writeNamePipe(FIFO_USER, data);*/
            }

            else if (strcmp(cmd.cmd, CMD_PLIST) == 0) {
                printf("%s Pedido de lista de jogadores de %s [%d]\n", TAG_MOTOR, cmd.arg, cmd.PID);
            }
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
        printf("Escrito %d bytes no FIFO do servidor\n", nBytes);  // TODO: remover
    }

    close(fd);
}