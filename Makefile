SRC_DIR=src
BUILD_DIR=build/debug
CC=clang++
SRC_FILES=${wildcard ${SRC_DIR}/*.cpp}
OBJ_NAME=life
COMPILER_FLAGS=-std=c++17 -Wall -O0 -g
LINKER_FLAGS=-lsdl2

all:
	bash -c "mkdir -p build/debug"
	${CC} ${COMPILER_FLAGS} ${LINKER_FLAGS} ${SRC_FILES} -o ${BUILD_DIR}/${OBJ_NAME}

clean:
	rm -rf ${BUILD_DIR}/*

run:
	./${BUILD_DIR}/${OBJ_NAME}

gdb:
	sudo gdb ./${BUILD_DIR}/${OBJ_NAME}
