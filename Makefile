build: main.c objects.c	client_udp.c server_udp.c network.c physic.c
	gcc -o game main.c objects.c client_udp.c server_udp.c network.c physic.c `sdl2-config --cflags --libs` -lSDL2_ttf -lpthread -I. -Wall

