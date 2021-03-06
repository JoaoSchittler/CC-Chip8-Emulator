#include "assembler.h"

#define INS_NUM 34

struct line {
	char** words;
	int numwords;
};
struct label{
	unsigned short adress;
	char* name;
};
typedef struct line LINE;
typedef struct label LABEL;
typedef unsigned short ushort;
char 	checkfileformat(char* filename);
char* 	getoutputfilename(char* inputname);
LINE** 	getinstructionset();
char* 	getLine(FILE* file);
char* 	removeunneededspaces(char* string);
LINE* 	lineformat(char* lineraw);
void 	filLabellist(Fila* list,FILE* file);
ushort 	formbinaryinstruction(LINE* line,LINE** instructionset,Fila* labellist,int linenumber);

int stringtohex(char* c);
int islabel(LINE* line,int linenumber);
int isshorthex(LINE* line);
int toReg(char* arg);

void delline(LINE* line);
void delinstructionset(LINE** instructions);

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

	char* outputname = (char*)malloc((i+1)*sizeof(char));
	//strcpy(outputname,inputname);
	memcpy(outputname,inputname,i*sizeof(char));
	outputname[i] = '\0';
	char* extension = ".bin";
	strcat(outputname,extension);
	printf("%s\n",outputname);
	return outputname;
}

LINE** getinstructionset()
{
	//Traverses intructionset.txt and fills the instructionset array
	FILE* instructionfile = fopen("instructionset.txt","r");
	if(instructionfile==NULL)
	{
		printf("instructionset.txt file not found\n");
		exit(0);
	}

	LINE** instructionset = (LINE**)malloc(INS_NUM*sizeof(LINE*));
	for(int i = 0; i < INS_NUM; i++)
	{
		if ( feof(instructionfile) )
		{
			printf("instructionset.txt missing instructions\n");
			exit(0);
		}
		//Creates entry in instructionset array
		instructionset[i] = (LINE*)malloc(sizeof(LINE));
		instructionset[i]->numwords = 5;	//Max number of words a line has
		instructionset[i]->words = (char**)malloc(instructionset[i]->numwords*sizeof(char*));

		for(int j = 0; j < instructionset[i]->numwords; j++)
		{
			instructionset[i]->words[j] = (char*)malloc(6*sizeof(char));	//This "6" is the minimal size that this string has to be
			//Reads from the file and checks for a correct read
			int nsuccess = fscanf(instructionfile,"%s\t",instructionset[i]->words[j]);
			if (nsuccess != 1)
			{
				printf("error while reading line %d of instructionset.txt\n",i);
				exit(0);
			}	
		}
		
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
			cont++;
		else
			return NULL;
		
	}
	fseek(file,inipos,SEEK_SET);
	long int endpos = ftell(file);
	//Retorna a string do ponto atual do arquivo ate o \n ou EOF
	char* line = (char*) malloc (cont*sizeof(char));
	for(int i = 0; i < cont;i++){
		line[i] = getc(file);
		//printf("%d|",line[i]);
	}
	//printf("\n");
	line[cont-1] = '\0';
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

		if (string[cont] ==' ' || string[cont] =='\t')
		{
			if (cont == 0)
			{
				includechar[0] = 0;
				newsize--; 
				cont++;	
				continue;
			}
			if ( string[cont-1] == ' ' || string[cont-1] =='\t')
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
	if (string[lastinclude] == ' ' || string[lastinclude] == '\t')
		includechar[lastinclude] = 0;

	char* linenew = (char*)malloc(newsize*sizeof(char));
	for(int i = 0, j = 0; i < size;i++)
	{
		//printf("%d-",includechar[i]);
		if(includechar[i])
		{
			linenew[j] = string[i];	
			j++;
		}
	}
	//printf("\n");
	/*for(int i = 0; i < newsize;i++)
	{
		printf("%c|",linenew[i]);
	}*/
	//printf("\nOld Line = -%s- Size:%d\nNew Line = -%s- Size:%d\n",string,size,linenew,newsize);
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
		if (linenew[i]==' ' || linenew[i]=='\t')
			line->numwords++;
	}	
	line->words = (char**)malloc(line->numwords*sizeof(char*));
	
	int iniwordidx = 0;
	int wordidx = 0;
	for (int i = 0; i < strlen(linenew) + 1;i++)
	{
		if(linenew[i]==' ' || linenew[i] == '\0' || linenew[i]== '\t')
		{
			line->words[wordidx] = (char*)malloc((i-iniwordidx + 1)*sizeof(char));
			for(int j = iniwordidx; j < i; j++)
				line->words[wordidx][j - iniwordidx] = linenew[j];

			line->words[wordidx][i - iniwordidx] = '\0';
			iniwordidx = i+1;
			wordidx++;
		}
		
	}

	//free(linenew);

	return line;
}

