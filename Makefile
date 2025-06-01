COMPILER = g++
WARNING_OPTIONS = -Wextra -Wall -Werror
SOURCE = src/*.cpp src/core/*.cpp
INCLUDE = -I/usr/include/freetype2 -I/usr/include/glm
LIBS = -lGL -lGLEW -lGLU -lglut -lm -lfreetype
TARGET = GameEngine


default:
	$(COMPILER) -o $(TARGET) $(SOURCE) $(INCLUDE) $(LIBS)

warnings:
	$(COMPILER) -o $(TARGET) $(SOURCE) $(INCLUDE) $(LIBS) $(WARNING_OPTIONS)

debug:
	$(COMPILER) -g -o $(TARGET) $(SOURCE) $(INCLUDE) $(LIBS) $(WARNING_OPTIONS)	
