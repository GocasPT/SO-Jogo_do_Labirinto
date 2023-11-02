#include "motor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAG_MOTOR "<Motor>"
#define TAG_CMD ">>"

void showInfo(Motor motor, char type) {
    switch (type) {
        case 'u':
            printf("Lista de usuarios:\n");
            for (int i = 0; i < motor.nUserOn; i++)
                printf("User: %s [PID - %d]\n", motor.userList[i].username, motor.userList[i].PID);
            break;

        case 'b':
            printf("Lista de bots (%d bots):\n", motor.nBotOn);
            for (int i = 0; i < motor.nBotOn; i++)
                printf("Bot: %d [PID - %d]\n", i + 1, motor.botList[i].PID);
            break;

        case 'm':
            printf("Lista de paredes em movimento (%d paredes):\n", motor.nMoveBlockOn);
            for (int i = 0; i < motor.nMoveBlockOn; i++)
                printf("MoveBlock: %d [x: %d, y: %d, dir: %d, step: %d]\n", i, motor.moveBlockList[i].x, motor.moveBlockList[i].y, motor.moveBlockList[i].dir, motor.moveBlockList[i].step);
            break;

        case 'r':
            printf("Lista de pedras (%d pedras):\n", motor.nRockOn);
            for (int i = 0; i < motor.nRockOn; i++)
                printf("Rock: %d [x: %d, y: %d, time: %d]\n", i, motor.rockList[i].x, motor.rockList[i].y, motor.rockList[i].time);
            break;

        case 'l':
            printf("Level: %d\n", motor.level.level);
            break;

        case 't':
            printf("Tempo de jogo: %d\n", motor.timerGame);
            printf("Tempo de desconto: %d\n", motor.stepTimerGame);
            break;
    }
}

int execBot(Motor* motor) {
    printf("Executando bot\n\n");

    int pid;
    int fd[2];

    if (pipe(fd) == -1) {
        printf("%s Erro ao criar o pipe\n", TAG_MOTOR);
        return -1;
    }

    printf("%s A executar os bots\n", TAG_MOTOR);

    for (int i = 0; i < motor->level.level + 1; i++) {
        motor->botList[motor->nBotOn].PID = fork();
        pid = motor->botList[motor->nBotOn].PID;

        if (pid < 0) {
            printf("%s Erro ao criar o processo filho\n", TAG_MOTOR);
            return -1;
        }

        // Filho
        else if (pid == 0) {
            close(1);

            if (dup(fd[1]) == -1) {
                fprintf(stderr, "%s Erro ao redirecionar o stdout\n", TAG_MOTOR);
                return -1;
            }

            close(fd[0]);
            close(fd[1]);

            char interval[3], duration[3];
            sprintf(interval, "%d", 30 - motor->nBotOn * 5);
            sprintf(duration, "%d", 10 - motor->nBotOn * 5);

            if (execl("./bot/bot", "./bot/bot", interval, duration, NULL) == -1) {
                fprintf(stderr, "%s [FILHO] Erro ao executar o bot\n", TAG_MOTOR);
                close(fd[0]);
                close(fd[1]);
                motor->nBotOn--;
                return -1;
            }
        }

        // Pai
        else
            motor->nBotOn++;
    }

    close(fd[1]);

    char buffer[10];

    showInfo(*motor, 'b');
    printf("%s A ler o pipe:\n", TAG_MOTOR);

    // TODO [?]: Uma para sair do loop (sinal ou algo do género)
    while (1) {
        if (read(fd[0], buffer, sizeof(buffer)) == -1) {
            printf("\n%s Erro ao ler o pipe\n", TAG_MOTOR);
            return 1;
        }

        buffer[strlen(buffer) - 1] = '\0';

        int x, y, duration;
        sscanf(buffer, "%d %d %d", &x, &y, &duration);

        printf("%s RECEBI: %d %d %d\n", TAG_MOTOR, x, y, duration);
    }

    return 0;
}

