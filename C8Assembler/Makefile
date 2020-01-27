all : main
main : main.o assembler.o fila.o
	gcc	main.o assembler.o fila.o -o main 
assembler.o: assembler.c assembler.h fila.h
	gcc	-c	assembler.c 
clean:
	rm -f *.o main
