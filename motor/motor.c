#include "motor.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "MapManager/MapManager.h"

// Variável global (para o sinal)
// TODO: mudar para outro tipo de variável
SignalContext contexto;  // Estrutura que guarda o contexto do sinal

// TODO: atualizar a função
/**
 * Handler do sinal
 * \param signum Número do sinal
 */
void singalHandler(int signum) {
    printf("\n%s Recebi o sinal %d [Sinal para sair]\nComecar a terminar o motor e os seus mecanismos\n", TAG_MOTOR, signum);

    //  Ativa a endFlag para sair dos loops
    printf("Parar de ler o pipe dos bots\n");
    contexto.endFlag = 1;

    // Termina os bots
    printf("Matar bots\n");
    union sigval val;
    for (int i = 0; i < *contexto.nBotOn; i++) {
        printf("Bot: %d [PID - %d]\n", i + 1, contexto.botList[i]);
        sigqueue(contexto.botList[i], SIGINT, val);
    }

    // Sai da função e avisa caso nao tenha acabado introduzir o comando ou ENTER
    printf("%s Caso nao tenha saido, precione ENTER para enviar o comando que esta em espera\n", TAG_MOTOR);
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
    contexto.endFlag = 0;
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

int main() {
    Motor servidor;   // Estrutura que guarda o servidor
    char input[MAX];  // String que guarda o input do utilizador
    char* inputResult;

    // Verifica se existe um motor aberto
    // TODO: checkMotor();

    //  Configura o servidor
    if (configServer(&servidor) == -1)
        return -1;

    // TODO: thread
    while (!contexto.endFlag) {
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

    // TODO: fechar programa (esperar thread e fazer limpeza das cenas)
    printf("%s A sair do programa...\n", TAG_MOTOR);

    return 0;
}