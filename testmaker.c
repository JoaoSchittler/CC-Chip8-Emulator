#include <stdio.h>
#define ins unsigned short
int main()
{
	ins Instructions[] = 
	{
		0x8EE0,
		0x0012,
		0x8EE1,
		0x0412,
		0x8EE2,
		0x0812

	};
	int n = 6;

	FILE* fp = fopen("TestGame","wb");

	fwrite(Instructions,sizeof(ins),n,fp);

	fclose(fp);	
}
//0xEX8E
//0x1NNN
