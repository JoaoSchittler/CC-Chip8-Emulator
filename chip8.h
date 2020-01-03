//Chip8.h
#include <stdio.h>
#include "screen.h"
typedef struct sprite_data{
	unsigned char x;
	unsigned char y;
	unsigned char height;
} sprite;
typedef struct Chip8 
{
		unsigned short currentinstruction;
		sprite sprite_buffer;
		int drawflag;
		int debug;
		/*
	 	Memory map:
			0x000-0x1FF (Font set)
			0x050-0xA0  (For the 4x5 Font set)
			0x200-0xFFF (ROM e RAM)	
	
		*/
		unsigned char memory[4096];//Memória de 4k bytes	
		unsigned char regs[16]; // Banco de registradores (V0 -> VF)
		unsigned short index;
		unsigned short pc;
		unsigned int game_size;
		unsigned short sound_timer; // 60HZ
		unsigned short delay_timer; // 60HZ
		unsigned short stack[16];
		unsigned short sp;
		unsigned char key[16];
		unsigned char fontset[80];

		screen_info* scr;

} chip8;
void c8_init(struct Chip8* c8,int doDebug);
void c8_print_all_instructions(struct Chip8* c8);
void c8_play_game(struct Chip8* c8);
int c8_loadGame(const char* str,struct Chip8* c8);

void c8_finalize(struct Chip8* c8);