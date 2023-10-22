JOGOUI = jogoUI
MOTOR = motor

all: jogoUI motor

jogoUI:
	gcc ./${JOGOUI}/${JOGOUI}.c -o /${JOGOUI}/${JOGOUI}

motor:
	gcc ./${MOTOR}/${MOTOR}.c -o ./${MOTOR}/${MOTOR}

clean:
	rm ./${JOGOUI}/${JOGOUI} ./${MOTOR}/${MOTOR}