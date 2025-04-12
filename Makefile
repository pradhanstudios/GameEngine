# sudo apt-get install freeglut3-dev
default:
	g++ -o GameEngine src/core/*.cpp -lGL -lGLU -lglut -lm