CFLAGS= -lxml2 -lz -ljansson -lSDL -lmpg123 -lout123 -Wall -lGL -ldl -lglfw -I/usr/include/freetype2 -lfreetype -z muldefs 
OBJS=$(shell ls src/*.c*) $(shell ls src/*/*.c*)
all:
	g++ $(CFLAGS) $(OBJS) -o main.out /usr/lib/libtmx.a

start:
	./main.out