// TODO [Meta 2]: Receber o nivel para saber qual ficheiro ler (em ves de level1.txt)
int readLevelMap(char* path, char board[ROWS][COLLUMN]) {
    // TODO [after makefile]: Mudar o caminho quando o excutavel mudar o local de execução
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        printf("%s Erro ao abrir o arquivo %s\n", TAG_MOTOR, path);
        return -1;
    }

    char endLine[2];  // Para ler o \n do final da linha

    for (int i = 0; i < ROWS; i++) {
        // Lê uma linha do arquivo e coloca na board
        if (fgets(board[i], COLLUMN, file) == NULL) {
            printf("%s Erro ao ler o arquivo %s\n", TAG_MOTOR, path);
            return -1;
        }

        // Lê o \n do final da linha e descarta
        if (fgets(endLine, 2, file) == NULL) {
            printf("%s Erro ao ler o arquivo %s\n", TAG_MOTOR, path);
            return -1;
        }
    }

    fclose(file);

    return 0;
}

int configServer(Motor* motor) {
    // TODO [META 2]: "Mandar" o ficheiro → Mandar o caminha para a pasta com os mapas
    if (readLevelMap("map/level1.txt", motor->level.board) == -1)
        return -1;

    // TODO [META 2]: Por os valores corretos (variáveis de ambiente)
    motor->level.level = 1;
    motor->timerBegin = 0;     // ← INSCRICAO
    motor->timerGame = 0;      // ← DURACAO
    motor->stepTimerGame = 0;  // ← DECREMENTO
    motor->nUserMin = 1;       // ← NPLAYERS
    motor->nUserOn = 0;
    motor->nBotOn = 0;
    motor->nRockOn = 0;
    motor->nMoveBlockOn = 0;

    printf("%s Configuração do servidor:\n", TAG_MOTOR);

    printf("Mapa:\n");
    for (int i = 0; i < ROWS; i++)
        printf("%s\n", motor->level.board[i]);
    printf("\nLevel: %d\n", motor->level.level);
    printf("Tempo de inscricao: %d\n", motor->timerBegin);
    printf("Tempo de jogo: %d\n", motor->timerGame);
    printf("Tempo de desconto: %d\n", motor->stepTimerGame);
    printf("Numero minimo de usuarios: %d\n\n", motor->nUserMin);

    return 0;
}

int validateCommand(char* input, Motor* motor) {
    char cmd[MAX];
    char argv[1][MAX];
    int argc;

    // TODO [DUVIDA]: argv[1] recebe lixo
    if (argc = sscanf(input, "%s %s", cmd, argv[0])) {
        if (!strcmp(cmd, "help"))
            printf("%s Comando helps:\n- users\n- kick\n- bots\n- bmov\n- rbm\n- begin\n- end\n- test_bot\n", TAG_MOTOR);

        else if (!strcmp(cmd, "users")) {
            printf("%s Comando users\n", TAG_MOTOR);
            showInfo(*motor, 'u');
        }

        else if (!strcmp(cmd, "kick")) {
            if (argc != 2)
                printf("%s Comando kick invalido - Falta de arugmentos\n", TAG_MOTOR);
            else
                printf("%s Comando msg\nUser: %s\n", TAG_MOTOR, argv[0]);
        }

        else if (!strcmp(cmd, "bots")) {
            printf("%s Comando bots\n", TAG_MOTOR);
            showInfo(*motor, 'b');
        }

        else if (!strcmp(cmd, "bmov")) {
            printf("%s Comando bmov\n", TAG_MOTOR);
            showInfo(*motor, 'm');
        }

        else if (!strcmp(cmd, "rbm")) {
            printf("%s Comando rbm\n", TAG_MOTOR);
            showInfo(*motor, 'r');
        }

        else if (!strcmp(cmd, "begin"))
            printf("%s Comando begin\n", TAG_MOTOR);

        else if (!strcmp(cmd, "end")) {
            printf("%s Comando end\n", TAG_MOTOR);
            return -1;
        }

        //* PLACEHOLDER [META 1]
        else if (!strcmp(cmd, "test_bot")) {
            printf("%s Comando test_bot\n", TAG_MOTOR);
            if (execBot(motor) == -1) {
                printf("%s Erro ao executar o bot\n", TAG_MOTOR);
                return -1;
            }
        }

        else
            printf("%s Comando inválido\n", TAG_MOTOR);
    }

    return 0;
}

int main() {
    printf("Programa: motor\n\n");

    Motor servidor;
    char input[MAX];

    if (configServer(&servidor) == -1)
        return -1;

    while (1) {
        printf("%s ", TAG_CMD);
        fgets(input, sizeof(input), stdin);

        if (validateCommand(input, &servidor) == -1)
            break;
    }

    return 0;
}