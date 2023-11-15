#include "motor.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAG_MOTOR "<Motor>"
#define TAG_CMD ">>"
// Variável global (para o sinal)
SignalContext contexto;  // Estrutura que guarda o contexto do sinal

int configServer(Motor* motor);
int readLevelMap(char* path, char board[ROWS][COLLUMN]);
int validateCommand(char* input, Motor* motor);
void showInfo(Motor motor, char type);
int execBot(Motor* motor);
void singalHandler(int signum);

int main() {
    Motor servidor;   // Estrutura que guarda o servidor
    char input[MAX];  // String que guarda o input do utilizador
    char* inputResult;

    //  Configura o servidor
    if (configServer(&servidor) == -1)
        return -1;

    while (!contexto.flag) {
        // Imprime o prompt e lê o input do utilizador
        printf("%s ", TAG_CMD);
        inputResult = fgets(input, sizeof(input), stdin);

        if (inputResult == NULL) {
            printf("%s Erro ao ler o input\n", TAG_MOTOR);
            break;
        }

        /**
         * Valida o comando
         * Se o comando for "end" retorna -1 para sair do programa
         */
        if (validateCommand(input, &servidor) == -1)
            break;
    }

    printf("%s A sair do programa...\n", TAG_MOTOR);

    return 0;
}

/**
 * Configura o servidor
 * \param motor Ponteiro da estrutura que guarda o servidor
 */
int configServer(Motor* motor) {
    // TODO [META 2]: "Mandar" o ficheiro → Mandar o caminha para a pasta com os mapas
    /**
     * Lê o mapa do ficheiro e coloca na estrutura do servidor
     * Se retornar -1, houve um erro ao ler o ficheiro
     */
    if (readLevelMap("map/level1.txt", motor->level.board) == -1)
        return -1;

    struct sigaction signal;
    signal.sa_handler = singalHandler;
    signal.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGINT, &signal, NULL);

    // Variáveis de ambiente
    char* timerBegin_env = getenv("INSCRICAO");
    char* timerGame_env = getenv("DURACAO");
    char* stepTimerGame_env = getenv("DECREMENTO");
    char* nUserMin_env = getenv("NPLAYERS");

    // Verifica se as variáveis de ambiente foram lidas corretamente
    if (timerBegin_env == NULL || timerGame_env == NULL || stepTimerGame_env == NULL || nUserMin_env == NULL) {
        printf("%s Erro ao ler as variáveis de ambiente\n", TAG_MOTOR);
        return -1;
    }

    // Inicializa o servidor com os valores padrões
    motor->level.level = 1;
    motor->timerBegin = atoi(timerBegin_env);        // ← INSCRICAO
    motor->timerGame = atoi(timerGame_env);          // ← DURACAO
    motor->stepTimerGame = atoi(stepTimerGame_env);  // ← DECREMENTO
    motor->nUserMin = atoi(nUserMin_env);            // ← NPLAYERS
    motor->nUserOn = 0;
    motor->nBotOn = 0;
    motor->nRockOn = 0;
    motor->nMoveBlockOn = 0;

    // Configuração dos ponteiro do contexto do sinal
    contexto.flag = 0;
    contexto.botList = motor->botList;
    contexto.nBotOn = &motor->nBotOn;

    // Imprime a configuração do servidor
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

        else if (!strcmp(cmd, "kick")) {
            if (argc != 2)  // Verifica se o numero de argumentos é valido
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
            /**
             * Executa o bot
             * Se retornar -1, houve um erro ao executar o bot
             */
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
            printf("Level: %d\n", motor.level.level);
            break;

        // Tempo
        case 't':
            printf("Tempo de jogo: %d\n", motor.timerGame);
            printf("Tempo de desconto: %d\n", motor.stepTimerGame);
            break;
    }
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
    while (!contexto.flag) {
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

void singalHandler(int signum) {
    printf("\n%s Recebi o sinal %d [Sinal para sair]\nComecar a terminar o motor e os seus mecanismos\n", TAG_MOTOR, signum);

    printf("Parar de ler o pipe dos bots\n");
    contexto.flag = 1;

    printf("Matar bots\n");

    union sigval val;
    for (int i = 0; i < *contexto.nBotOn; i++) {
        printf("Bot: %d [PID - %d]\n", i + 1, contexto.botList[i]);
        sigqueue(contexto.botList[i], SIGINT, val);
    }

    printf("%s Caso nao tenha saido, precione ENTER para enviar o comando que esta em espera\n", TAG_MOTOR);
}