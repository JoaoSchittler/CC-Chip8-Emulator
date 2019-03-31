#include "Chip8.hpp"
#include <stdio.h>
int main()
{
	chip8::Chip8 processor;
	processor.init();
	processor.loadGame("ROMS/PONG");
	int ins_count = 0;
	while(processor.pc <= processor.game_size + 0x200)
	{
		processor.emulate_cycle();
		ins_count++;
	}
	printf("Total instructions = %d \n",ins_count);
	
}
