#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <unistd.h>

#define MAX 100  // Numerode caracteres maximo que as strings podem ter (99 + 1 para o \0)

#define ROWS 16         // 16 linhas
#define COLLUMN 40 + 1  // 40 colunas + 1 para o \0

#define FIFO_JOGOUI "fifo_JogoUI_%d"
#define FIFO_MOTOR "fifo_Motor"

// Estruturas de dados do jogadore
typedef struct {
    pid_t PID;           // PID do processo do jogador
    char username[MAX];  // Nome do jogador
} User;

// Esturtura de dados dos comandos que será enviado para o servidor
typedef struct {
    char cmd[10];  // Comando - Identificar do tipo de comando (move, msg, plist)
    char arg[20];  // Argumentos do comando (move: up, down, left, right; msg: username)
} CommandToServer;

// Estrutura de dados do nível
typedef struct {
    char board[ROWS][COLLUMN];  // Tabuleiro
    int level;                  // Nível
} Level;

#endif  // UTILS_H