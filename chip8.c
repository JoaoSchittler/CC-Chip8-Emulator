//Chip8.c
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "instructions.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>

void c8_emulate_cycle(struct Chip8* c8);
void c8_decode_execute_instruction(struct Chip8* c8);

//TODO - init fontset
void c8_init(struct Chip8* c8,char* doDebug)
{
	srand(time(NULL));
	// Initialize registers and memory 
	c8->currentinstruction  = 0;
	c8->drawflag = 0;

	if(doDebug[0] == '0' && doDebug[1] == '\0')
		c8->debug = 0;
	else
		c8->debug = 1;

	c8->pc      = 0x200;
	c8->game_size = 0;
	c8->sp	= 0;
	c8->index	= 0;
	c8->sound_timer = 0;
	c8->delay_timer = 0;
	int i;
	for(i=0;i<4096;i++)
	{
		if(i<16)
		{
			c8->regs[i]=0;
			c8->stack[i]=0;
			c8->key[i]=0;
		} 
		c8->memory[i] = 0;
	}
	//Load fontset
	unsigned char aux[80] = { 
	  		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  		0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0xF0, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    	};
	for(i=0;i<80;i++)
	{
		c8->fontset[i] = aux[i];
		c8->memory[i]=c8->fontset[i];
	}


	c8->scr = screen_init(800,600,"Chip-8 Emulator");
	
}
int c8_loadGame(const char* str,struct Chip8* c8)
{
	//Open the file in output mode and set the fp to the end 
	FILE* game_file = fopen(str,"r");
	if(game_file == NULL) {
		printf("File not found\n");
		return 0;
	}
	fseek(game_file, 0L, SEEK_END);
    int size = ftell(game_file); // Number of Bytes of file
    rewind(game_file);
	c8->game_size = size;

	for(int i=0;i<size;i++){
		int aux = getc(game_file);
		c8->memory[i+0x200] = (unsigned char)aux;
	}
	c8->index = 0x200 + size; //Init mem index

	printf("%d Bytes Loaded\n",size);
	fclose(game_file);
	return 1;
}
void printregs(BYTE* regs)
{
	for(int i = 0; i < 16; i++)
	{
		printf("Reg[%d]: %d\n",i,regs[i]);
	}
}
void printstacktrace(unsigned short * stack)
{
	for(int i = 0; i < 16; i++)
	{
		printf("Stack[%d]: %x\n",i,stack[i]);
	}	
}
void print_screen_matrix(BYTE** matrix)
{
	for(int l = 0; l < 32;l++)
	{
		for(int c = 0; c < 64;c++)
			printf("%d|",matrix[l][c]);
		printf("\n");
	}

}
void print_keys(BYTE* key)
{
	printf("[%d][%d][%d][%d]\n[%d][%d][%d][%d]\n[%d][%d][%d][%d]\n[%d][%d][%d][%d]\n"
		   ,key[1],key[2],key[3],key[0xC]
		   ,key[4],key[5],key[6],key[0xD]
		   ,key[7],key[8],key[9],key[0xE]
		   ,key[0xA],key[0],key[0xB],key[0xF]);
}
void c8_print_all_instructions(struct Chip8* c8)
{
	unsigned short pc = 0x200;
	while(pc <= c8->game_size + 0x200 + sizeof(unsigned short))
	{
		unsigned short ins = (c8->memory[pc]<< 8) | c8->memory[pc + 1];
		printf("%x -> %x\n",pc,ins);
		pc+=2;
	}	
}
void c8_play_game(struct Chip8* c8)
{

	while(1)
	{
		if(c8->pc > c8->game_size + 0x200 + sizeof(unsigned short)){ // Verifica se chegou ao fim das instruções
				return;
		}
		c8_emulate_cycle(c8);

		screen_manage_events(c8->scr,c8->key);
		screen_refresh(c8->scr);
		screen_wait(c8->scr,0.01666);

		if( c8->debug )
		{
			printf("%x\n",c8->currentinstruction);
			printregs(c8->regs);
			printf("\n\n");
			printf("I = %x\n",c8->index);
			printf("PC = %x\n",c8->pc);
			//printstacktrace(c8->stack);

			getchar(); // system pause
		}
		if (c8->drawflag == 3 && c8->debug)
		{
			print_keys(c8->key);
			c8->drawflag = 0;
		}	

		if(c8->drawflag == 2)
		{
			screen_clear_grid(c8->scr);
			//if (c8->debug)
			//	print_screen_matrix(c8->scr->screen_matrix);
			c8->drawflag = 0;
		}
		if(c8->drawflag == 1)
		{
			screen_alter_grid(c8->scr,c8->sprite_buffer.x,c8->sprite_buffer.y,c8->sprite_buffer.height,c8->regs,c8->memory,c8->index);
			//if (c8->debug)
			//	print_screen_matrix(c8->scr->screen_matrix);
			c8->drawflag = 0;
		}


	}

}

void c8_emulate_cycle(struct Chip8* c8)
{	
	// Fetch Instruction
	c8->currentinstruction =  (c8->memory[c8->pc]<< 8) | c8->memory[c8->pc + 1];	
	c8->pc = c8->pc+2;	//Always increments pc
	c8_decode_execute_instruction(c8);
  	// Update timers
  	if(c8->delay_timer>0)
  		c8->delay_timer--;
  	if(c8->sound_timer>0)
	{
		if(c8->sound_timer==1)
		  	printf("Beep"); // Play beep sound
	
		c8->sound_timer--;	
	}

}

