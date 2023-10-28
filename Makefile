#TODO: makefile com .PHONY para forçar a compilação de todos os arquivos como deve ser

JOGOUI = jogoUI
MOTOR = motor
BOT = bot
NCURSES = -lncurses
C = gcc

.PHONY: all jogoUI motor clean
all: jogoUI motor

jogoUI:
	${C} -o ./${JOGOUI}/${JOGOUI} ./${JOGOUI}/${JOGOUI}.c ${NCURSES}

motor:
	${C} -o ./${MOTOR}/${MOTOR} ./${MOTOR}/${MOTOR}.c 

clean:
	rm ./${JOGOUI}/${JOGOUI} ./${MOTOR}/${MOTOR}