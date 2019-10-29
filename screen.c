#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>
#include "screen.h"
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

    info->length = length;
    info->width  = width;
    //Init Screen Matrix
    info->screen_matrix = (BYTE**)malloc(64*sizeof(BYTE*));
    for(int i = 0; i < 64; i++)
        info->screen_matrix[i] = (BYTE*)malloc(32*sizeof(BYTE));

    for(int i = 0; i < 64;i++)
            for(int j = 0; j < 32; j++)
                info->screen_matrix[i][j] = 0; // 0 = Black , 1 = White

    printf("Display created sucessfully\n");    

    return info;
}


void screen_alter_grid(struct screen_info* info,int* draw_flag,int x,int y,int height,unsigned char* regs,unsigned char* memory,unsigned short* index)
{
    *draw_flag = 0;
    for (int l = 0; l < height; l++)
    {

        unsigned char c = memory[*index];
        for (int cont = 0;cont < 8;cont++)
        {
            unsigned char aux = c &0x01;
            if (aux)
            {
                 if (info->screen_matrix[y+l][x+cont]^aux == 1)
                    regs[15]=1;
                info->screen_matrix[y+l][x+cont] ^= aux;
            }
            c<<1;
        }
        (*index)++;
    }

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
    //16 px per line
    unsigned int px_per_line = info->length / 64;
    //25 px per collumn
    unsigned int px_per_collumn = info->width / 32;
    for(int l = 0; l < 64; l++)
    {
        for(int c = 0; c < 32; c++)
        {
            if(info->screen_matrix[l][c] == 1)
            {
                al_draw_filled_rectangle(px_per_collumn*c,px_per_line*l,px_per_collumn*(c+1),px_per_line*(l+1),al_map_rgb(0,0,0));
            }
            else
            {
                al_draw_filled_rectangle(px_per_collumn*c,px_per_line*l,px_per_collumn*(c+1),px_per_line*(l+1),al_map_rgb(0xFF,0xFF,0xFF));
            }    
        }
    }    
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

