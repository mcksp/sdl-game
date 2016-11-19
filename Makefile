build: main.c objects.c	client_udp.c
	gcc -o game main.c objects.c client_udp.c `sdl2-config --cflags --libs` -I.

