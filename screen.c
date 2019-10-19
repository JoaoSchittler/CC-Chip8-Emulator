#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>
#include "screen.h"
#include "instructions.h"
#define BYTE unsigned char

struct screen_info* screen_init(unsigned int length, unsigned int width,const char * screen_name)
{
	if (!al_init()) 
	{
        printf("Error while initializing allegro");
        exit(0);
    }
    al_install_keyboard();
    al_install_audio();
    al_init_primitives_addon();
    al_init_acodec_addon();



    struct screen_info* info =(struct screen_info*)malloc(sizeof(struct screen_info));

	info->display = al_create_display(width, length);
	if (info->display == NULL)
	{
		printf("Error While Creating Display");
		exit(0);
	}	
    info->queue = al_create_event_queue();
    al_register_event_source(info->queue, al_get_keyboard_event_source());
    al_register_event_source(info->queue, al_get_display_event_source(info->display));
    al_set_window_title(info->display, screen_name);


    //Init Screen Matrix
    info->screen_matrix = (BYTE**)malloc(64*sizeof(BYTE*));
    for(int i = 0; i < 64; i++)
        info->screen_matrix[i] = (BYTE*)malloc(32*sizeof(BYTE));

    for(int i = 0; i < 64;i++)
            for(int j = 0; j < 32; j++)
                info->screen_matrix[i][j] = 0;

    printf("Dysplay created sucessfully\n");    

    return info;
}


void screen_alter_grid(struct screen_info* info,int* draw_flag,int x,int y,BYTE* e0, int collum,int line,int size)
{
    *draw_flag = 0;
}
void screen_clear_grid(struct screen_info* info,int* draw_flag)
{
    for(int i = 0; i < 64;i++)
        for(int j =0; j < 32; j++)
            info->screen_matrix[i][j]=0;
    *draw_flag = 0;    
}

void screen_refresh(struct screen_info* info)
{
    al_flip_display();
}

void screen_wait(struct screen_info* info , double ms)
{
    al_rest(ms);
}

void screen_delete(struct screen_info* info)
{
    for(int i =0; i < 64; i++)
        free(info->screen_matrix[i]);

    al_destroy_display(info->display);
    al_destroy_event_queue(info->queue);
    free(info);
}

