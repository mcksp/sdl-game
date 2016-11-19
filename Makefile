build: main.c objects.c	client_udp.c server_udp.c
	gcc -o game main.c objects.c client_udp.c server_udp.c `sdl2-config --cflags --libs` -I.

