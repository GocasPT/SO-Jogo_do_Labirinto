#include "Consola.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "../NamePipe/NamePipe.h"
#include "../UserManager/UserManager.h"

// TODO: docs
void readConsola(Motor* motor, int* endFlag) {
    char input[MAX];  // String que guarda o input do utilizador
    char* inputResult;

    while (!(*endFlag)) {
        // Imprime o prompt e lê o input do utilizador
        printf("%s ", TAG_CMD);
        inputResult = fgets(input, sizeof(input), stdin);

        if (inputResult == NULL) {
            // TODO: printf("%s Erro ao ler o input\n", TAG_MOTOR);
            break;
        }

        /**
         * Valida o comando
         * Se o comando for "end" retorna -1 para sair do programa
         */
        if (validateCommand(input, motor) == -1)
            break;
    }
}

/**
 * Valida o comando
 * \param input String input do utilizador
 * \param motor Ponteiro da estrutura que guarda o servidor
 */
int validateCommand(char* input, Motor* motor) {
    char cmd[MAX];      // String que guarda o comando
    char argv[1][MAX];  // Vetor de strings que guarda os argumentos do comando
    int argc;           // Numero de argumentos do comando

    argc = sscanf(input, "%s %s", cmd, argv[0]);

    if (argc != 0) {
        if (!strcmp(cmd, "help"))
            printf("%s Comando helps:\n- users\n- kick\n- bots\n- bmov\n- rbm\n- begin\n- end\n- test_bot\n", TAG_MOTOR);

        else if (!strcmp(cmd, "users")) {
            printf("%s Comando users\n", TAG_MOTOR);
            showInfo(*motor, 'u');
        }

        else if (!strcmp(cmd, "kick")) {  // Verifica se o numero de argumentos é valido
            if (argc != 2)
                printf("%s Comando kick invalido - Falta de arugmentos\n", TAG_MOTOR);
            else {
                printf("%s Comando kick\nUser: %s\n", TAG_MOTOR, argv[0]);

                User* user = getUserByUsername(motor->userList, motor->nUserOn, argv[0]);

                if (user == NULL) {
                    printf("\n%s Utilizador nao encontrado\n", TAG_MOTOR);
                    return 0;
                }

                char FIFO_FINAL[MAX];
                sprintf(FIFO_FINAL, FIFO_JOGOUI, user->PID);

                DataRecive data = {
                    .dataType = DATA_FEEDBACK,
                    .data.feedBack = {
                        .feedback = FEEDBACK_CLOSE,
                    },
                };

                writeNamePipe(FIFO_FINAL, data);
                removeUser(user->PID, user->username, motor->userList, motor->nUserOn, motor->playerList, motor->nPlayersOn);
            }
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
            sigqueue(getpid(), SIGINT, (const union sigval)NULL);
            return -1;
        }

        else
            printf("%s Comando inválido\n", TAG_MOTOR);
    }

    return 0;
}

/**
 * Imprime as informações do servidor
 * \param motor Estrutura que guarda o servidor
 * \param type Tipo de informação a imprimir
 */
void showInfo(Motor motor, char type) {
    switch (type) {
        //  Usuários
        case 'u':
            printf("Lista de usuarios:\n");
            for (int i = 0; i < motor.nUserOn; i++)
                printf("User: %s [PID - %d]\n", motor.userList[i].username, motor.userList[i].PID);
            break;

        // Bots
        case 'b':
            printf("Lista de bots (%d bots):\n", motor.nBotOn);
            for (int i = 0; i < motor.nBotOn; i++)
                printf("Bot: %d [PID - %d]\n", i + 1, motor.botList[i]);
            break;

        //  Paredes em movimento
        case 'm':
            printf("Lista de paredes em movimento (%d paredes):\n", motor.nMoveBlockOn);
            for (int i = 0; i < motor.nMoveBlockOn; i++)
                printf("MoveBlock: %d [x: %d, y: %d, dir: %d, step: %d]\n", i, motor.moveBlockList[i].x, motor.moveBlockList[i].y, motor.moveBlockList[i].dir, motor.moveBlockList[i].step);
            break;

        //  Pedras
        case 'r':
            printf("Lista de pedras (%d pedras):\n", motor.nRockOn);
            for (int i = 0; i < motor.nRockOn; i++)
                printf("Rock: %d [x: %d, y: %d, time: %d]\n", i, motor.rockList[i].x, motor.rockList[i].y, motor.rockList[i].time);
            break;

        //  Nível
        case 'l':
            printf("Level: %d\nMapa:\n", motor.level.level);
            for (int i = 0; i < ROWS; i++)
                printf("%s\n", motor.level.board[i]);
            break;

        // Tempo
        case 't':
            printf("Tempo de jogo: %d\n", motor.timerGame);
            printf("Tempo de desconto: %d\n", motor.stepTimerGame);
            break;
    }
}