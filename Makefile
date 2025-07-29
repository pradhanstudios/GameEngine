COMPILER = g++
WARNING_OPTIONS = -Wextra -Wall -Werror
SOURCE = src/*.cpp src/core/*.cpp
INCLUDE = -I/usr/include/freetype2 -I/usr/include/glm
LIBS_PATHS = -L/usr/lib/x86_64-linux-gnu
LIBS = -lGLEW -lglfw -lGL -lm -lfreetype
TARGET = GameEngine
MAIN_CMD = -o $(TARGET) $(SOURCE) $(INCLUDE) $(LIBS_PATHS) $(LIBS)


default:
	$(COMPILER) $(MAIN_CMD)

warnings:
	$(COMPILER) $(MAIN_CMD) $(WARNING_OPTIONS)

debug:
	$(COMPILER) -g $(MAIN_CMD) $(WARNING_OPTIONS)	
