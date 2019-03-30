//Chip8.hpp
#pragma once
namespace chip8
{
	struct Chip8 
	{
		unsigned short opcode;
		bool drawflag;	//if opcode ==  0x00E0  or 0xDXYN then True else False
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

		unsigned char gfx[64*32];
		unsigned short sound_timer;
		unsigned short delay_timer; // 60HZ

		unsigned short stack[16];
		unsigned short sp;

		unsigned char key[16];
	
		unsigned char fontset[80];
		

		void init();
		// chip8();
	
		void loadGame(const char* str );

		void emulate_cycle();

		bool draw_flag();
	
		void setKeys();

	};
};

