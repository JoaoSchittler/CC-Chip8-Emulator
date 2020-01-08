#include "chip8.h"
#include <stdio.h>

int main(int argc, char** argv)
{

	chip8 processor;
	if(argc > 2)
		c8_init(&processor,argv[2]);
	else
		c8_init(&processor,"0");
	if(argc > 1)
	{
		c8_loadGame(argv[1],&processor);
	}
	else
	{
		printf("No Rom detected\n");
		return 0;
	}
	//c8_print_all_instructions(&processor);
	c8_play_game(&processor);
	c8_finalize(&processor);
	return 0;
	
}
