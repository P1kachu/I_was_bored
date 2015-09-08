# Makefile
CC=gcc
FLAGS=-g -std=c99 -Wall -Werror -Wextra -pedantic
LIBS= `sdl-config --cflags --libs`
SRC= pixel_operations.c kbhit.c chip.c main.c
OUTPT=emulator

all: main

main:
	$(CC) $(FLAGS) $(SRC) -o $(OUTPT) $(LIBS)

clean:
	rm -rf $(OUTPT) *~ *.\# *\# 

