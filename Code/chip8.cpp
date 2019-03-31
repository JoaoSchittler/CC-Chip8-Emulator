//Chip8.cpp
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "chip8.hpp"

namespace chip8
{
	
	void Chip8::init()
	{
		// Initialize registers and memory once
		opcode  = 0;
		pc  	= 0x200;
		game_size = 0;
		sp		= 0;
		index	= 0;
		sound_timer = 0;
		delay_timer = 0;
		int i;
	
		for(i=0;i<4096;i++)
		{
			if(i<16)
			{
				reg_v[i]=0;
				stack[i]=0;
				key[i]=0;
			} 
			if(i<2048)
			{
				gfx[i]=0;
			}
			memory[i] = 0;
		}
		//Load fontset
		for(i=0;i<80;i++)
		{
			memory[i]=fontset[i];
		}
	
	}
	void Chip8::loadGame(const char* str)
	{
		//Open the file in output mode and set the fp to the end 
		std::ifstream ROM (str,std::ios::out|std::ios::ate|std::ios::binary);
		if(ROM.is_open())
		{
			std::streampos size = ROM.tellg();//Using fp at the end of the file we can find its size
			char* aux = new char [size];//Allocate auxiliary memory
			//Get the contents of the file into aux
			ROM.seekg(0,std::ios::beg);
			ROM.read(aux,size);
			ROM.close();
			//Load aux into memory
			for(int i=0;i<=size;i++)
			{
				memory[i+0x200] = aux[i];
			}
			//Delete aux
			delete[] aux;
			std::cout << size << " Bytes loaded\n";
			game_size = size;
		}
		else
			std::cout << "Unable to open file\n";	
			
		
	}
	void Chip8::decode_execute_opcode()
	{

		unsigned char first_nibble = opcode>>12;
		pc = pc+2;	//Always increments pc
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

	void Chip8::emulate_cycle()
	{
 		// Fetch Opcode
		opcode = (memory[pc]<<8)|memory[pc+1];
		//decode_execute_opcode();
 		printf("0x%X\n",opcode);
 		pc+=2;//Temporary
 		
  		// Update timers
  		if(delay_timer>0)delay_timer--;
  		if(sound_timer>0)
		{
		  	if(sound_timer==1)
		  		std::cout << "BEEP\n";
		  	sound_timer--;	
		}

	}
	bool Chip8::draw_flag()
	{
		return drawflag;
	}
}