void filLabellist(Fila* list,FILE* file)
{
	//Goes through the file and inserts any labels into list
	int linenumber = 1;
	int lineadress = 0x200;
	while (1)
	{
		char* lineraw = getLine(file);
		if (lineraw == NULL)
			break;
		if (lineraw[0] == '#'|| lineraw[0] == '\0')
			continue;

		LINE* line = lineformat(lineraw);
		free (lineraw);

		if ( islabel(line,linenumber))
		{
			LABEL* lb = (LABEL*)malloc(sizeof(LABEL));
			lb->adress = lineadress;
			lb->name = line->words[0];
			fila_insere(list,lb);
			continue;
		}
		else
			lineadress+=2;
	}
	fseek(file,0,SEEK_SET);	
}

unsigned short formbinaryinstruction(LINE* line,LINE** instructionset,Fila* labellist,int linenumber)
{
	printf("String is :");
	for (int i = 0; i < line->numwords; i++)
		printf("%s\t",line->words[i]);
	printf("\n");
	//Search for keyword match
	int insnumber = -1;
	for(int i = 0; i < INS_NUM; i++)
	{
		if ( ! strcmp(line->words[0],instructionset[i]->words[0]) )
		{
			insnumber = i;
			break;
		}
	}
	if(insnumber == -1)
	{
		printf("Invalid Keyword %s at line %d\n",line->words[0],linenumber);
		exit(0);
	}	
	//Attempt to generate instruction
	unsigned short instruction = 0;
	short aux = 0;
	int shift = 0;
	int argindex = line->numwords - 1;
	for (int i = instructionset[insnumber]->numwords -1; i > 0;i--)
	{
		if ( instructionset[insnumber]->words[i][0] == '-' )
		{
			continue;
		}
		if ( strlen(instructionset[insnumber]->words[i]) > 1  )
		{
			//Gets argument from line and adjusts shift
		
			if ( argindex < 0)
			{
					printf("Error in number of arguments (%d)\n",linenumber);
					exit(0);
			}	

			//REG-type argument, format is RX, X being the reg desired (0-F)
			if ( ! strcmp (instructionset[insnumber]->words[i],"REG") )
			{
				aux = toReg(line->words[argindex]);
				if (aux == -1)
				{
					printf("Invalid register argument %s, register arguments have the format 'RX', 0 =< X <= F (%d)\n",line->words[argindex],linenumber);
					exit(0);
				}
				aux = aux <<shift;
				shift+=4;
			}

			//BYTE-type argument
			if ( ! strcmp (instructionset[insnumber]->words[i],"BYTE") )
			{
				aux = stringtohex(line->words[argindex]);
				if (aux == -1)
				{
					printf("Error while converting %s to hex (%d)\n",line->words[argindex],linenumber);
					exit(0);
				}	
				//Convert string to BYTE
				if ( aux > 0xFF)
				{
					printf("%s doesn't fit in a BYTE\n",line->words[argindex]);
					exit(0);
				}
				aux = aux << shift;
				shift+=8;
			}
			//NIB-type argument
			if ( ! strcmp (instructionset[insnumber]->words[i],"NIB") )
			{
				aux = stringtohex(line->words[argindex]);
				if (aux == -1)
				{
					printf("Error while converting %s to hex (%d)\n",line->words[argindex],linenumber);
					exit(0);
				}	
				//Convert string to BYTE
				if ( aux > 0x0F)
				{
					printf("%s doesn't fit in a NIBBLE\n",line->words[argindex]);
					exit(0);
				}
				aux = aux << shift;
				shift+=4;
			}
			//LABL-type argument	
			if ( ! strcmp (instructionset[insnumber]->words[i],"LABL") )
			{
				aux = 0;
				if ( line->words[argindex][0] != '!' )
				{
					printf("Error, expected label in argument %s (%d)\n",line->words[argindex],linenumber);
					exit(0);
				}
				for (int j = 0; j < labellist->size;j++)
				{
					LABEL* lb = fila_peek(labellist,j);
					if ( ! strcmp(lb->name,line->words[argindex]))
					{
						aux = lb->adress;
						break;
					}
					
				}
				if ( aux == 0)
				{
					printf("Error, unknown label %s (%d)\n",line->words[argindex],linenumber);
					exit(0);

				}	
				//Check if valid label
				//Add label adress to "aux"	
				aux = aux << shift;
				shift+=12;
			}

			instruction+=aux;
			argindex--;
		}
		else	//Opcode-type argument
		{
			//I know for a fact that instructionset won't have any invalid hex numbers, so i can do this more directly
			//PepeLaugh
			if ( instructionset[insnumber]->words[i][0] >= '0' && instructionset[insnumber]->words[i][0] <= '9')
				aux = instructionset[insnumber]->words[i][0] - '0';
			else
				aux = instructionset[insnumber]->words[i][0] - 'A' + 10;

			aux = aux << shift;
			instruction+=aux;
			shift+=4;
		}

	}
	printf("Instruction is %x (%d)\n",instruction,linenumber);

	return instruction;
}

