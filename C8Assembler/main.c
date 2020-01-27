#include <stdio.h>
#include "assembler.h"
int main(int argc,char** args)
{
	if ( argc == 1 )
	{
		printf("No source file found\n");
		return 0;
	}
	assemble(args[1]);
}
