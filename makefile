JOGOUI = jogoUI
MOTOR = motor
BOT = bot
NCURSES = -lncurses
C = gcc

all: jogoUI motor bot

#TODO: Por os executaveis na pasta root do projeto
jogoUI: ./${JOGOUI}/${JOGOUI}.c
	${C} -o ./${JOGOUI}/${JOGOUI} ./${JOGOUI}/${JOGOUI}.c ${NCURSES}

motor: ./${MOTOR}/${MOTOR}.c 
	${C} -o ./${MOTOR}/${MOTOR} ./${MOTOR}/${MOTOR}.c 

bot: ./${BOT}/${BOT}.c
	${C} -o ./${BOT}/${BOT} ./${BOT}/${BOT}.c

clean:
	rm ./${JOGOUI}/${JOGOUI} ./${MOTOR}/${MOTOR} ./${BOT}/${BOT}