void filloutputfile(Fila* bins,FILE* file)
{
	while (bins->size != 0)
	{
		unsigned char* bin = fila_remove(bins);
		putc(*(bin+1),file);
		putc(*bin,file);
		printf("%x%x\n",*(bin+1),*bin);
		free((unsigned short*)bin);
	}
}
int  stringtohex(char* c)
{
	int length = strlen(c);
	if ( length <= 2)
	{
		printf("String too small\n");
		return -1;
	}
	if ( c[0] != '0' || c[1] != 'x')
	{
		printf("String prefix not '0x'\n");
		return -1;
	}
	unsigned short hex = 0;
	for(int i = 2; i < length;i++)
	{
		if ( c[i] >='0' && c[i] <='9')
		{
			hex+= (c[i]-'0')<<(4*(length-i-1));
			continue;
		}
		if ( c[i] >='A' && c[i] <='F')
		{
			hex+= (c[i]-'A'+10)<<(4*(length-i-1));
			continue;
		}
		printf("%c is not a hex value\n",c[i]);
		return -1;	

	}
	return hex;	

}
int islabel(LINE* line,int linenumber)
{
	if ( line->words[0][0] == '!')
	{
		if ( line->numwords == 1)
			return 1;

		printf("Error, too many words in label ");
		for (int i = 0; i < line->numwords; i++)
			printf("%s ",line->words[i]);
		printf("(%d)\n",linenumber);
		exit(0);
	}
	return 0;	
}
int isshorthex(LINE* line)
{
	int lenth = strlen(line->words[0]);
	if ( lenth >= 2 )
	{
		if (line->words[0][0] == '0' && line->words[0][1] == 'x')
		{
			int hex = stringtohex(line->words[0]);
			if (hex > 0xFFFF)
			{
				printf("Error, %d too big for unsigned short variable\n",hex);
				exit(0);			
			}
			return 1;	
		}
	}
	return 0;
}
int toReg(char* arg)
{
	if ( strlen(arg) == 2)
	{
		if ( arg[0] == 'R')
		{
			if ( arg[1] >= '0' && arg[1] <= '9')
				return arg[1] - '0';
			if ( arg[1] >= 'A' && arg[1] <= 'F')
				return arg[1] - 'A' + 10;
		}
	}	
	return -1;	
}

void delline(LINE* line)
{
	for(int i = 0; i < line->numwords;i++)
		free(line->words[i]);
	free(line->words);
	free(line);		
}


void delinstructionset(LINE** instructions)
{
	for(int i = 0; i < INS_NUM;i++)
		delline(instructions[i]);
	free(instructions);
}

void assemble(char* filename)
{
	FILE* inputfile = fopen(filename,"r");

	LINE** instructions = getinstructionset();
	if (instructions == NULL)	return;

	Fila* bininstructions = fila_cria();

	Fila* labellist = fila_cria();

	int linenumber = 1;
	int adress = 0x200;
	unsigned short* binins;

	filLabellist(labellist,inputfile);
	while (1)
	{
		char* lineraw = getLine(inputfile);
		if (lineraw == NULL)
			break;
		if (lineraw[0] == '#' ||  lineraw[0] == '!' || lineraw[0] == '\0')	//Comment, label or empty line
		{
			linenumber++;
			continue;
		}

		LINE* line = lineformat(lineraw);
		free (lineraw);	
		binins = (unsigned short*)malloc(sizeof(unsigned short));

		if (isshorthex(line))	//Mainly used for indexing sprites
			*binins = stringtohex(line->words[0]);
		else
			*binins = formbinaryinstruction(line,instructions,labellist,linenumber);
					
		fila_insere(bininstructions,binins); 

		delline(line);
		linenumber++;
		adress+=2;
		printf("/////////////////////////\n");
	}
	delinstructionset(instructions);
	fclose(inputfile);
	char* outputname = getoutputfilename(filename);
	if (outputname == NULL)		
		return;
	FILE* outputfile = fopen(outputname,"w");
	free(outputname);
	filloutputfile(bininstructions,outputfile);
	fila_deleta(bininstructions);
	fclose(outputfile);	
	return;
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
