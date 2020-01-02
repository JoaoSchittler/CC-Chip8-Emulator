#ifndef _INS_H_
#define _INS_H_
#include <stdio.h>
#include <stdlib.h>
#define BYTE unsigned char
#define BYTE_2 unsigned short
//Instructions.c
//Opcode 0x00E0
void clear_screen(int* flag);

//Opcode 0x00EE
void subroutine_return(unsigned short * stack, unsigned short* sp, BYTE_2* pc);

//Opcode 0x1NNN
void go_to(BYTE_2 adress,BYTE_2* pc);

//Opcode 0x2NNN
void subroutine_go(unsigned short * stack, unsigned short* sp, BYTE_2* pc, BYTE_2 N);

//Opcode 0x3XNN
void skip_eq_imm(BYTE x,BYTE n,BYTE* regs,BYTE_2* pc);

//Opcode 0x4XNN
void skip_ne_imm(BYTE x,BYTE n,BYTE* regs,BYTE_2* pc);

//Opcode 0x5XY0
void skip_eq_reg(BYTE x,BYTE y,BYTE* regs,BYTE_2* pc);

//Opcode 0x6XNN
void load_imm(BYTE x,BYTE n,BYTE* regs);

//Opcode 0x7XNN
void add_imm(BYTE x,BYTE n,BYTE* regs);

//Opcode 0x8XY0
void move_reg(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XY1
void or_reg(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XY2
void and_reg(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XY3
void xor_reg(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XY4
void add_reg(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XY5
void sub_reg1(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XY6
void store_lsb_shiftl(BYTE x,BYTE* regs);

//Opcode 0x8XY7
void sub_reg2(BYTE x,BYTE y,BYTE* regs);

//Opcode 0x8XYE
void store_msb_shiftr(BYTE x,BYTE* regs);

//Opcode 0x9XY0
void skip_ne_reg(BYTE x,BYTE y,BYTE* regs,BYTE_2* pc);

//Opcode 0xANNN
void load_address(BYTE_2* index,BYTE_2 n);

//Opcode 0xBNNN
void goto_imm(BYTE_2 n,BYTE* regs,BYTE_2* pc);

//Opcode 0xCXNN
void generate_mask(BYTE_2 n, BYTE* regs, BYTE x);

//Opcode 0xDXYN
void draw_sprite();

//Opcode 0xEX8E
void skip_if_key_press(BYTE* key,BYTE x,BYTE_2* pc);

//Opcode 0xEXA1
void skip_if_key_not_pressed(BYTE* key,BYTE x,BYTE_2* pc);

//Opcode 0xFX07
void get_dtimer(BYTE x,BYTE* regs,BYTE_2 delay_timer);

//Opcode 0xFX0A
void get_key(BYTE x,BYTE* regs);

//Opcode 0xFX15
void set_dtimer(BYTE x,BYTE* regs,BYTE_2* delay_timer);

//Opcode 0xFX18
void set_stimer(BYTE x,BYTE* regs,BYTE_2* sound_timer);

//Opcode 0xFX1E
void set_i(BYTE x,BYTE* regs,BYTE_2* index);

//Opcode 0xF29
void load_sprite_address();

//Opcode 0xFX33 
void BCD_convert(BYTE x,BYTE* regs,BYTE_2 index,BYTE* memory);

//Opcode 0xFX55
void dump_regs(BYTE x,BYTE* regs,BYTE_2 index,BYTE* memory);

//Opcode 0xFX65
void load_regs(BYTE x,BYTE* regs,BYTE_2 index,BYTE* memory);

#endif