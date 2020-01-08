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
		0xE000


	};
	int n = 6;

	FILE* fp = fopen("TestGame","wb");

	fwrite(Instructions,sizeof(ins),n,fp);

	fclose(fp);	
}
