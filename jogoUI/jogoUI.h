#ifndef JOGOUI_H
#define JOGOUI_H

#define MAX 100  // Numerode caracteres maximo que as strings podem ter (99 + 1 para o \0)

// Estruturas de dados do jogadore
typedef struct {
    int PID;             // PID do processo do jogador
    char username[MAX];  // Nome do jogador
} User;

// Esturtura de dados dos comandos que será enviado para o servidor
typedef struct {
    char cmd[10];  // Comando - Identificar do tipo de comando (move, msg, plist)
    char arg[20];  // Argumentos do comando (move: up, down, left, right; msg: username)
} CommandToServer;

#endif  // JOGOUI_H