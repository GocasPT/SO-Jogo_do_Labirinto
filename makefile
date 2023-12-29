CC = gcc
NCURSES = -lncursesw
THREAD = -lpthread
CFLAGS = -Wall -Wextra -g

UTILS = utils
NAMEPIPE = NamePipe
JOGOUI = jogoUI
UI = UI
MOTOR = motor
CONSOLA = Consola
MAPMANAGER = MapManager
BOT = bot

SRC_DIR = ./src
OBJ_DIR = ./obj

UTILS_DIR = ${SRC_DIR}/${UTILS}/${UTILS}

JOGOUI_DIR = ${SRC_DIR}/${JOGOUI}/${JOGOUI}
JOGOUI_NAMEPIPE_DIR = ${SRC_DIR}/${JOGOUI}/${NAMEPIPE}/${NAMEPIPE}
UI_DIR = ${SRC_DIR}/${JOGOUI}/${UI}/${UI}

MOTOR_DIR = ${SRC_DIR}/${MOTOR}/${MOTOR}
MOTOR_NAMEPIPE_DIR = ${SRC_DIR}/${MOTOR}/${NAMEPIPE}/${NAMEPIPE}
CONSOLA_DIR = ${SRC_DIR}/${MOTOR}/${CONSOLA}/${CONSOLA}
MAPMANAGER_DIR = ${SRC_DIR}/${MOTOR}/${MAPMANAGER}/${MAPMANAGER}

BOT_DIR = ${SRC_DIR}/${BOT}/${BOT}

OBJ_JOGOUI = ${OBJ_DIR}/${JOGOUI}.o ${OBJ_DIR}/${NAMEPIPE}_${JOGOUI}.o ${OBJ_DIR}/${UI}.o
OBJ_MOTOR = ${OBJ_DIR}/${MOTOR}.o ${OBJ_DIR}/${NAMEPIPE}_${MOTOR}.o ${OBJ_DIR}/${CONSOLA}.o ${OBJ_DIR}/${MAPMANAGER}.o
OBJ_BOT = ${OBJ_DIR}/${BOT}.o

all: jogoUI motor bot

jogoUI: ${OBJ_JOGOUI}
	${CC} -o ${JOGOUI} ${OBJ_JOGOUI} ${NCURSES} ${THREAD} ${CFLAGS}

motor: ${OBJ_MOTOR}
	${CC} -o ${MOTOR} ${OBJ_MOTOR} ${THREAD} ${CFLAGS}

bot: ${OBJ_BOT}
	${CC} -o ${BOT} ${OBJ_BOT} ${CFLAGS}

${OBJ_DIR}/${JOGOUI}.o: ${JOGOUI_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${NAMEPIPE}_${JOGOUI}.o: ${JOGOUI_NAMEPIPE_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${UI}.o: ${UI_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${MOTOR}.o: ${MOTOR_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${NAMEPIPE}_${MOTOR}.o: ${MOTOR_NAMEPIPE_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${CONSOLA}.o: ${CONSOLA_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${MAPMANAGER}.o: ${MAPMANAGER_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

${OBJ_DIR}/${BOT}.o: ${BOT_DIR}.c
	${CC} -c $< -o $@ ${CFLAGS}

clean:
	rm -rf ${OBJ_DIR}/*.o ${JOGOUI} ${MOTOR} ${BOT}
