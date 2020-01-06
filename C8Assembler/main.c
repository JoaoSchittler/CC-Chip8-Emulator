#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INS_NUM 34
struct line {
	char** words;
	int numwords;
};
struct instruction {
	char* keyword;
	int numargs;
};
typedef struct line LINE;
typedef struct instruction INS;

char checkfileformat(char* filename)
{
	int length = strlen(filename);
	printf("File name = %s\nHas length %d\n",filename,length);
	
	return 0;
}
char* getoutputfilename(char* inputname)
{
	int i = 0;
	while (inputname[i]!='\0')
	{
		if (inputname[i]== '.')
			break;
		i++;
	}
	if (inputname[i]=='\0')
	{
		printf("Invalid file format, file has no type\n");
		exit(0);
	}
	char* outputnameaux = (char*)malloc((i+1)*sizeof(char));
	memcpy(outputnameaux,inputname,i*sizeof(char));
	outputnameaux[i] = '\0';
	char* outputnamefinal = strcat(outputnameaux,".bin");
	
	printf("Input : -%s-\nOutput: -%s-\n",inputname,outputnamefinal);
	
	return outputnamefinal;
}
INS** getinstructionset()
{
	INS** instructionset = (INS**)malloc(INS_NUM*sizeof(INS*));
	FILE* instructionfile = fopen("instructionset.txt","r");
	printf("File Opened\n");
	if(instructionfile==NULL)
	{
		printf("instructionset.txt file not found\n");
		exit(0);
	}
	for(int i = 0; i < INS_NUM; i++)
	{
		printf("Line %d\n",i);
		if ( feof(instructionfile) )
		{
			printf("instructionset.txt missing instructions\n");
			exit(0);
		}
		
		INS* instruction = (INS*)malloc(sizeof(INS));
		printf("Allocated space\n");
		char keyword[10];
		int numargs;
		printf("Goind to read from file\n");
		int nsuccess = fscanf(instructionfile,"%s\t%d\n",keyword,&numargs);
		if(nsuccess != 2)
		{
			printf("error while reading instructionset.txt\n");
			exit(0);
		}
		strcpy(instruction->keyword,keyword);
		instruction->numargs = numargs;
		printf("Keyword = %s\tNargs = %d\n",instruction->keyword,instruction->numargs);
		instructionset[i] = instruction;
	}
	return instructionset;
}
char* getline(FILE* file, int* status)
{
	//Acha o proximo \n ou EOF
	long int inipos = ftell(file);
	int cont = 0;
	char aux;
	do
	{
		if ( ! feof(file) )
		{
			aux = getc(file);
			cont++;
		}
		else
		{
			*status = 1;
			break;
		}
		
	}while(aux != '\n');
	fseek(file,inipos,SEEK_SET);
	long int endpos = ftell(file);
	
	//Retorna a string do ponto atual do arquivo ate o \n ou EOF
	char* line = (char*) malloc (cont*sizeof(char));
	for(int i = 0; i < cont;i++)
		line[i] = getc(file);
	line[cont-1] = '\0';
	
	return line;	
}
LINE* lineformat(char* lineraw)
{
	LINE* line = (LINE*)malloc(sizeof(LINE));
	line->numwords = 1;
	//Removes unecessary spaces
	int size = strlen(lineraw) + 1;
	int newsize = size;
	int cont = 0;
	char* includechar = (char*)malloc(size*sizeof(char));
	includechar[size-1]=1;
	while (lineraw[cont]!='\0')
	{
		
		if (lineraw[cont]!=' ')
			includechar[cont] = 1;
		else
		{
			if (cont == 0)
			{
				includechar[0] = 0;
				newsize--; 
				cont++;	
				continue;
			}
			if (cont == size-2)
			{
				includechar[size-2] = 0;
				newsize--; 
				cont++;	
				continue;
			}
			if ( lineraw[cont-1] != ' ' && lineraw[cont+1]!=' ')
			{
				includechar[cont] = 1;	
				line->numwords++;
			}						
		}
		cont++;	
	}
	char* linenew = (char*)malloc(newsize*sizeof(char));
	for(int i = 0, j = 0; i < size;i++)
	{
		//printf("%d-",includechar[i]);
		if(includechar[i])
		{
			linenew[j] = lineraw[i];	
			j++;
		}
	}
	//printf("\n");
//	printf("Old Line = -%s- Size:%d\nNew Line = -%s- Size:%d\n",lineraw,size,linenew,newsize);
	free(lineraw);
	line->words = (char**)malloc(line->numwords*sizeof(char*));
	printf("Line has %d Words\n",line->numwords);
	
	int iniwordidx = 0;
	int wordidx = 0;
	for (int i = 0; i < newsize;i++)
	{
		//printf("L[%d] = %c\n",i,linenew[i]);
		if(linenew[i]==' ' || linenew[i] == '\0')
		{
			line->words[wordidx] = (char*)malloc((i-iniwordidx + 1)*sizeof(char));
			for(int j = iniwordidx; j < i; j++)
			{
				line->words[wordidx][j - iniwordidx] = linenew[j];
			}
			line->words[i - iniwordidx] = '\0';
			printf("Word %d is -%s-\n",wordidx,line->words[wordidx]);
			iniwordidx = i+1;
			wordidx++;
		}
		
	}	
	free(linenew);
	return line;
}
int formbinaryinstruction(LINE* line)
{
	return 1;
}

