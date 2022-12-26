include config.mk

SRC = src
OBJ = obj

SRCS = ${wildcard ${SRC}/*.c}
OBJS = ${patsubst ${SRC}/%.c, ${OBJ}/%.o, ${SRCS}}

all: ${BIN}

${BIN}: ${OBJS}
	${CC} ${CFLAGS} ${LIBS} ${OBJS} -o $@

${OBJ}/%.o: ${SRC}/%.c
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

ctests: ${wildcard tests/*.c}
	${CC} -O2 -Wall -g ${LIBS} -I./includes -lcnct -o test_socket tests/raw_socket_test.c
	${CC} -O2 -Wall -g ${LIBS} -I./includes -lcnct -o test_endpoint tests/basic_endpoint.c

.PHONY: clean install
clean:
	rm ${OBJ}/*.o ${BIN}
	rm ./test_endpoint ./test_socket

install:
	cp ${BIN} /usr/lib/
