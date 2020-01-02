#include <stdio.h>
#define ins unsigned short
int main()
{
	ins Instructions[] = 
	{
		0x0460,
		0x0561,
		0x1482,
		0x0482

	};
	//0x8XY4 X <- Y + X , if Carry out then F = 1 else F = 0
	int n = 4;

	FILE* fp = fopen("TestGame","wb");

	fwrite(Instructions,sizeof(ins),n,fp);

	fclose(fp);	
}
