#include "chip8.h"
#include <stdio.h>
// gcc -o main main.c chip8.c chip8.h screen.c screen.h instructions.h -lallegro -lallegro_main -lallegro_color -lallegro_font -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec
//Status
// All instructions are being read and stored Correctly
//TODO =Init fontset
int main()
{
	chip8 processor;
	c8_init(&processor);
	c8_loadGame("PONG",&processor);
	c8_play_game(&processor);
	c8_finalize(&processor);
	return 0;
	
}
