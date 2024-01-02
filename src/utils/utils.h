#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <unistd.h>

#define MAX 100  // Numero de caracteres maximo que as strings podem ter (99 + 1 para o \0)

#define ROWS 16         // 16 linhas
#define COLLUMN 40 + 1  // 40 colunas + 1 para o \0

#define FIFO_JOGOUI "fifo_JogoUI_%d"  // Nome do FIFO (base) do jogoUI
#define FIFO_MOTOR "fifo_Motor"       // Nome do FIFO do motor

#define DATA_LEVEL 1     // Data do tipo Level
#define DATA_FEEDBACK 2  // Data do tipo feedback do motor
#define DATA_MSG 3       // Data do tipo mensagem de um jogoUI

#define CMD_CONNECT "connect"        // TODO: docs
#define CMD_DESCONNECT "disconnect"  // TODO: docs
#define CMD_MOVE "move"              // TODO: docs
#define CMD_MSG "msg"                // TODO: docs
#define CMD_PLIST "plist"            // TODO: docs

#define ARG_UP "up"        // TODO: docs
#define ARG_DOWN "down"    // TODO: docs
#define ARG_LEFT "left"    // TODO: docs
#define ARG_RIGHT "right"  // TODO: docs

#define FEEDBACK_CLOSE "close"      // TODO: docs
#define FEEDBACK_INVALID "invalid"  // TODO: docs

// Estruturas de dados do jogadore
typedef struct {
    pid_t PID;           // PID do processo do jogador
    char username[MAX];  // Nome do jogador
} User;

// Estrutura de dados do nível
typedef struct {
    char board[ROWS][COLLUMN];  // Tabuleiro
    int level;                  // Nível
} Level;

// Estrutura de dados do feedback do motor
// TODO: ver o que se pode user aqui (respotas do motor)
typedef struct {
    char feedback[MAX];    // Feedback do motor
    int numPlayers;        // Numero de jogadores
    char players[5][MAX];  // Lista de jogadores
    User user;             // Dados do jogador que o jogoUI pediu
} FeedBack;

// Estrutura de dadods do pacote de mensagem que um jogoUI envia para outro jogoUI
typedef struct {
    char username[MAX];  // Nome do jogador
    char msg[MAX];       // Mensagem
} MSG;

// Estrutura de dados do pacote de mensagem que o jogoUI vai receber (do motor ou de outro jogoUI)
// TODO: ver struct (e union)
typedef struct {
    int dataType;  // Tipo de dados (Level, FeedBack, MSG)
    union {
        Level level;        // Dados do nível
        FeedBack feedBack;  // Dados do feedback do motor
        MSG msg;            // Dados da mensagem
    } data;                 // Dados (reduzir tamanho da struct)
} DataRecive;

// Estrutura de dados dos comandos que será enviado para o motor
typedef struct {
    pid_t PID;      // PID do processo do jogador
    char cmd[10];   // Comando - Identificar do tipo de comando (move, msg, plist)
    char arg[MAX];  // Argumentos do comando (move: up, down, left, right; msg: username)
} CommandToServer;

#endif  // UTILS_H