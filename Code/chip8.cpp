//Chip8.cpp
#include <fstream>
#include <iostream>
#include "chip8.hpp"


namespace chip8
{
	
	void Chip8::init()
	{
		// Initialize registers and memory once
		opcode  = 0;
		pc  	= 0x200;
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
		std::ifstream ROM (str,std::ios::out|std::ios::ate);
		if(ROM.is_open())
		{
			std::streampos size = ROM.tellg();//Using fp at the end of the file we can find its size
			char* aux = new char [size];//Allocate auxiliary memory
			//Get the contents of the file into aux
			ROM.seekg(0,std::ios::beg);
			ROM.read(aux,size);
			ROM.close();
			//Load aux into memory
			for(int i=0;i<size;i++)
			{
				memory[i+0x200] = aux[i];
			}
			//Delete aux
			delete[] aux;
			std::cout << size << " Bytes loaded\n";
		}
		else
			std::cout << "Unable to open file";	
	
	}
	void Chip8::emulate_cycle()
	{
 		// Fetch Opcode
	
  		// Decode Opcode

  		// Execute Opcode
 
  		// Update timers

	}
	bool Chip8::draw_flag()
	{
		return drawflag;
	}
}


