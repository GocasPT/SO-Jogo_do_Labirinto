#include "motor.h"

#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Consola/Consola.h"
#include "MapManager/MapManager.h"
#include "NamePipe/NamePipe.h"

// TODO: docs
int endFlag = 0;

// TODO: docs
void singalHandler(int sig, siginfo_t* info, void* context) {
    //  Ativa a endFlag para sair dos loops
    endFlag = 1;
}

/**
 * Função que veririca se o motor está a correr
 */
void checkMotorOpen() {
    int fd;
    fd = open(FIFO_MOTOR, O_WRONLY);
    if (fd != -1) {
        printf("\n%s Ja existe um motor a ser executdo\n", TAG_MOTOR);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

// TODO: docs
void configThreads(Motor* motor) {
    int resutl;

    motor->threadReadPipeData = (ThreadData_ReadPipe){
        .endThread = &endFlag,
        .level = &motor->level,
        .nUserOn = &motor->nUserOn,
        .userList = &motor->userList,
        .nPlayersOn = &motor->nPlayersOn,
        .playerList = &motor->playerList,
    };

    // TODO: argumento da thread (melhor uma srtuct dedica ou mandar o motor?)
    resutl = pthread_create(&motor->threadReadPipe, NULL, readNamePipe, &motor->threadReadPipeData);
    if (resutl != 0) {
        printf("Erro ao criar a thread de leitura do name pipe\n");
        exit(EXIT_FAILURE);
    } else
        printf("Thread \'readNamePipe\' criada\n");

    // TODO: crias função/thread para a leitura dos bots + argumento
    /*resutl = pthread_create(&motor->threadReadBots, NULL, readBots, NULL);
    if (resutl != 0) {
        printf("Erro ao criar a thread de leitura dos bots\n");
        return -1;
    } else
        printf("%s Thread'readBots' criada\n" TAG_MOTOR);*/

    // TODO: crias função/thread para os ticks + argumento
    /*resutl = pthread_create(&motor->threadTick, NULL, NULL, NULL);
    if (resutl != 0) {
        printf("Erro ao criar a thread de leitura dos bots\n");
        return -1;
    } else
        printf("%s Thread 'tick' criada\n" TAG_MOTOR);*/
}

/**
 * Configura o servidor
 * \param motor Ponteiro da estrutura que guarda o servidor
 */
void configServer(Motor* motor) {
    // TODO [META 2]: "Mandar" o ficheiro → Mandar o caminha para a pasta com os mapas
    /**
     * Lê o mapa do ficheiro e coloca na estrutura do servidor
     * Se retornar -1, houve um erro ao ler o ficheiro
     */
    if (readLevelMap("src/map/level1.txt", motor->level.board) == -1)
        exit(EXIT_FAILURE);

    // TODO: better load inicial map

    struct sigaction signal;
    signal.sa_sigaction = singalHandler;
    signal.sa_flags = SA_SIGINFO;
    // TODO: ifs
    sigaction(SIGINT, &signal, NULL);

    // Variáveis de ambiente
    char* timerBegin_env = getenv("INSCRICAO");
    char* timerGame_env = getenv("DURACAO");
    char* stepTimerGame_env = getenv("DECREMENTO");
    char* nUserMin_env = getenv("NPLAYERS");

    // Verifica se as variáveis de ambiente foram lidas corretamente
    if (timerBegin_env == NULL || timerGame_env == NULL || stepTimerGame_env == NULL || nUserMin_env == NULL) {
        printf("%s Erro ao ler as variáveis de ambiente\n", TAG_MOTOR);
        exit(EXIT_FAILURE);
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
    motor->nPlayersOn = 0;

    // Configuração da flag global para sair do programa
    endFlag = 0;

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

    printf("%s A criar o FIFO para a comunicacao com o motor\n", TAG_MOTOR);
    createNamePipe();

    printf("%s Configuracao da threads:\n", TAG_MOTOR);
    configThreads(motor);
}

// TODO: docs
void closeServer(Motor* motor) {
    // TODO: fechar programa (esperar thread e fazer limpeza das cenas)
    printf("%s A sair do programa...\n", TAG_MOTOR);

    for (int i = 0; i < motor->nBotOn; i++) {
        printf("Bot: %d [PID - %d]\n", i + 1, motor->botList[i]);
        sigqueue(motor->botList[i], SIGINT, (const union sigval)NULL);
        waitpid(motor->botList[i], NULL, 0);
    }

    // TODO: docs
    pthread_join(motor->threadReadPipe, NULL);
    // pthread_join(motor->threadReadBots, NULL);
    // pthread_join(motor->threadTick, NULL);

    unlink(FIFO_MOTOR);
}

int main() {
    Motor servidor;  // Estrutura que guarda o servidor

    // Verifica se existe um motor aberto
    checkMotorOpen();

    //  Configura o servidor
    configServer(&servidor);

    // TODO: docs
    readConsola(&servidor, &endFlag);

    // TODOS: docs
    closeServer(&servidor);

    return 0;
}