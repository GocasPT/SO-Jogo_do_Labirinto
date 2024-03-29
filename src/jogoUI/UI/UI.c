#include "UI.h"

#include <signal.h>
#include <string.h>

#include "../NamePipe/NamePipe.h"

/**
 * Função que configura as janelas do ncurses
 * \param ui Ponteiro da estrutura que guarda as janelas
 */
void configUI(UI* ui) {
    ui->maze = newwin(H_MAZE, W_MAZE, Y_MAZE, X_MAZE);
    ui->info = newwin(H_INFO, W_INFO, Y_INFO, X_INFO);
    ui->notification = newwin(H_NOTIFICATION, W_NOTIFICATION, Y_NOTIFICATION, X_NOTIFICATION);
    ui->console = newwin(H_INPUT, W_INPUT, Y_INPUT, X_INPUT);

    box(ui->maze, 0, 0);  // Desenha a borda da janela
    box(ui->info, 0, 0);  // Desenha a borda da janela

    scrollok(ui->notification, TRUE);  // Ativa o scroll da janela
    keypad(ui->maze, TRUE);            // Ativa o keypad da janela

    noecho();     // Desativa o echo
    cbreak();     // Ativa o cbreak
    curs_set(0);  // Desativa o cursor

    wrefresh(ui->maze);
    wrefresh(ui->info);
    wrefresh(ui->notification);
    wrefresh(ui->console);
}

/**
 * Função que desenha o labirinto na janela
 * \param ui Ponteiro da estrutura que guarda as janelas para desenhar
 * \param board Matriz que guarda o labirinto
 */
void drawMaze(UI* ui, char board[ROWS][COLLUMN]) {
    // wclear(ui->maze);     // Limpa a janela do tabuleiro
    box(ui->maze, 0, 0);  // Desenha a borda da janela

    // Percorre as linhas to tabuleiro e imprime elas
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLLUMN; j++)
            mvwprintw(ui->maze, i + 1, j + 1, "%c", board[i][j]);
    // mvwprintw(ui->maze, i + 1, 1, "%s", board[i]);

    wrefresh(ui->maze);  // Atualiza a janela

    mvwprintw(ui->info, 1, 1, "Nivel: %d\n", 1);  // TODO: mudar para o nivel atual
    wrefresh(ui->info);                           // Atualiza a janela
}

/**
 * Função que valida o comando introduzido pelo utilizador
 * \param ui Ponteiro da estrutura que guarda as janelas para escrever na notificação ou na informação
 * \param input String que guarda o input do utilizador
 */
int validateCommand(UI* ui, char* input) {
    char cmd[MAX];      // String que guarda o comando
    char argv[2][MAX];  // Vetor de strings que guarda os argumentos do comando
    int argc;           // Numero de argumentos do comando

    argc = sscanf(input, "%s %s %90[^\n]", cmd, argv[0], argv[1]);

    if (argc != 0) {
        if (!strcmp(cmd, "help"))
            wprintw(ui->notification, "Comando help:\n- players\n- msg <username> <mensagem>\n- exit\n");

        else if (!strcmp(cmd, "players")) {
            wprintw(ui->notification, "Comando players\n");
            CommandToServer cmd = {
                .PID = getpid(),
                .cmd = CMD_PLIST,
                .arg = "",
            };
            writeMotor(ui, cmd);
        }

        else if (!strcmp(cmd, "msg")) {
            if (argc < 3)  // Verifica se o numero de argumentos é valido
                wprintw(ui->notification, "Comando de msg invalido\n\tmsg <username> <mensagem>\n");
            else {
                wprintw(ui->notification, "Comando msg\nDestinatario: %s\nMensagem: %s\n", argv[0], argv[1]);
                writeMessage(ui, argv[0], argv[1]);
            }
        }

        else if (!strcmp(cmd, "exit")) {
            CommandToServer cmd = {
                .PID = getpid(),
                .cmd = CMD_DESCONNECT,
                .arg = ""  // TODO [??]: mandar username
            };

            wprintw(ui->notification, "PID: %d\n cmd: %s\n arg: %s\n", cmd.PID, cmd.cmd, cmd.arg);
            wrefresh(ui->notification);

            writeMotor(ui, cmd);
            wrefresh(ui->notification);

            sigqueue(getpid(), SIGINT, (const union sigval)NULL);

            // TODO [?]: ainda fica aqui isto
            wprintw(ui->notification, "Comando exit\nPreciona 'Enter' para sair");
            wrefresh(ui->notification);
            return 1;
        }

        else
            wprintw(ui->notification, "Comnado invalido\n");
    }

    wrefresh(ui->notification);
    return 0;
}

/**
 * Função que lê o input do utilizador
 * \param jogoUI Ponteiro da estrutura princpal do jogoUI
 */
void readInput(UI* ui) {
    char input[MAX];  // String que guarda o input do utilizador
    int key, exit;    // Variavel que guarda a tecla de movimento do utilizador
    CommandToServer cmd = {
        .PID = getpid(),
        .cmd = CMD_MOVE,
    };

    // TODO [?]: sair do logo ou fazer o processo encerrar
    exit = 0;  // exit = false
    while (exit != 1) {
        // drawMaze(ui, level->board);  // Desenha o labirinto

        key = wgetch(ui->maze);  // Espera por uma tecla de movimento
        switch (key) {
            case KEY_UP:
                strcpy(cmd.arg, ARG_UP);
                writeMotor(ui, cmd);
                break;
            case KEY_DOWN:
                strcpy(cmd.arg, ARG_DOWN);
                writeMotor(ui, cmd);
                break;
            case KEY_LEFT:
                strcpy(cmd.arg, ARG_LEFT);
                writeMotor(ui, cmd);
                break;
            case KEY_RIGHT:
                strcpy(cmd.arg, ARG_RIGHT);
                writeMotor(ui, cmd);
                break;

            // Se a tecla for SPACE, entra no modo de comando
            case ' ':
                echo();       // Ativa o echo
                nocbreak();   // Desativa o cbreak
                curs_set(1);  // Ativa o cursor

                // Escreve o TAG na janela e espera pelo input do utilizador
                mvwprintw(ui->console, 1, 1, TAG);
                wrefresh(ui->console);
                wscanw(ui->console, " %100[^\n]", input);

                /**
                 * Valida o comando
                 * Se o comando for exit, retorna 1 para sair do jogo
                 */
                if (validateCommand(ui, input) == 1)
                    exit = 1;

                // Limpa a janela do input
                wmove(ui->console, 1, 1);
                wclrtoeol(ui->console);
                wrefresh(ui->console);

                noecho();     // Desativa o echo
                cbreak();     // Ativa o cbreak
                curs_set(0);  // Desativa o cursor
                break;
        }
        wrefresh(ui->notification);  // Atualiza a janela da notificação
    }

    // Espera pelo utilizador precionar 'Enter' para sair e termina o ncurses
    wgetch(ui->console);
}