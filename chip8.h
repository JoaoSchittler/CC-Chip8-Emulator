//Chip8.h
#include <stdio.h>
#include "screen.h"

//#include "instructions.cpp"
typedef struct Chip8 
{
		unsigned short currentinstruction;
		int drawflag;	//if opcode ==  0x00E0  or 0xDXYN then True else False
		int get_keyboard_input;
		/*
	 	Memory map:
			0x000-0x1FF (Font set)
			0x050-0xA0  (For the 4x5 Font set)
			0x200-0xFFF (ROM e RAM)	
	
		*/
		unsigned char memory[4096];//Memória de 4k bytes	
		unsigned char reg_v[16]; // Banco de registradores (V0 -> VF)
		unsigned short index;
		unsigned short pc;
		unsigned int game_size;
		unsigned char gfx[64*32];
		unsigned short sound_timer; // 60HZ
		unsigned short delay_timer; // 60HZ
		unsigned short stack[16];
		unsigned short sp;
		unsigned char key[16];
		unsigned char fontset[80];

		screen_info* scr;

} chip8;
void c8_init(struct Chip8* c8);

void c8_play_game(struct Chip8* c8);
int c8_loadGame(const char* str,struct Chip8* c8);
void c8_emulate_cycle(struct Chip8* c8);
void c8_decode_execute_instruction(struct Chip8* c8);
void c8_setKeys(struct Chip8* c8);

void c8_finalize(struct Chip8* c8);