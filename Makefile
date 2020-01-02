LDFLAGS = -lallegro -lallegro_main -lallegro_color -lallegro_font -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec

all : main
main : main.o chip8.o instructions.o screen.o
	gcc	main.o chip8.o instructions.o screen.o -o main $(LDFLAGS)

chip8.o: chip8.c chip8.h  instructions.h screen.h
	gcc -c chip8.c
instructions.o: instructions.c instructions.h
	gcc -c instructions.c
screen.o: screen.c screen.h
	gcc -c screen.c
clean:
	rm -f *.o a.out main TestGame
