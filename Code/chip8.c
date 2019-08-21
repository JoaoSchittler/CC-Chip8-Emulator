//Chip8.c
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include <stdbool.h>
void c8_init(struct Chip8 c8)
{
	// Initialize registers and memory once
	c8.opcode  = 0;
	c8.pc  	= 0x200;
	c8.game_size = 0;
	c8.sp	= 0;
	c8.index	= 0;
	c8.sound_timer = 0;
	c8.delay_timer = 0;
	int i;
	for(i=0;i<4096;i++)
	{
		if(i<16)
		{
			c8.reg_v[i]=0;
			c8.stack[i]=0;
			c8.key[i]=0;
		} 
		if(i<2048)
		{
			c8.gfx[i]=0;
		}
		c8.memory[i] = 0;
	}
	//Load fontset
	for(i=0;i<80;i++)
	{
		c8.memory[i]=c8.fontset[i];
	}
	
}
void c8_loadGame(const char* str,struct Chip8 c8)
{
	//Open the file in output mode and set the fp to the end 
	std::ifstream ROM (str,std::ios::out|std::ios::ate|std::ios::binary);
	if(ROM.is_open())
	{
		std::streampos size = ROM.tellg();//Using fp at the end of the file we can find its size
		char* aux = (char*)malloc(size*sizeof(char));//Allocate auxiliary memory
		//Get the contents of the file into aux
		ROM.seekg(0,std::ios::beg);
		ROM.read(aux,size);
		ROM.close();
		//Load aux into memory
		for(int i=0;i<=size;i++)
		{
			c8.memory[i+0x200] = aux[i];
		}
		//Delete aux
		free(aux);
		std::cout << size << " Bytes loaded\n";
		c8.game_size = size;
	}
	else
		std::cout << "Unable to open file\n";	
				
}
void c8_decode_execute_opcode(struct Chip8 c8)
{
	unsigned char first_nibble = opcode>>12;
	c8.pc = c8.pc+2;	//Always increments pc
	switch(first_nibble)
	{
		case 0x0: 
				break;
		case 0x1: 
				break;
		case 0x2:
				break;
		case 0x3:
				break;
		case 0x4:
				break;
		case 0x5:
				break;
		case 0x6: 
				break;
		case 0x7: 
				break;
		case 0x8:
				break;
		case 0x9:
				break;
		case 0xA:
				break;
		case 0xB:
				break;	
		case 0xC:
				break;
		case 0xD:
				break;
		case 0xE:
				break;
		case 0xF:
				break;
		default:
				printf("Unknown opcode : 0x%X\n",opcode);			
		}
}

void c8_emulate_cycle(struct Chip8 c8)
{	
	// Fetch Opcode
	c8.opcode = (c8.memory[pc]<<8)|c8.memory[pc+1];
	//decode_execute_opcode();
	printf("0x%X\n",c8.opcode);
	c8.pc+=2;//Temporary
 		
  	// Update timers
  	if(c8.delay_timer>0)c8.delay_timer--;
  	if(c8.sound_timer>0)
	{
		if(c8.sound_timer==1)
		  	std::cout << "BEEP\n"; // Play beep sound
		c8.sound_timer--;	
	}

}
bool c8_draw_flag(struct Chip8 c8)
{
	return c8.drawflag;
}



