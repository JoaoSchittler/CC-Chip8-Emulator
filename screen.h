#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#define BYTE unsigned char
struct screen_info{

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
    BYTE** screen_matrix;
    unsigned int length;
    unsigned int  width;
};
typedef struct screen_info screen_info;
screen_info* screen_init(unsigned int  legnth, unsigned int width,const char * screen_name);

void screen_alter_grid(struct screen_info* info,int* draw_flag,int x,int y,int height,unsigned char* regs,unsigned char* memory,unsigned short* index);

void screen_clear_grid(struct screen_info* info,int* draw_flag);

void screen_handle_event();

void screen_refresh(screen_info* info);

void screen_wait(screen_info* info , double ms);

void screen_delete(screen_info* info);
	
