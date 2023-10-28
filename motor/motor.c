#include "motor.h"

#include <stdio.h>
#include <string.h>

#define TAG_MOTOR "<Motor>"
#define TAG_CMD "<CMD>"

int readLevelMap(char* path, char board[ROWS][COLLUMN]) {
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
    if (readLevelMap("map/level1.txt", motor->borad))
        return -1;

    motor->level = 1;
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
        printf("%s\n", motor->borad[i]);

    printf("\nLevel: %d\n", motor->level);
    printf("Tempo de inscricao: %d\n", motor->timerBegin);
    printf("Tempo de jogo: %d\n", motor->timerGame);
    printf("Tempo de desconto: %d\n", motor->stepTimerGame);
    printf("Numero minimo de usuarios: %d\n\n", motor->nUserMin);

    return 0;
}

int validateCommand(char* input) {
    char cmd[MAX];
    char argv[1][MAX];
    int argc;

    if (argc = sscanf(input, "%s %s", cmd, argv[0])) {
        if (!strcmp(cmd, "help"))
            printf("%s Comando helps:\n- user\n- kick\n- bots\n- bmov\n- rbm\n- begin\n- end\n- test_bot\n", TAG_MOTOR);

        else if (!strcmp(cmd, "users"))
            printf("%s Comando users\n", TAG_MOTOR);

        else if (!strcmp(cmd, "bots"))
            printf("%s Comando bots\n", TAG_MOTOR);

        else if (!strcmp(cmd, "bmov"))
            printf("%s Comando bmov\n", TAG_MOTOR);

        else if (!strcmp(cmd, "rbm"))
            printf("%s Comando rbm\n", TAG_MOTOR);

        else if (!strcmp(cmd, "begin"))
            printf("%s Comando begin\n", TAG_MOTOR);

        else if (!strcmp(cmd, "end")) {
            printf("%s Comando end\n", TAG_MOTOR);
            return -1;
        }

        else if (!strcmp(cmd, "kick")) {
            if (argc != 2)
                printf("%s Comando kick invalido - Falta de arugmentos\n", TAG_MOTOR);
            else
                printf("%s Comando msg\nUser: %s\n", TAG_MOTOR, argv[0]);
        }

        // [META 1]
        else if (!strcmp(cmd, "test_bot"))
            printf("%s Comando test_bot\n", TAG_MOTOR);

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

        if (validateCommand(input) == -1)
            break;
    }

    return 0;
}