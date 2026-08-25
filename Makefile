build:
	g++ -o game.out src/main.cpp src/glad.c -Iinclude `sdl2-config --cflags --libs` -lGL -ldl
