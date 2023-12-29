#include "MapManager.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// TODO [Meta 2]: Receber o nivel para saber qual ficheiro ler (em ves de level1.txt)
/**
 * Lê o mapa do ficheiro e coloca no tabuleiro
 * \param path Caminho do ficheiro
 * \param board Tabuleiro
 */
int readLevelMap(char* path, char board[ROWS][COLLUMN]) {
    // Abre o ficheiro
    FILE* file = fopen(path, "r");

    // Verifica se o ficheiro foi aberto corretamente
    if (file == NULL) {
        printf("%s Erro ao abrir o arquivo %s\n", TAG_MOTOR, path);
        return -1;
    }

    char endLine[2];  // Para ler o \n do final da linha

    // Lê o ficheiro linha a linha
    for (int i = 0; i < ROWS; i++) {
        /**
         * Lê uma linha do arquivo e coloca na board
         * Se retornar NULL, houve um erro ao ler o ficheiro
         */
        if (fgets(board[i], COLLUMN, file) == NULL) {
            printf("%s Erro ao ler o arquivo %s\n", TAG_MOTOR, path);
            return -1;
        }

        /**
         * Lê o \n do final da linha e descarta
         * Se retornar NULL, houve um erro ao ler o ficheiro
         */
        if (fgets(endLine, 2, file) == NULL) {
            printf("%s Erro ao ler o arquivo %s\n", TAG_MOTOR, path);
            return -1;
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
        exportLevel.board[playerList[i].x][playerList[i].y] = 'P';

    printf("Mapa:\n");
    for (int i = 0; i < ROWS; i++)
        printf("%s\n", exportLevel.board[i]);

    return exportLevel;
}

/**
 * Executa o bot
 * \param motor Ponteiro da estrutura que guarda o servidor
 */
int execBot(Motor* motor) {
    printf("Executando bot\n\n");

    // TODO: 1 pipe → array de pipes (dinamico?)
    int pid;    // PID do processo filho
    int fd[2];  // Pipe

    /**
     * Cria o pipe
     * Se retornar -1, houve um erro ao criar o pipe
     */
    if (pipe(fd) == -1) {
        printf("%s Erro ao criar o pipe\n", TAG_MOTOR);
        return -1;
    }

    printf("%s A executar os bots\n", TAG_MOTOR);

    /**
     * Cria os processos filhos
     * O número de processos filhos é igual ao nível + 1
     * Nível 1 → 2 bots
     * Nível 2 → 3 bots
     * Nível 3 → 4 bots
     */
    // for (int i = 0; i < motor->level.level +1; i++) {
    for (int i = 0; i < 1; i++) {
        motor->botList[motor->nBotOn] = fork();  // Cria o processo filho
        pid = motor->botList[motor->nBotOn];     // Guarda o PID do processo filho

        // Erro ao criar o processo filho
        if (pid < 0) {
            printf("%s Erro ao criar o processo filho\n", TAG_MOTOR);
            return -1;
        }

        // Filho
        else if (pid == 0) {
            close(1);  // Fecha o stdout

            /**
             * Redireciona o stdout para o pipe
             * Se retornar -1, houve um erro ao redirecionar o stdout
             */
            if (dup(fd[1]) == -1) {
                fprintf(stderr, "%s Erro ao redirecionar o stdout\n", TAG_MOTOR);
                return -1;
            }

            // Fecha o pipe
            close(fd[0]);
            close(fd[1]);

            /**
             * Define os valores do intervalo e duração (depende do número de bots)
             * O intervalo é de 30 a 10 segundos
             * A duração é de 10 a 0 segundos
             */
            char interval[3], duration[3];
            sprintf(interval, "%d", 30 - motor->nBotOn * 5);
            sprintf(duration, "%d", 10 - motor->nBotOn * 5);

            /**
             * Executa o bot
             * Se retornar -1, houve um erro ao executar o bot
             */
            if (execl("./bot/bot", "./bot/bot", interval, duration, NULL) == -1) {
                fprintf(stderr, "%s [FILHO] Erro ao executar o bot\n", TAG_MOTOR);

                // Fecha o pipe
                close(fd[0]);
                close(fd[1]);
                motor->nBotOn--;  // Decrementa o número de bots
                return -1;
            }
        }

        // Pai
        else
            // Incrementa o número de bots
            motor->nBotOn++;
    }

    // Fecha o pipe
    close(fd[1]);

    // String que guarda o input do pipe
    char buffer[10];

    // Mostra as informações dos bots
    showInfo(*motor, 'b');
    printf("%s A ler o pipe:\n", TAG_MOTOR);

    // Loop infinito para ler o pipe
    // TODO: thread + select
    while (1) {
        /**
         * Lê o pipe
         * Se retornar -1, houve um erro ao ler o pipe
         */
        if (read(fd[0], buffer, sizeof(buffer)) == -1) {
            printf("\n%s Erro ao ler o pipe\n", TAG_MOTOR);
            return 1;
        }

        // Posiciona o \0 no final da string
        buffer[strlen(buffer) - 1] = '\0';

        // Define os valores do x, y e duração lidos do pipe (buffer)
        int x, y, duration;
        sscanf(buffer, "%d %d %d", &x, &y, &duration);

        printf("%s RECEBI: %d %d %d\n", TAG_MOTOR, x, y, duration);
    }

    return 0;
}

// TODO: docs
void movePlayer(char board[ROWS][COLLUMN], Player* list, int listSize, char* username, char direction) {
    Player* player;

    for (int i = 0; i < listSize; i++) {
        if (strcmp(list[i].username, username) == 0) {
            player = &list[i];
            break;
        }
    }

    if (player == NULL)
        return;

    int x = player->x;
    int y = player->y;

    if (strcmp(direction, ARG_DOWN)) {
        if (board[x + 1][y] == ' ')
            player->x++;
    }

    else if (strcmp(direction, ARG_UP)) {
        if (board[x - 1][y] == ' ')
            player->x--;
    }

    else if (strcmp(direction, ARG_LEFT)) {
        if (board[x][y - 1] == ' ')
            player->y--;
    }

    else if (strcmp(direction, ARG_RIGHT)) {
        if (board[x][y + 1] == ' ')
            player->y++;
    }
}