void c8_decode_execute_instruction(struct Chip8* c8)
{
	unsigned short first_nibble  = (c8->currentinstruction & 0xF000) >> 12;
	unsigned short second_nibble = (c8->currentinstruction & 0x0F00) >> 8;
	unsigned short third_nibble  = (c8->currentinstruction & 0x00F0) >> 4;
	unsigned short fourth_nibble = (c8->currentinstruction & 0x000F)     ;
	unsigned char key = 0;
	int invalid_ins = 0;
	switch(first_nibble)
	{
		case 0x0: 	switch(fourth_nibble){

						case 0x0:
								clear_screen(&(c8->drawflag));
								break;
						case 0xE:
								subroutine_return(c8->stack,&(c8->sp),&(c8->pc));
								break;
						default:
								invalid_ins = 1;				
					}	
					break;

		case 0x1: 	go_to(c8->currentinstruction & 0x0FFF,&(c8->pc));
					break;

		case 0x2:	subroutine_go(c8->stack,&(c8->sp),&(c8->pc),c8->currentinstruction & 0x0FFF);

					break;

		case 0x3:	skip_eq_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->regs,&(c8->pc));
					break;

		case 0x4:	skip_ne_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->regs,&(c8->pc));
					break;

		case 0x5:	skip_eq_reg(second_nibble,third_nibble,c8->regs,&(c8->pc));
					break;

		case 0x6: 	load_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->regs);
					break;

		case 0x7: 	add_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->regs);
					break;

		case 0x8:	switch(fourth_nibble){

						case 0x0:
								move_reg(second_nibble,third_nibble,c8->regs);
								break;
						case 0x1:
								or_reg(second_nibble,third_nibble,c8->regs);
								break;
						case 0x2:
								and_reg(second_nibble,third_nibble,c8->regs);
								break;
						case 0x3:
								xor_reg(second_nibble,third_nibble,c8->regs);
								break;
						case 0x4:
								add_reg(second_nibble,third_nibble,c8->regs);
								break;
						case 0x5:
								sub_reg1(second_nibble,third_nibble,c8->regs);
								break;	
						case 0x6:
								store_lsb_shiftl(second_nibble,c8->regs);
								break;
						case 0x7:
								sub_reg2(second_nibble,third_nibble,c8->regs);
								break;	
						case 0xE:
								store_msb_shiftr(second_nibble,c8->regs);
								break;					
						default:
								invalid_ins = 1;				
					}	
					break;

		case 0x9:	skip_ne_reg(second_nibble,third_nibble,c8->regs,&(c8->pc));
					break;

		case 0xA:	load_address(&(c8->index),c8->currentinstruction & 0x0FFF);
					break;

		case 0xB:	goto_imm(c8->currentinstruction & 0x0FFF,c8->regs,&(c8->pc));
					break;	

		case 0xC:	generate_mask(c8->currentinstruction & 0x00FF,c8->regs,second_nibble);
					break;

		case 0xD:
				{
					c8->sprite_buffer.x = c8->regs[second_nibble];
					c8->sprite_buffer.y = c8->regs[third_nibble];
					c8->sprite_buffer.height = fourth_nibble;
					c8->drawflag = 1;
					//Opcode 0xDXYN
					break;
				}
				

		case 0xE:	switch(fourth_nibble){

						case 0xE:
								skip_if_key_press(c8->key,second_nibble,&(c8->pc),c8->regs);
								c8->drawflag = 3;
								break;
						
						case 0x1:
								skip_if_key_not_pressed(c8->key,second_nibble,&(c8->pc),c8->regs);
								c8->drawflag = 3;
								break;
						default:
								invalid_ins = 1;				
					}	
					break;

		case 0xF:	switch(third_nibble){

						case 0x0:
								switch(fourth_nibble){

									case 0x7:
												get_dtimer(second_nibble,c8->regs,c8->delay_timer);
												break;
						
									case 0xA:
												get_key(second_nibble,c8->regs,screen_getinput(c8->scr));
												break;
									default:
										invalid_ins = 1;				
								}	
								break;
						case 0x1:
								switch(fourth_nibble){

									case 0x5:
												set_dtimer(second_nibble,c8->regs,&(c8->delay_timer));
												break;
						
									case 0x8:
												set_stimer(second_nibble,c8->regs,&(c8->sound_timer));
												break;
									case 0xE:
												set_i(second_nibble,c8->regs,&(c8->index));
												break;
									default:
										invalid_ins = 1;				
								}	
								break;
						case 0x2:
								//load_sprite_address();
								break;
						case 0x3:
								BCD_convert(second_nibble,c8->regs,c8->index,c8->memory);
								break;
						case 0x5:
								dump_regs(second_nibble,c8->regs,c8->index,c8->memory);
								break;
						case 0x6:
								load_regs(second_nibble,c8->regs,c8->index,c8->memory);
								break;					
						default:
								invalid_ins = 1;				
					}	
					break;

		default:
				invalid_ins = 1;

	}
	if(invalid_ins)
		printf("Unknown instruction %x\n",c8->currentinstruction);


}

/*

Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+

*/
void c8_finalize(struct Chip8* c8)
{
	screen_delete(c8->scr);
}
