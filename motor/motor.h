#ifndef MOTOR_H
#define MOTOR_H

#include "../utils/utils.h"

#define MAX_LEVEL 3        // Número máximo de níveis
#define MAX_USER 10        // Número máximo de utilizadores
#define MAX_BOT 10         // Número máximo de bots
#define MAX_ROCK 50        // Número máximo de pedras
#define MAX_MOVE_BLOCK5 5  // Número máximo de blocos móveis de 5 segundos

#define TAG_MOTOR "<Motor>"
#define TAG_CMD ">>"

// Estrutura de dados do jogador
typedef struct {
    char username[MAX];  // Nome do jogador
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

// Estrutura de dados do motor/servidor
// TODO: verificar se tem num max de players
typedef struct {
    Level level;                                 // Nível
    int timerBegin;                              // Tempo de início
    int timerGame, stepTimerGame;                // Tempo de jogo e tempo de desconto
    int nUserMin;                                // Número mínimo de jogadores
    int nUserOn, nBotOn, nRockOn, nMoveBlockOn;  // Número de jogadores, bots, pedras e paredes em movimento
    User userList[MAX_USER];                     // Lista de utilizadores (conecxoes)
    Player playerList[MAX_USER];                 // Lista de jogadores (players)
    int botList[MAX_BOT];                        // Lista de bots
    Rock rockList[MAX_ROCK];                     // Lista de pedras
    MoveBlock moveBlockList[MAX_MOVE_BLOCK5];    // Lista de paredes em movimento
} Motor;

// Estrutura de dados do contexto do sinal
// TODO: converter para int e ser algo mais sequencial (sai do loop e acabas as cenas)
typedef struct {
    int endFlag;   // Flag de saida - 0: continua; 1: termina
    int *botList;  // Ponteiro para a lista de bots
    int *nBotOn;   // Ponteiro para o número de bots
} SignalContext;

#endif  // MOTOR_H