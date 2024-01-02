#include "MapManager.h"

#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

// TODO [Meta 2]: Receber o nivel para saber qual ficheiro ler (em ves de level1.txt)
int readLevelMap(char* path, char board[ROWS][COLLUMN]) {
    // Abre o ficheiro
    FILE* file = fopen(path, "r");

    // Verifica se o ficheiro foi aberto corretamente
    if (file == NULL) {
        printf("%s Erro ao abrir o arquivo %s\n", TAG_MOTOR, path);
        return -1;
    }

    // Lê o ficheiro linha a linha
    for (int i = 0; i < ROWS; i++) {
        /**
         * Lê uma linha do arquivo e coloca na board
         * Se retornar NULL, houve um erro ao ler o ficheiro
         */
        if (fgets(board[i], COLLUMN + 1, file) == NULL) {
            printf("%s Erro ao ler o arquivo %s\n", TAG_MOTOR, path);
            return -1;
        }

        /**
         * Lê o \n do final da linha e descarta
         * Se retornar NULL, houve um erro ao ler o ficheiro
         */
        // Replace the newline character with a null character
        char* newline = strchr(board[i], '\n');
        if (newline) {
            *newline = '\0';
        }
    }

    // Fecha o ficheiro
    fclose(file);

    return 0;
}

// TODO: docs
int loadLevelMap(char board[ROWS][COLLUMN]) {
}

// TODO: exportLevel (complete)
// TODO: docs
Level exportLevel(Level level, Player* playerList, int playerListSize /*, Rock* rockList, int rockListSize, MoveBlock* moveBlockList, int moveBlockListSize*/) {
    Level exportLevel = level;

    for (int i = 0; i < playerListSize; i++)
        exportLevel.board[playerList[i].y][playerList[i].x] = 'P';

    return exportLevel;
}

/**
 * Executa o bot
 * \param motor Ponteiro da estrutura que guarda o servidor
 */
void execBot(int nBots, int fd[][2], int* botList) {
    printf("\n%s A executar %d bots\n", TAG_MOTOR, nBots);

    int pid;  // PID do processo filho

    for (int i = 0; i < nBots; i++) {
        if (pipe(fd[i]) == -1) {
            printf("\n%s Erro ao criar o pipe anonimos\n", TAG_MOTOR);
            sigqueue(getpid(), SIGINT, (const union sigval)NULL);
        }

        botList[i] = fork();  // Cria o processo filho
        pid = botList[i];     // Guarda o PID do processo filho

        // Erro
        if (pid < 0) {
            printf("\n%s Erro ao criar o processo filho\n", TAG_MOTOR);
            sigqueue(getpid(), SIGINT, (const union sigval)NULL);
            return;
        }

        // Filho
        else if (pid == 0) {
            close(1);  // Fecha o stdout

            // stdout → pipe anonimo (escrita)
            if (dup(fd[i][1]) == -1) {
                fprintf(stderr, "\n%s Erro ao redirecionar o stdout\n", TAG_MOTOR);
                sigqueue(getpid(), SIGINT, (const union sigval)NULL);
                return;
            }

            close(fd[i][0]);  // Fecha o stdin
            close(fd[i][1]);  // Fecha o pipe anonimo (escrita)

            char interval[3], duration[3];
            sprintf(interval, "%d", 30 - i * 5);
            sprintf(duration, "%d", 10 - i * 5);

            if (execl("./bot", "./bot", interval, duration, NULL) == -1) {
                fprintf(stderr, "\n%s [FILHO] Erro ao executar o bot\n", TAG_MOTOR);

                close(fd[i][0]);
                close(fd[i][1]);
                exit(-1);
            }

            // Pai
        } else {
            close(fd[i][1]);
        }
    }
}

void* readBots(void* arg) {
    Motor* motor = (Motor*)arg;
    int fd[MAX_BOT + 1][2];
    int result;

    // TODO: endflag
    while (*(motor->endFlag) != 1) {
        int nBots = motor->level.level + 1;

        execBot(nBots, fd, motor->botList);
        motor->nBotOn = nBots;

        fd_set readFds;
        FD_ZERO(&readFds);

        for (int i = 0; i < nBots; i++)
            FD_SET(fd[i][0], &readFds);

        // TODO: change level
        while (motor->timerGame > 0 && *(motor->endFlag) != 1) {
            result = select(fd[nBots - 1][0] + 1, &readFds, NULL, NULL, NULL);
            if (result == -1) {
                printf("\n%s Erro ao executar o select\n", TAG_MOTOR);
                sigqueue(getpid(), SIGINT, (const union sigval)NULL);
                continue;
                ;
            } else if (result == 0)
                continue;
            else {
                for (int i = 0; i < nBots; i++)
                    if (FD_ISSET(fd[i][0], &readFds)) {
                        char buffer[MAX];
                        int nBytes = read(fd[i][0], buffer, sizeof(buffer));
                        if (nBytes == -1) {
                            printf("\n%s Erro ao ler o pipe anonimo\n", TAG_MOTOR);
                            sigqueue(getpid(), SIGINT, (const union sigval)NULL);
                            continue;
                        } else if (nBytes == 0)
                            continue;
                        else {
                            printf("\n%s Bot: %s\n", TAG_MOTOR, buffer);
                            // TODO: addRock();
                        }
                    }
            }
        }

        printf("\n%s A terminar os bots\n", TAG_MOTOR);
        for (int i = 0; i < nBots; i++) {
            printf("Bot: %d [PID - %d]\n", i + 1, motor->botList[i]);
            close(fd[i][0]);
            close(fd[i][1]);
            sigqueue(motor->botList[i], SIGINT, (const union sigval)NULL);
            waitpid(motor->botList[i], NULL, 0);
        }
        nBots = 0;
        motor->nBotOn = nBots;
    }

    pthread_exit(NULL);
}

// TODO: docs
void movePlayer(char board[ROWS][COLLUMN], Player* player, char* direction) {
    int x = player->x;
    int y = player->y;

    if (strcmp(direction, ARG_DOWN) == 0) {
        if (board[y + 1][x] == ' ')
            player->y++;
    }

    else if (strcmp(direction, ARG_UP) == 0) {
        if (board[y - 1][x] == ' ')
            player->y--;
    }

    else if (strcmp(direction, ARG_LEFT) == 0) {
        if (board[y][x - 1] == ' ')
            player->x--;
    }

    else if (strcmp(direction, ARG_RIGHT) == 0) {
        if (board[y][x + 1] == ' ')
            player->x++;
    }

    printf("%s %s moveu-se para %d %d\n", TAG_MOTOR, player->username, player->x, player->y);
}

void addRock(Rock* list, int* listSize, int x, int y, int time) {
    // TODO: validar coor
    Rock newRock = {
        .x = x,
        .y = y,
        .time = time,
    };

    list[*listSize] = newRock;
    (*listSize)++;
}