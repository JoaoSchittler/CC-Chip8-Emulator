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
	char* outputname = (char*)malloc((i+5)*sizeof(char));
	memcpy(outputname,inputname,i*sizeof(char));
	outputname[i+4] = '\0';
	printf("%s\n",outputname);
	char* extension = ".bin";
	char* a = strcat(outputname,extension);

	return outputname;
}
INS** getinstructionset()
{
	INS** instructionset = (INS**)malloc(INS_NUM*sizeof(INS*));
	FILE* instructionfile = fopen("instructionset.txt","r");
	if(instructionfile==NULL)
	{
		printf("instructionset.txt file not found\n");
		exit(0);
	}
	for(int i = 0; i < INS_NUM; i++)
	{
		if ( feof(instructionfile) )
		{
			printf("instructionset.txt missing instructions\n");
			exit(0);
		}
		
		INS* instruction = (INS*)malloc(sizeof(INS));
		instruction->keyword = (char*)malloc(10*sizeof(char));
		char keyword[10];
		int numargs;
		int nsuccess = fscanf(instructionfile,"%s\t%d\n",keyword,&numargs);
		if(nsuccess != 2)
		{
			printf("error while reading instructionset.txt\n");
			exit(0);
		}
		strcpy(instruction->keyword,keyword);
		instruction->numargs = numargs;
		instructionset[i] = instruction;
	}
	return instructionset;
}
char* getLine(FILE* file)
{
	//Acha o proximo \n ou EOF
	long int inipos = ftell(file);
	int cont = 0;
	char aux = '\0';
	while(aux != '\n' && aux != EOF)
	{
		if ( (aux = getc(file)) != EOF)
		{
			//printf("Read %d\n",aux);
			cont++;
		}
		else
			return NULL;
		
	}
	//printf("Cont = %d\n",cont);
	fseek(file,inipos,SEEK_SET);
	long int endpos = ftell(file);
	//Retorna a string do ponto atual do arquivo ate o \n ou EOF
	char* line = (char*) malloc (cont*sizeof(char));
	for(int i = 0; i < cont;i++){
		line[i] = getc(file);
		//printf("wrote %d\n",line[i]);
	}
	line[cont-1] = '\0';
	printf("Final String :%s\n",line);
	return line;	
}
char* removeunneededspaces(char* string)
{
	int size = strlen(string) + 1;
	int newsize = size;
	int cont = 0;
	int lastinclude = 0;
	char* includechar = (char*)malloc(size*sizeof(char));
	includechar[size-1]=1;
	while (string[cont]!='\0')
	{
		includechar[cont] = 1;

		if (string[cont] ==' ')
		{
			if (cont == 0)
			{
				includechar[0] = 0;
				newsize--; 
				cont++;	
				continue;
			}
			if ( string[cont-1] == ' ')
			{
				includechar[cont] = 0;
				newsize--;
			}
			if ( string[cont+1] == '\0')
			{
				includechar[cont] = 0;
				newsize--; 
				break;
			}
			if ( includechar[cont] == 1)
				lastinclude = cont;
		}
		else
			lastinclude = cont;

		cont++;	
	}
	if (string[lastinclude] == ' ')
		includechar[lastinclude] = 0;

	char* linenew = (char*)malloc(newsize*sizeof(char));
	for(int i = 0, j = 0; i < size;i++)
	{
		printf("%d-",includechar[i]);
		if(includechar[i])
		{
			linenew[j] = string[i];	
			j++;
		}
	}
	printf("\n");
	for(int i = 0; i < newsize;i++)
	{
		printf("%d|",linenew[i]);
	}	
	printf("\nOld Line = -%s- Size:%d\nNew Line = -%s- Size:%d\n",string,size,linenew,newsize);
	free(includechar);

	return linenew;
}
LINE* lineformat(char* lineraw)
{
	LINE* line = (LINE*)malloc(sizeof(LINE));
	line->numwords = 1;
	//Removes unecessary spaces
	char* linenew = removeunneededspaces(lineraw);
	for(int i = 0; i < strlen(linenew);i++)
	{
		if (linenew[i]==' ')
			line->numwords++;
	}	
	line->words = (char**)malloc(line->numwords*sizeof(char*));
	printf("Line has %d Words, Total Length is %d\n",line->numwords,strlen(linenew) + 1);
	
	int iniwordidx = 0;
	int wordidx = 0;
	for (int i = 0; i < strlen(linenew) + 1;i++)
	{
		printf("L[%d] = %c\n",i,linenew[i]);
		if(linenew[i]==' ' || linenew[i] == '\0')
		{
			printf("pp\n");
			line->words[wordidx] = (char*)malloc((i-iniwordidx + 1)*sizeof(char));
			for(int j = iniwordidx; j < i; j++)
			{
				line->words[wordidx][j - iniwordidx] = linenew[j];
			}
			line->words[wordidx][i - iniwordidx] = '\0';
			printf("Word %d is -%s-\n",wordidx,line->words[wordidx]);
			iniwordidx = i+1;
			wordidx++;
		}
		
	}

	//free(linenew);

	return line;
}
int formbinaryinstruction(LINE* line)
{
	return 1;
}
void delinstructionset(INS** instructions)
{
	for(int i = 0; i < INS_NUM;i++)
	{
		free(instructions[i]->keyword);
		free(instructions[i]);
	}
	free(instructions);
}
void delLine(LINE* line)
{
	for(int i = 0; i < line->numwords;i++)
		free(line->words[i]);
	free(line->words);
	free(line);		
}

void assemble(char* filename)
{
	FILE* inputfile = fopen(filename,"r");

	char* outputname = getoutputfilename(filename);
	if (outputname == NULL)		return;
	FILE* outputfile = fopen(outputname,"w");
	free(outputname);

	INS** instructions = getinstructionset();
	if (instructions == NULL)	return;
	int linenumber = 1;
	while (1)
	{
		char* lineraw = getLine(inputfile);
		if (lineraw == NULL)
		{
			printf("Finished\n");
			break;
		}	
		printf("Read Line %d\n",linenumber);

		LINE* line = lineformat(lineraw);
		free (lineraw);	
		for (int i = 0; i < line->numwords; i++)
				printf("Word %d is -%s-\n",i,line->words[i]);

		//delLine(line);
		//if( !formbinaryinstruction(line) )	return;

		linenumber++;
	}
	delinstructionset(instructions);
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
