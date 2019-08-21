#include "Chip8.hpp"
#include <stdio.h>
int main()
{
	chip8 processor;
	c8_init(processor);
	c8_loadgame("ROMS/PONG",processor);
	int ins_count = 0;
	while(processor.pc <= processor.game_size + 0x200)
	{
		c8_emulate_cycle(processor);
		ins_count++;
	}
	printf("Total instructions = %d \n",ins_count);
	
}