void assemble(char* filename)
{
	FILE* inputfile = fopen(filename,"r");
	char* outputname = getoutputfilename(filename);
	if (outputname == NULL)		return;
	FILE* outputfile = fopen(outputname,"w");
	INS** instructions = getinstructionset();
	if (instructions == NULL)	return;
	int done = 0;
	//do
//	{
		char* lineraw = getline(inputfile,&done);
		LINE* line = lineformat(lineraw);		
		//free(line);
		if( !formbinaryinstruction(line) )
			return;
//	}while (!done);

	fclose(inputfile);
	fclose(outputfile);	
	return;
}
int main(int argc,char** args)
{
	if ( argc == 1 )
	{
		printf("No source file found\n");
		return 0;
	}
	assemble(args[1]);
}
/*
0nnn - SYS addr
Jump to a machine code routine at nnn.

This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.


00E0 - CLS
Clear the display.


00EE - RET
Return from a subroutine.

The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.


1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.


2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.


3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.


4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.


5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.


6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.


7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx.

8xy0 - LD Vx, Vy
Set Vx = Vy.

Stores the value of register Vy in register Vx.


8xy1 - OR Vx, Vy
Set Vx = Vx OR Vy.

Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.


8xy2 - AND Vx, Vy
Set Vx = Vx AND Vy.

Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.


8xy3 - XOR Vx, Vy
Set Vx = Vx XOR Vy.

Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.


8xy4 - ADD Vx, Vy
Set Vx = Vx + Vy, set VF = carry.

The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.


8xy5 - SUB Vx, Vy
Set Vx = Vx - Vy, set VF = NOT borrow.

If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.


8xy6 - SHR Vx {, Vy}
Set Vx = Vx SHR 1.

If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.


8xy7 - SUBN Vx, Vy
Set Vx = Vy - Vx, set VF = NOT borrow.

If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.


8xyE - SHL Vx {, Vy}
Set Vx = Vx SHL 1.

If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.


9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.

The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.


Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.


Bnnn - JP V0, addr
Jump to location nnn + V0.

The program counter is set to nnn plus the value of V0.


Cxkk - RND Vx, byte
Set Vx = random byte AND kk.

The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.


Dxyn - DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.


Ex9E - SKP Vx
Skip next instruction if key with the value of Vx is pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.


ExA1 - SKNP Vx
Skip next instruction if key with the value of Vx is not pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.


Fx07 - LD Vx, DT
Set Vx = delay timer value.

The value of DT is placed into Vx.


Fx0A - LD Vx, K
Wait for a key press, store the value of the key in Vx.

All execution stops until a key is pressed, then the value of that key is stored in Vx.


Fx15 - LD DT, Vx
Set delay timer = Vx.

DT is set equal to the value of Vx.


Fx18 - LD ST, Vx
Set sound timer = Vx.

ST is set equal to the value of Vx.


Fx1E - ADD I, Vx
Set I = I + Vx.

The values of I and Vx are added, and the results are stored in I.


Fx29 - LD F, Vx
Set I = location of sprite for digit Vx.

The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.


Fx33 - LD B, Vx
Store BCD representation of Vx in memory locations I, I+1, and I+2.

The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.


Fx55 - LD [I], Vx
Store registers V0 through Vx in memory starting at location I.

The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.


Fx65 - LD Vx, [I]
Read registers V0 through Vx from memory starting at location I.

The interpreter reads values from memory starting at location I into registers V0 through Vx.
*/
