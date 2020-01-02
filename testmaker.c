#include <stdio.h>
#define ins unsigned short
int main()
{
	ins Instructions[] = 
	{
		//0xFF61,
		//0xFF60,
		//0x1482,
		//0x0482,
		0x0AF5,
		0x0012

	};
	int n = 2;

	FILE* fp = fopen("TestGame","wb");

	fwrite(Instructions,sizeof(ins),n,fp);

	fclose(fp);	
}

//Opcode 0xFX0A 