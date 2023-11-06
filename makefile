JOGOUI = jogoUI
MOTOR = motor
BOT = bot
NCURSES = -lncursesw
CFLAGS = -Wall -Werror -Wextra
C = gcc

all: jogoUI motor bot

#TODO [DUVIDA]: Por os executaveis na pasta root do projeto
jogoUI: ./${JOGOUI}/${JOGOUI}.c
	${C} -o ./${JOGOUI}/${JOGOUI} -s ./${JOGOUI}/${JOGOUI}.c ${NCURSES} ${CFLAGS}

motor: ./${MOTOR}/${MOTOR}.c 
	${C} -o ./${MOTOR}/${MOTOR} -s ./${MOTOR}/${MOTOR}.c ${CFLAGS}

bot: ./${BOT}/${BOT}.c
	${C} -o ./${BOT}/${BOT} -s ./${BOT}/${BOT}.c

clean:
	rm ./${JOGOUI}/${JOGOUI} ./${MOTOR}/${MOTOR} ./${BOT}/${BOT}