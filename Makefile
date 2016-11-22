build: main.c objects.c	client_udp.c server_udp.c network.c
	gcc -o game main.c objects.c client_udp.c server_udp.c network.c `sdl2-config --cflags --libs` -lpthread -I. -Wall

