JOGOUI = jogoUI
MOTOR = motor
BOT = bot
NCURSES = -lncurses
C = gcc

.PHONY: all jogoUI motor bot clean
all: jogoUI motor bot

jogoUI:
	${C} -o ./${JOGOUI}/${JOGOUI} ./${JOGOUI}/${JOGOUI}.c ${NCURSES}

motor:
	${C} -o ./${MOTOR}/${MOTOR} ./${MOTOR}/${MOTOR}.c 

bot:
	${C} -o ./${BOT}/${BOT} ./${BOT}/${BOT}.c

clean:
	rm ./${JOGOUI}/${JOGOUI} ./${MOTOR}/${MOTOR} ./${BOT}/${BOT}