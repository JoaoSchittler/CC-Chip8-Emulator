#include "screen.h"
unsigned char translate_key (int raw_key);


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


void screen_alter_grid(struct screen_info* info,int x,int y,int height,unsigned char* regs,unsigned char* memory,unsigned short index)
{   
    
    for (int l = 0; l < height; l++)
    {
        unsigned char c = memory[index + l];
        
        for (int cont = 0;cont < 8;cont++)
        {
            unsigned char aux = c &0x80;
            if (aux)
            {
                 if (info->screen_matrix[y+l][x+cont]^aux == 1){
                    regs[15]=1;
                 }
                info->screen_matrix[y+l][x+cont] ^= aux;
                
            }
            c = c<<1;
        }
        
    }

}
void screen_clear_grid(struct screen_info* info)
{
    for(int i = 0; i < 64;i++)
        for(int j =0; j < 32; j++)
            info->screen_matrix[i][j]=0;
       
}

void screen_manage_events(struct screen_info* info,unsigned char* keys)
{
    ALLEGRO_EVENT event;
    while(al_get_next_event(info->queue,&event))
    {
        switch(event.type)
        {
            case ALLEGRO_EVENT_KEY_DOWN :
            {           
                int idx = translate_key(event.keyboard.keycode);
                if(idx < 16) 
                    keys[idx] = 1;
                break;
            }
            case ALLEGRO_EVENT_KEY_UP:
            {
                int idx = translate_key(event.keyboard.keycode);
                if(idx < 16) 
                    keys[idx] = 0;
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
            {
                screen_delete(info);
                exit(0);
            }
        }
    }    
}
unsigned char screen_getinput(struct screen_info* info)
{
    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;
    unsigned char key;
    int ev;
    loop:
    al_init_timeout(&timeout, 0.016); // Checks at 60Hz for input
    ev = al_wait_for_event_until(info->queue, &event, &timeout);


    if ( ev && event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        key = translate_key(event.keyboard.keycode);
        if( key < 16){
            return key;
        }
        else{
            goto loop;
        }
    }
    else
    {
        goto loop;
    }    
    
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
            if(info->screen_matrix[l][c] == 0)
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
unsigned char translate_key (int raw_key)
{
    //Converts real keyboard key into CHIP-8 key index, returns 0x10 if invalid key
    switch ( raw_key )
    {
        case ALLEGRO_KEY_1 :            return 0x01;
        case ALLEGRO_KEY_2 :            return 0x02;
        case ALLEGRO_KEY_3 :            return 0x03;
        case ALLEGRO_KEY_4 :            return 0x0C;
        case ALLEGRO_KEY_Q :            return 0x04;
        case ALLEGRO_KEY_W :            return 0x05;
        case ALLEGRO_KEY_E :            return 0x06;
        case ALLEGRO_KEY_R :            return 0x0D;
        case ALLEGRO_KEY_A :            return 0x07;
        case ALLEGRO_KEY_S :            return 0x08;
        case ALLEGRO_KEY_D :            return 0x09;
        case ALLEGRO_KEY_F :            return 0x0E;
        case ALLEGRO_KEY_Z :            return 0x0A;
        case ALLEGRO_KEY_X :            return 0x00;
        case ALLEGRO_KEY_C :            return 0x0B;
        case ALLEGRO_KEY_V :            return 0x0F;
        default:                        return 0x10;
    }
}
