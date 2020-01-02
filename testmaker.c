#include <stdio.h>
#define ins unsigned short
int main()
{
	ins Instructions[] = 
	{
		0x3C60, 
		0xC361,
		0xFF62,
		0x55F2,
		0x43D5,
		0XE000

	};
	int n = 6;

	FILE* fp = fopen("TestGame","wb");

	fwrite(Instructions,sizeof(ins),n,fp);

	fclose(fp);	
}

//Opcode 0xFX0A 
/*
	0x3C   00111100     ****
	0xC3   11000011   **    **
	0xFF   11111111   ********
0xDXYN Draw Sprite at X,Y with height N starting at address I, F = 1 if any pixels are changed else F = 0
0xFX55 store
0x6XNN	 X <- NN	
*/