//Instructions.cpp
//Opcode 0x00E0
void clear_screen(bool* flag)
{
	*flag = true;
}
//Opcode 0x00EE
void subroutine_return()
{
	
}
//Opcode 0x1NNN
void goto_(unsigned short adress,unsigned short* pc)
{
	*pc = adress;
}
//Opcode 0x2NNN
void subroutine_go()
{
	
}
//Opcode 0x3XNN  if X == NN then PC+2 else PC
void skip_eq_imm(unsigned char x,unsigned char n,unsigned char* regs,unsigned short* pc) 
{
	if(n == regs[x])  (*pc)+=2;
}
//Opcode 0x4XNN	if X != NN then PC+2 else PC
void skip_ne_imm(unsigned char x,unsigned char n,unsigned char* regs,unsigned short* pc)
{
	if (n != regs[x]) (*pc)+=2;
}
//Opcode 0x5XY0 if X == Y then PC+2 else PC
void skip_eq_reg(unsigned char x,unsigned char y,unsigned char* regs,unsigned short* pc)
{
	if( regs[x] == regs[y] ) (*pc) +=2;
}
//Opcode 0x6XNN	 X <- NN	
void load_imm(unsigned char x,unsigned char n,unsigned char* regs)
{
	regs[x] = n;	
}
//Opcode 0x7XNN X <- X + NN
void add_imm(unsigned char x,unsigned char n,unsigned char* regs)
{
	regs[x]+= n;
}
//Opcode 0x8XY0  X <- Y
void move_reg(unsigned char x,unsigned char y,unsigned char* regs)
{
	regs[x] = regs[y];
}
//Opcode 0x8XY1 X <- X | Y
void or_reg(unsigned char x,unsigned char y,unsigned char* regs)
{
	regs[x] = regs[x]|regs[y];
}
//Opcode 0x8XY2 X <- X & Y
void and_reg(unsigned char x,unsigned char y,unsigned char* regs)
{
	regs[x] = regs[x]&regs[y];
}
//Opcode 0x8XY3 X <- X ^ Y
void xor_reg(unsigned char x,unsigned char y,unsigned char* regs)
{
	regs[x] = regs[x]^regs[y];
}
//Opcode 0x8XY4 X <- Y + X , if Carry out then F = 1 else F = 0
void add_reg(unsigned char x,unsigned char y,unsigned char* regs)
{
	int aux = (int)regs[x] + (int)regs[y];
	if (aux > 255) regs[15] = 1;
	else regs[15] = 0;
	regs[x] = (unsigned char)aux;
}
//Opcode 0x8XY5 X <- X - Y, if Carry in then F = 0 else F = 1 
void sub_reg1(unsigned char x,unsigned char y,unsigned char* regs)
{
	if (regs[y] > regs[x]) regs[15] = 0;
	else regs[15] = 1;
	regs[x] = regs[x]-regs[y];
}
//Opcode 0x8XY6 F = LSB X, X >> 1
void store_lsb_shiftl(unsigned char x,unsigned char* regs)
{
	regs[15] = regs[x]&0x01;
	regs[x]	= regs[x] >> 1;
}
//Opcode 0x8XY7 X <- Y - X , if Carry in then F = 0 else F = 1
void sub_reg2(unsigned char x,unsigned char y,unsigned char* regs)
{
	if (regs[x] > regs[y]) regs[15] = 0;
	else regs[15] = 1;
	regs[x] = regs[y]-regs[x];
}
//Opcode 0x8XYE F = MSB X, X << 1
void store_msb_shiftr(unsigned char x,unsigned char* regs)
{
	regs[15] = regs[x]&0x80;
	regs[x]	= regs[x] << 1;	
}
//Opcode 0x9XY0 if X != Y then PC+2 else PC
void skip_ne_reg(unsigned char x,unsigned char y,unsigned char* regs,unsigned short* pc)
{
	if( regs[x] != regs[y] ) (*pc) +=2;
}
//Opcode 0xANNN I <- NNN
void load_address(unsigned short* index,unsigned short n)
{
	(*index) = n;
}
//Opcode 0xBNNN PC = NNN + V0
void goto_imm(unsigned short n,unsigned char* regs,unsigned short* pc)
{
	(*pc) = n + regs[0];
}
//Opcode 0xCXNN X <- rand() & NN
void generate_mask()
{
	
}
//Opcode 0xDXYN Draw Sprite at X,Y with N bytes starting at address I, F = 1 if any pixels are changed else F = 0
void draw_sprite()
{
	//Doesn't necessarly have to do the drawing, it could only send X,Y,N through aux variables (that currently dont exist)	
}
//Opcode 0xEX8E if key X is pressed then PC+2 else PC
void skip_if_key_press(unsigned char* key,unsigned char x,unsigned short* pc)
{
	if(key[x] == 1) (*pc)+=2;
}
//Opcode 0xEXA1 if key X is not pressed then PC+2 else PC
void skip_if_key_not_pressed(unsigned char* key,unsigned char x,unsigned short* pc)
{
	if(key[x] == 0) (*pc)+=2;
}
//Opcode 0xFX07 X <- delay_timer
void get_dtimer(unsigned char x,unsigned char* regs,unsigned short delay_timer)
{
	regs[x] = delay_timer;
}
//Opcode 0xFX0A Waits for a keypress (Blocking Operation. All instruction halted until next key event), Stores the key in X
void get_key()
{
	
}
//Opcode 0xFX15 delay_timer <- X
void set_dtimer(unsigned char x,unsigned char* regs,unsigned short* delay_timer)
{
	(*delay_timer) = regs[x];
}
//Opcode 0xF18 sound_timer <- X
void set_stimer(unsigned char x,unsigned char* regs,unsigned short* sound_timer)
{
	(*sound_timer) = regs[x];
}
//Opcode 0xFX1E I <- X
void set_i(unsigned char x,unsigned char* regs,unsigned short* index)
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
//Opcode 0xFX55 Store register data from V0 to VX starting at address I, // Not sure : -> afterwards I <- I + X + 1
void dump_regs(unsigned char x,unsigned char* regs,unsigned short* index,unsigned char* memory)
{
	unsigned short i;
	for(i=0;i<=x;i++)
	{
		memory[i+(*index)]= regs[i];
	}
	(*index) += x+1;//

	
}
//Opcode 0xFX65 Load register data from V0 to VX starting at address I, // Not sure : -> afterwards I <- I + X + 1
void load_regs(unsigned char x,unsigned char* regs,unsigned short* index,unsigned char* memory)
{
	unsigned short i;
	for(i=0;i<=x;i++)
	{
		regs[i] = memory[i+(*index)];
	}
	(*index) += x+1;//
	
}
