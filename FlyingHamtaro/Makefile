## Simple Makefile

CC=clang++

CPPFLAGS=
CFLAGS= -Wall -Wextra -Werror
LDFLAGS=
LDLIBS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
NAME=hamtaro
SRC= Doves.cpp main.cpp Annexes.cpp

all: main

main:
	$(CC) $(CFLAGS) $(LDLIBS) $(SRC) -o $(NAME)

clean:
	rm -f *~ *.o
	rm -f main

#END
