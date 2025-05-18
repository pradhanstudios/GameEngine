default:
	g++ -o GameEngine src/core/*.cpp src/main.cpp -lGL -lGLEW -lGLU -lglut -lm -I /usr/include/freetype2 -lfreetype

warnings:
	g++ -o GameEngine src/core/*.cpp src/main.cpp -lGL -lGLEW -lGLU -lglut -lm -I /usr/include/freetype2 -lfreetype -Wextra -Wall -Werror
