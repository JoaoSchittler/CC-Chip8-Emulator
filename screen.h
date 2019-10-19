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
};
typedef struct screen_info screen_info;
screen_info* screen_init(unsigned int  legnth, unsigned int width,const char * screen_name);

void screen_alter_grid(screen_info* info,int* draw_flag,int x,int y,BYTE* e0, int collum,int line,int size);

void screen_clear_grid(struct screen_info* info,int* draw_flag);

void screen_refresh(screen_info* info);

void screen_wait(screen_info* info , double ms);

void screen_delete(screen_info* info);
	
