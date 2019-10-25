//Chip8.c
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "instructions.h"
#include <stdbool.h>
#include <string.h>


//TODO - init fontset
void c8_init(struct Chip8* c8)
{
	// Initialize registers and memory 
	c8->currentinstruction  = 0;
	c8->drawflag = 0;
	c8->get_keyboard_input = 0;
	c8->pc      = 0x200;
	c8->game_size = 0;
	c8->sp	= 0;
	c8->index	= 0;
	c8->sound_timer = 0;
	c8->delay_timer = 0;
	int i;
	for(i=0;i<4096;i++)
	{
		if(i<16)
		{
			c8->reg_v[i]=0;
			c8->stack[i]=0;
			c8->key[i]=0;
		} 
		c8->memory[i] = 0;
	}
	//Load fontset
	for(i=0;i<80;i++)
	{
		c8->memory[i]=c8->fontset[i];
	}


	c8->scr = screen_init(800,600,"Chip-8 Emulator");
	
}
int c8_loadGame(const char* str,struct Chip8* c8)
{
	//Open the file in output mode and set the fp to the end 
	FILE* game_file = fopen(str,"r");
	if(game_file == NULL) 
		return 0;

	fseek(game_file, 0L, SEEK_END);
    int size = ftell(game_file); // Number of Bytes of file
    rewind(game_file);
	c8->game_size = size/2;

	for(int i=0;i<size;i++){
		int aux = getc(game_file);
		c8->memory[i+0x200] = (unsigned char)aux;
	}

	printf("%d Bytes Loaded\n",size);
	fclose(game_file);
	return 1;
}
void c8_play_game(struct Chip8* c8)
{
	while(1)
	{
		if(c8->pc > c8->game_size + 0x200)
				break;

		c8_emulate_cycle(c8);

		if(c8->drawflag == 2)
			screen_clear_grid(c8->scr,&(c8->drawflag));
		//if(c8->drawflag == 1)
			//screen_alter_grid();

		screen_wait(c8->scr,0.01666);
	}

}

void c8_decode_execute_instruction(struct Chip8* c8)
{
	unsigned short first_nibble  = (c8->currentinstruction & 0xF000) >> 12;
	unsigned short second_nibble = (c8->currentinstruction & 0x0F00) >> 8;
	unsigned short third_nibble  = (c8->currentinstruction & 0x00F0) >> 4;
	unsigned short fourth_nibble = (c8->currentinstruction & 0x000F)     ;
	int invalid_ins = 0;
	c8->pc = c8->pc+2;	//Always increments pc
	switch(first_nibble)
	{
		case 0x0: 	switch(fourth_nibble){

						case 0x0:
								clear_screen(&(c8->drawflag));
								break;
						case 0xE:
								//subroutine_return();
								break;
						default:
								invalid_ins = 1;				
					}	
					break;

		case 0x1: 	//go_to(,&(c8->pc));

					break;

		case 0x2:	//subroutine_go();

					break;

		case 0x3:	skip_eq_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->reg_v,&(c8->pc));
					break;

		case 0x4:	skip_ne_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->reg_v,&(c8->pc));
					break;

		case 0x5:	skip_eq_reg(second_nibble,third_nibble,c8->reg_v,&(c8->pc));
					break;

		case 0x6: 	load_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->reg_v);
					break;

		case 0x7: 	add_imm(second_nibble,(BYTE)c8->currentinstruction & 0x00FF,c8->reg_v);
					break;

		case 0x8:	switch(fourth_nibble){

						case 0x0:
								move_reg(second_nibble,third_nibble,c8->reg_v);
								break;
						case 0x1:
								or_reg(second_nibble,third_nibble,c8->reg_v);
								break;
						case 0x2:
								and_reg(second_nibble,third_nibble,c8->reg_v);
								break;
						case 0x3:
								xor_reg(second_nibble,third_nibble,c8->reg_v);
								break;
						case 0x4:
								add_reg(second_nibble,third_nibble,c8->reg_v);
								break;
						case 0x5:
								sub_reg1(second_nibble,third_nibble,c8->reg_v);
								break;	
						case 0x6:
								store_lsb_shiftl(second_nibble,c8->reg_v);
								break;
						case 0x7:
								sub_reg2(second_nibble,third_nibble,c8->reg_v);
								break;	
						case 0xE:
								store_msb_shiftr(second_nibble,c8->reg_v);
								break;					
						default:
								invalid_ins = 1;				
					}	
					break;

		case 0x9:	skip_ne_reg(second_nibble,third_nibble,c8->reg_v,&(c8->pc));
					break;

		case 0xA:	load_address(&(c8->index),c8->currentinstruction & 0x0FFF);
					break;

		case 0xB:	goto_imm(c8->currentinstruction & 0x0FFF,c8->reg_v,&(c8->pc));
					break;	

		case 0xC:	//generate_mask()
					break;

		case 0xD:	draw_sprite(&(c8->drawflag));
					break;

		case 0xE:	switch(fourth_nibble){

						case 0xE:
								skip_if_key_press(c8->key,second_nibble,&(c8->pc));
								break;
						
						case 0x1:
								skip_if_key_not_pressed(c8->key,second_nibble,&(c8->pc));
								break;
						default:
								invalid_ins = 1;				
					}	
					break;

		case 0xF:	switch(third_nibble){

						case 0x0:
								switch(fourth_nibble){

									case 0x7://Opcode 0xFX07 X <- delay_timer
												get_dtimer(second_nibble,c8->reg_v,c8->delay_timer);
												break;
						
									case 0xA:
												//get_key(BYTE x,c8->reg_v);
												//c8->getKey = true;
												break;
									default:
										invalid_ins = 1;				
								}	
								break;
						case 0x1:
								switch(fourth_nibble){

									case 0x5:
												set_dtimer(second_nibble,c8->reg_v,&(c8->delay_timer));
												break;
						
									case 0x8:
												set_stimer(second_nibble,c8->reg_v,&(c8->sound_timer));
												break;
									case 0xE:
												set_i(second_nibble,c8->reg_v,&(c8->index));
												break;
									default:
										invalid_ins = 1;				
								}	
								break;
						case 0x2:
								//load_sprite_address();
								break;
						case 0x3:
								//BCD_convert();
								break;
						case 0x5://Opcode 0xFX55
								dump_regs(second_nibble,c8->reg_v,&(c8->index),c8->memory);
								break;
						case 0x6:
								load_regs(second_nibble,c8->reg_v,&(c8->index),c8->memory);
								break;					
						default:
								invalid_ins = 1;				
					}	
					break;

		default:
				invalid_ins = 1;

	}
	if(invalid_ins)
		printf("Unknown instruction 1:%x,2:%x,3:%x,4:%x\n",first_nibble,second_nibble,third_nibble,fourth_nibble);

}

void c8_emulate_cycle(struct Chip8* c8)
{	
	// Fetch Instruction
	c8->currentinstruction =  (c8->memory[c8->pc]<< 8) | c8->memory[c8->pc + 1];	
	printf("%x\n",c8->currentinstruction);
	c8_decode_execute_instruction(c8);
  	// Update timers
  	if(c8->delay_timer>0)c8->delay_timer--;
  	if(c8->sound_timer>0)
	{
		if(c8->sound_timer==1)
		  	printf("Beep"); // Play beep sound
	
		c8->sound_timer--;	
	}

}

void c8_finalize(struct Chip8* c8)
{
	screen_delete(c8->scr);
}
