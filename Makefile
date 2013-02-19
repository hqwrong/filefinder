phony:
	gcc -std=c99 -o ff ff.c bar.c `pkg-config --libs --cflags gtk+-2.0`
	mv ff ~/bin
