UTILS = utils
NAMEPIPE = NamePipe
JOGOUI = jogoUI
UI = UI
MOTOR = motor
BOT = bot
NCURSES = -lncursesw
THREAD = -pthread
CFLAGS = -Wall -Werror -Wextra
C = gcc

UTILS_DIR = ./${UTILS}/${UTILS}
JOGOUI_DIR = ./${JOGOUI}/${JOGOUI}
UI_DIR = ./${JOGOUI}/${UI}/${UI}
JOGOUI_NAMEPIPE_DIR = ./${JOGOUI}/${NAMEPIPE}/${NAMEPIPE}
MOTOR_DIR = ./${MOTOR}/${MOTOR}
MOTOR_NAMEPIPE_DIR = ./${MOTOR}/${NAMEPIPE}/${NAMEPIPE}

all: jogoUI motor bot

jogoUI: ${UTILS_DIR}.h ${JOGOUI_DIR}.c ${JOGOUI_DIR}.h ${UI_DIR}.c ${UI_DIR}.h
	${C} -o ./${JOGOUI}/${JOGOUI} ${UI_DIR}.c ${UI_DIR}.h ${UTILS_DIR}.h ${JOGOUI_DIR}.c ${JOGOUI_DIR}.h ${NCURSES} ${CFLAGS}

motor: ./${MOTOR}/${MOTOR}.c 
	${C} -o ./${MOTOR}/${MOTOR} -s ./${MOTOR}/${MOTOR}.c

bot: ./${BOT}/${BOT}.c
	${C} -o ./${BOT}/${BOT} -s ./${BOT}/${BOT}.c

clean:
	rm ./${JOGOUI}/${JOGOUI} ./${MOTOR}/${MOTOR} ./${BOT}/${BOT}