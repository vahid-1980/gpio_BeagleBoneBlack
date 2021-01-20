BUILD=build
INC=inc
SRC=src

SRCFILES=$(patsubst %,%/*.cpp,${SRC})
INCFILES=$(patsubst %,%/*.h,${INC})

CXX=g++
CFLAGS=-Wall -I./${INC}

dd: $(eval SHELL:=/bin/bash)
	echo ${SRCFILES}
	echo ${INCFILES}
	echo ${CFLAGS}


all:
	${CXX} ${CFLAGS} ${SRCFILES} -o ${BUILD}/mainApp


.PHONY: clean

clean:
	rm -rf ${BUILD}/mainApp
