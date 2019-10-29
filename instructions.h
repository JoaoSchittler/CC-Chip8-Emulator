#ifndef _INS_H_
#define _INS_H_
#include <stdio.h>
#include <stdlib.h>
#define BYTE unsigned char
#define BYTE_2 unsigned short
//Instructions.c
//Opcode 0x00E0
void clear_screen(int* flag)
{
	*flag = 2;
}
//Opcode 0x00EE
void subroutine_return(unsigned short * stack, unsigned short* sp, BYTE_2* pc)
{
	*(pc) = stack[*sp];
	*(sp)--;
	if(*sp < 0 )
	{
		printf("Stack UnderFlow!!!!!!\n");
		exit(0);
	}	
}
//Opcode 0x1NNN
void go_to(BYTE_2 adress,BYTE_2* pc)
{
	*pc = adress;
}
//Opcode 0x2NNN
void subroutine_go(unsigned short * stack, unsigned short* sp, BYTE_2* pc, BYTE_2 N)
{
	stack[*sp] = *pc;
	*(sp)++;
	if(*sp > 15)
	{
		printf("Stack Overflow!!!!!!!!!\n");
		exit(0);
	}
	*(pc) = N;
}
//Opcode 0x3XNN  if X == NN then PC+2 else PC
void skip_eq_imm(BYTE x,BYTE n,BYTE* regs,BYTE_2* pc) 
{
	if(n == regs[x])  (*pc)+=2;
}
//Opcode 0x4XNN	if X != NN then PC+2 else PC
void skip_ne_imm(BYTE x,BYTE n,BYTE* regs,BYTE_2* pc)
{
	if (n != regs[x]) (*pc)+=2;
}
//Opcode 0x5XY0 if X == Y then PC+2 else PC
void skip_eq_reg(BYTE x,BYTE y,BYTE* regs,BYTE_2* pc)
{
	if( regs[x] == regs[y] ) (*pc) +=2;
}
//Opcode 0x6XNN	 X <- NN	
void load_imm(BYTE x,BYTE n,BYTE* regs)
{
	regs[x] = n;	
}
//Opcode 0x7XNN X <- X + NN
void add_imm(BYTE x,BYTE n,BYTE* regs)
{
	regs[x]+= n;
}
//Opcode 0x8XY0  X <- Y
void move_reg(BYTE x,BYTE y,BYTE* regs)
{
	regs[x] = regs[y];
}
//Opcode 0x8XY1 X <- X | Y
void or_reg(BYTE x,BYTE y,BYTE* regs)
{
	regs[x] = regs[x]|regs[y];
}
//Opcode 0x8XY2 X <- X & Y
void and_reg(BYTE x,BYTE y,BYTE* regs)
{
	regs[x] = regs[x]&regs[y];
}
//Opcode 0x8XY3 X <- X ^ Y
void xor_reg(BYTE x,BYTE y,BYTE* regs)
{
	regs[x] = regs[x]^regs[y];
}
//Opcode 0x8XY4 X <- Y + X , if Carry out then F = 1 else F = 0
void add_reg(BYTE x,BYTE y,BYTE* regs)
{
	int aux = (int)regs[x] + (int)regs[y];
	if (aux > 255) regs[15] = 1;
	else regs[15] = 0;
	regs[x] = (BYTE)aux;
}
//Opcode 0x8XY5 X <- X - Y, if Carry in then F = 0 else F = 1 
void sub_reg1(BYTE x,BYTE y,BYTE* regs)
{
	if (regs[y] > regs[x]) regs[15] = 0;
	else regs[15] = 1;
	regs[x] = regs[x]-regs[y];
}
//Opcode 0x8XY6 F = LSB X, X >> 1
void store_lsb_shiftl(BYTE x,BYTE* regs)
{
	regs[15] = regs[x]&0x01;
	regs[x]	= regs[x] >> 1;
}
//Opcode 0x8XY7 X <- Y - X , if Carry in then F = 0 else F = 1
void sub_reg2(BYTE x,BYTE y,BYTE* regs)
{
	if (regs[x] > regs[y]) regs[15] = 0;
	else regs[15] = 1;
	regs[x] = regs[y]-regs[x];
}
//Opcode 0x8XYE F = MSB X, X << 1
void store_msb_shiftr(BYTE x,BYTE* regs)
{
	regs[15] = regs[x]&0x80;
	regs[x]	= regs[x] << 1;	
}
//Opcode 0x9XY0 if X != Y then PC+2 else PC
void skip_ne_reg(BYTE x,BYTE y,BYTE* regs,BYTE_2* pc)
{
	if( regs[x] != regs[y] ) (*pc) +=2;
}
//Opcode 0xANNN I <- NNN
void load_address(BYTE_2* index,BYTE_2 n)
{
	*(index) = n;
}
//Opcode 0xBNNN PC = NNN + V0
void goto_imm(BYTE_2 n,BYTE* regs,BYTE_2* pc)
{
	(*pc) = n + regs[0];
}
//Opcode 0xCXNN X <- rand() & NN
void generate_mask()
{
	
}
//Opcode 0xDXYN Draw Sprite at X,Y with height N starting at address I, F = 1 if any pixels are changed else F = 0
void draw_sprite()
{
	//Implemented in chip8.c
}
//Opcode 0xEX8E if key X is pressed then PC+2 else PC
void skip_if_key_press(BYTE* key,BYTE x,BYTE_2* pc)
{
	if(key[x]) (*pc)+=2;
}
//Opcode 0xEXA1 if key X is not pressed then PC+2 else PC
void skip_if_key_not_pressed(BYTE* key,BYTE x,BYTE_2* pc)
{
	if(!key[x]) (*pc)+=2;
}
//Opcode 0xFX07 X <- delay_timer
void get_dtimer(BYTE x,BYTE* regs,BYTE_2 delay_timer)
{
	regs[x] = delay_timer;
}
//Opcode 0xFX0A Waits for a keypress (Blocking Operation. All instruction halted until next key event), Stores the key in X
void get_key(BYTE x,BYTE* regs)
{

}
//Opcode 0xFX15 delay_timer <- X
void set_dtimer(BYTE x,BYTE* regs,BYTE_2* delay_timer)
{
	(*delay_timer) = regs[x];
}
//Opcode 0xFX18 sound_timer <- X
void set_stimer(BYTE x,BYTE* regs,BYTE_2* sound_timer)
{
	(*sound_timer) = regs[x];
}
//Opcode 0xFX1E I <- X
void set_i(BYTE x,BYTE* regs,BYTE_2* index)
{
	(*index) = regs[x];
}
//Opcode 0xF29 I <- &Sprite[x]
void load_sprite_address()
{
	
}
//Opcode 0xFX33 [I-I+2] <- BCD(X)
void BCD_convert()
{
	
}
//Opcode 0xFX55 Store register data from V0 to VX starting at address I
void dump_regs(BYTE x,BYTE* regs,BYTE_2* index,BYTE* memory)
{
	for(BYTE_2 i=0;i<=x;i++)
	{
		memory[*index]= regs[i];
		*(index)++;
	}

	
}
//Opcode 0xFX65 Load register data from V0 to VX starting at address I
void load_regs(BYTE x,BYTE* regs,BYTE_2* index,BYTE* memory)
{
	for(BYTE_2 i=0;i<=x;i++)
	{
		regs[i] = memory[*index];
		*(index)++;
	}
	
}
#endif