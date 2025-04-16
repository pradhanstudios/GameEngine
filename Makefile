# sudo apt-get install freeglut3-dev
default:
	g++ -o GameEngine src/core/*.cpp src/main.cpp -lGL -lGLU -lglut -lm