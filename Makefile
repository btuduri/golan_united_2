EXECUTABLE=golanunited2
SOURCES=main.c functions.c datas.c
EXTRALIBS=-lSDL2_image -lSDL2_ttf -lSDL2_mixer

#Linux build settings
#CC=gcc
#CFLAGS:=$(shell sdl2-config --cflags)
#LIBS=$(shell sdl2-config --libs) $(EXTRALIBS)

#Windows build settings
WINFOLDER:=/usr/x86_64-w64-mingw32/
CC:=x86_64-w64-mingw32-gcc
EXECUTABLE:=$(EXECUTABLE).exe
CFLAGS:=-I$(WINFOLDER)include/SDL2 -Dmain=SDL_main
LIBS=-L$(WINFOLDER)lib -lmingw32 -lSDL2main -lSDL2 -mwindows $(EXTRALIBS)

#all: gcc_$(EXECUTABLE)
#warn: CFLAGS += -Wall
#warn: all
#win32: all
#run:  all
#	./$(EXECUTABLE)
all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LIBS)

