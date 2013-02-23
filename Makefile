phony:
	gcc -g -std=c99 -o ff ff.c bar.c match.c `pkg-config --libs --cflags gtk+-2.0`

install:
	mv ff ~/bin
