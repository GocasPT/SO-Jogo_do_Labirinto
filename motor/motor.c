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
            printf("Lista de bots:\n");
            for (int i = 0; i < motor.nBotOn; i++)
                printf("Bot: %d [PID - %d]\n", i, motor.botList[i].PID);
            break;

        case 'm':
            printf("Lista de paredes em movimento:\n");
            for (int i = 0; i < motor.nMoveBlockOn; i++)
                printf("MoveBlock: %d [x: %d, y: %d, dir: %d, step: %d]\n", i, motor.moveBlockList[i].x, motor.moveBlockList[i].y, motor.moveBlockList[i].dir, motor.moveBlockList[i].step);
            break;

        case 'r':
            printf("Lista de pedras:\n");
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
    pipe(fd);

    motor->botList[motor->nBotOn].PID = fork();
    pid = motor->botList[motor->nBotOn].PID;
    motor->nBotOn++;

    if (pid < 0) {
        motor->nBotOn--;
        return -1;
    }

    // Filho
    else if (pid == 0) {
        close(1);
        dup(fd[1]);
        close(fd[0]);
        close(fd[1]);

        if (execl("./bot/bot", "./bot/bot", NULL)) {
            printf("%s [FILHO] Erro ao executar o bot\n", TAG_MOTOR);
            return 1;
        }
    }

    // Pai
    else {
        // TODO: Meta 2 - loop de leitura do pipe
        close(fd[1]);
        char buffer[MAX];

        showInfo(*motor, 'b');

        wait(&pid);

        if (read(fd[0], buffer, sizeof(buffer)) == -1) {
            printf("\n%s [PAI] Erro ao ler o pipe\n", TAG_MOTOR);
            return 1;
        }

        printf("\n%s [PAI] - Recebi:\n%s\n", TAG_MOTOR, buffer);
        motor->nBotOn--;
    }

    return 0;
}

int readLevelMap(char* path, char board[ROWS][COLLUMN]) {
    // TODO: Meta 2 - receber o nivel para saber qual ficheiro ler (em ves de level1.txt)
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
    if (readLevelMap("map/level1.txt", motor->level.board) == -1)
        return -1;

    motor->level.level = 1;
    motor->timerBegin = 0;
    motor->timerGame = 0;
    motor->stepTimerGame = 0;
    motor->nUserMin = 1;
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

        // [META 1]
        else if (!strcmp(cmd, "test_bot")) {
            printf("%s Comando test_bot\n", TAG_MOTOR);
            if (execBot(motor) == -1)
                printf("%s Erro ao executar o bot\n", TAG_MOTOR);
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