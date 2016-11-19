build: main.c	
	gcc -o game main.c `sdl2-config --cflags --libs`

