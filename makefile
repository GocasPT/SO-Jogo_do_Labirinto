C = gcc
NCURSES = -lncursesw
THREAD = -pthread
CFLAGS = -Wall -Wextra

UTILS = utils
NAMEPIPE = NamePipe
JOGOUI = jogoUI
UI = UI
MOTOR = motor
CONSOLA = Consola
MAPMANAGER = MapManager
BOT = bot

UTILS_DIR = ./${UTILS}/${UTILS}

JOGOUI_DIR = ./${JOGOUI}/${JOGOUI}
JOGOUI_NAMEPIPE_DIR = ./${JOGOUI}/${NAMEPIPE}/${NAMEPIPE}
UI_DIR = ./${JOGOUI}/${UI}/${UI}

MOTOR_DIR = ./${MOTOR}/${MOTOR}
MOTOR_NAMEPIPE_DIR = ./${MOTOR}/${NAMEPIPE}/${NAMEPIPE}
CONSOLA_DIR = ./${MOTOR}/${CONSOLA}/${CONSOLA}
MAPMANAGER_DIR = ./${MOTOR}/${MAPMANAGER}/${MAPMANAGER}

BOT_DIR = ./${BOT}/${BOT}

all: jogoUI motor bot

jogoUI: ${JOGOUI_DIR}.c ${JOGOUI_NAMEPIPE_DIR}.c ${UI_DIR}.c
	${C} -o ${JOGOUI_DIR} -s ${JOGOUI_DIR}.c ${JOGOUI_NAMEPIPE_DIR}.c ${UI_DIR}.c ${NCURSES} ${THREAD} ${CFLAGS}

motor: ${MOTOR_DIR}.c ${MOTOR_NAMEPIPE_DIR}.c ${CONSOLA_DIR}.c ${MAPMANAGER_DIR}.c
	${C} -o ${MOTOR_DIR} -s ${MOTOR_DIR}.c ${MOTOR_NAMEPIPE_DIR}.c ${CONSOLA_DIR}.c ${MAPMANAGER_DIR}.c ${THREAD} ${CFLAGS}

bot: ${BOT_DIR}.c
	${C} -o ${BOT_DIR} -s ${BOT_DIR}.c

clean:
	rm ${JOGOUI_DIR} ${MOTOR_DIR} ${BOT_DIR}