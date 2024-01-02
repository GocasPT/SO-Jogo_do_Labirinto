#include "NamePipe.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../jogoUI.h"

int createNamePipe(UI* ui, char FIFO_NAME[]) {
    int result;

    result = mkfifo(FIFO_NAME, 0666);
    if (result == -1) {
        wprintw(ui->notification, "Erro ao criar o FIFO do jogador\n");
        return -1;
    }

    return 0;
}

// TODO: docs
void* readNamePipe(void* lpram) {
    ThreadData* dados = (ThreadData*)lpram;
    DataRecive dataRecive;
    int fd;

    // Abrir os fifos
    // TODO: caso de erro, fechar o processo
    fd = open(dados->FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        wprintw(dados->ui->notification, "Erro ao abrir o FIFO do jogador\n");
        *(dados->endThread) = 1;
        close(fd);
        unlink(dados->FIFO_NAME);
        pthread_exit(NULL);
    }

    int nBytes;

    while (*(dados->endThread) != 1) {
        nBytes = read(fd, &dataRecive, sizeof(DataRecive));
        if (nBytes == -1) {
            wprintw(dados->ui->notification, "Erro ao ler o FIFO do jogador\n");
            *(dados->endThread) = 1;
            close(fd);
            unlink(dados->FIFO_NAME);
            pthread_exit(NULL);
        } else if (nBytes == 0)
            continue;
        else {
            // TODO: tratamento de dados
            if (dataRecive.dataType == DATA_LEVEL) {
                *(dados->level) = dataRecive.data.level;
                drawMaze(dados->ui, dados->level->board);
            }

            else if (dataRecive.dataType == DATA_FEEDBACK) {
                // TODO: logica
            }

            else if (dataRecive.dataType == DATA_MSG) {
                // TODO: logica
            }
        }
    }

    close(fd);

    pthread_exit(NULL);
}

// TODO: docs
// TODO: fução para escrever para o motor
void writeMotor(UI* ui, CommandToServer cmd) {
    int fd;

    fd = open(FIFO_MOTOR, O_WRONLY);
    // TODO: checkar erro
    if (fd == -1) {
        wprintw(ui->notification, "Erro ao abrir o FIFO do servidor\n");
        return;
    }

    int nBytes;

    nBytes = write(fd, &cmd, sizeof(CommandToServer));
    if (nBytes == -1) {
        wprintw(ui->notification, "Erro ao escrever no FIFO do servidor\n");
    }

    close(fd);

    return;
}

void writeMessage(UI* ui, char* username, char* message) {
    CommandToServer cmd = {
        .PID = getpid(),
        .cmd = CMD_MSG,
    };

    strcpy(cmd.arg, message);

    writeMotor(ui, cmd);

    int fd;
    char FIFO_FINAL[MAX];
    sprintf(FIFO_FINAL, FIFO_JOGOUI, getpid());

    fd = open(FIFO_FINAL, O_RDONLY);
    if (fd == -1) {
        wprintw(ui->notification, "Erro ao abrir o FIFO do jogador\n");
        return;
    }

    int nBytes;
    DataRecive dataRecive;
    int PID_user;
    while (1) {
        nBytes = read(fd, &dataRecive, sizeof(DataRecive));
        if (nBytes == -1) {
            wprintw(ui->notification, "Erro ao ler o FIFO do jogador\n");
            return;
        } else if (nBytes == 0)
            continue;
        else {
            if (dataRecive.dataType == DATA_FEEDBACK) {
                PID_user = dataRecive.data.feedBack.user.PID;
                break;
            }
        }
    }

    close(fd);

    sprintf(FIFO_FINAL, FIFO_JOGOUI, PID_user);
    dataRecive.dataType = DATA_MSG;
    MSG msg;
    strcpy(msg.msg, message);
    strcpy(msg.username, username);  // TODO: username do escrito, nao do destinatario
    dataRecive.data.msg = msg;

    fd = open(FIFO_FINAL, O_WRONLY);
    if (fd == -1) {
        wprintw(ui->notification, "Erro ao abrir o FIFO do jogador\n");
        return;
    }

    nBytes = write(fd, &msg, sizeof(MSG));
    if (nBytes == -1)
        wprintw(ui->notification, "Erro ao enviar o FIFO ao jogador destinado\n");
    else
        wprintw(ui->notification, "Mensagem enviada a %s", username);
}