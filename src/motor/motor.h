#ifndef MOTOR_H
#define MOTOR_H

#include "../utils/utils.h"
#include "NamePipe/NamePipe.h"

#define MAX_LEVEL 3        // Número máximo de níveis
#define MAX_USER 10        // Número máximo de utilizadores
#define MAX_BOT 10         // Número máximo de bots
#define MAX_ROCK 50        // Número máximo de pedras
#define MAX_MOVE_BLOCK5 5  // Número máximo de blocos móveis de 5 segundos

#define TAG_MOTOR "<Motor>"
#define TAG_CMD " >>"

// Estrutura de dados do jogador
typedef struct {
    char username[MAX];  // Nome do jogador
    char sprite;         // Sprite do jogador
    int x, y;            // Posição
} Player;

// Estrutura de dados da parede em movimento
typedef struct {
    int x, y;  // Posição
    int dir;   // Direção
    int step;  // Passo
} MoveBlock;

// Estrutura de dados da pedra
typedef struct {
    int x, y;  // Posição
    int time;  // Duração
} Rock;

// TODO: structs das thread datas (AQUI PARA N OUVER STRESS)
//  TODO: verificar esta struct
typedef struct {
    int* endThread;  // Ponteiro para a flag de fim de thread
    Level* level;    // Ponteiro para o nível

    int* nUserOn;        //
    User* userList;      //
    int* nPlayersOn;     //
    Player* playerList;  //
} ThreadData_ReadPipe;

// Estrutura de dados do motor/servidor
// TODO: verificar se tem num max de players
typedef struct {
    Level level;                                 // Nível
    int timerBegin;                              // Tempo de início
    int timerGame, stepTimerGame;                // Tempo de jogo e tempo de desconto
    int nUserMin;                                // Número mínimo de jogadores
    int nUserOn, nBotOn, nRockOn, nMoveBlockOn;  // Número de jogadores, bots, pedras e paredes em movimento
    User userList[MAX_USER];                     // Lista de utilizadores (conexoes)
    int nPlayersOn;                              // Número de jogadores
    Player playerList[MAX_USER];                 // Lista de jogadores (players)
    int botList[MAX_BOT];                        // Lista de bots
    Rock rockList[MAX_ROCK];                     // Lista de pedras
    MoveBlock moveBlockList[MAX_MOVE_BLOCK5];    // Lista de paredes em movimento

    pthread_t threadReadPipe;                // Thread de leitura do name pipe
    ThreadData_ReadPipe threadReadPipeData;  // Dados da thread de leitura do name pipe
    pthread_t threadReadBots;                // Thread de leitura dos bots
    pthread_t threadTick;                    // Thread que gera o tick do jogo

    int* endFlag;
} Motor;

#endif  // MOTOR_H