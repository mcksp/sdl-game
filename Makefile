build: main.c objects.c	
	gcc -o game main.c objects.c `sdl2-config --cflags --libs` -I.

