BUILD=build
INC=inc
SRC=src

SRCFILES=$(patsubst %,%/*.cpp,${SRC})
INCFILES=$(patsubst %,%/*.h,${INC})

#installed package g++-arm-linux-gnueabihf on ubuntu
CXX=arm-linux-gnueabihf-g++
SDKINCPATH=/usr/arm-linux-gnueabihf/include
CFLAGS=-Wall -I./${INC} -I${SDKINCPATH}

dd: $(eval SHELL:=/bin/bash)
	echo ${SRCFILES}
	echo ${INCFILES}
	echo ${CFLAGS}
	echo ${SDKINCPATH}

all:
	${CXX} ${CFLAGS} ${SRCFILES} -o ${BUILD}/mainApp


.PHONY: clean

clean:
	rm -rf ${BUILD}/mainApp
