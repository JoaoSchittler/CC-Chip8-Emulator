#include "chip8.h"
#include <stdio.h>
// gcc -o main main.c chip8.c chip8.h screen.c screen.h instructions.h -lallegro -lallegro_main -lallegro_color -lallegro_font -lallegro_primitives -lallegro_image -lallegro_audio -lallegro_acodec
//Status
// All instructions are being read and stored Correctly
//TODO =Init fontset
int main()
{
	chip8 processor;
	c8_init(&processor,true);
	c8_loadGame("roms/Tetris [Fran Dachille, 1991].ch8",&processor);
	//c8_print_all_instructions(&processor);
	c8_play_game(&processor);
	c8_finalize(&processor);
	return 0;
	
}
/*

PONG
0x6A02	- Reg 10 = 0x02
0x6B0C	- Reg 11 = 0x0C
0x6C3F	- Reg 12 = 0x3F
0x6D0C  - Reg 13 = 0x0C
0xA2EA  - Index  = 0x2EA
0xDAB6  - Draw at 10 12 6 Height
0xDCD6  - Draw at 13 14 6 Height
0x6E00  - Reg 14 = 0x00
0x22D4	- PC     = 0x2D4

*/