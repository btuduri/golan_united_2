#include "functions.h"


void last_part() {
    first_fl=2;
    lmb=0;
    flash_screen();
    display_kenet();
    while(!lmb) {
        test_lmb();
        scan_keys();
        display_text(color_car, color_omb, SDL_ALPHA_OPAQUE);
        init_coords();
        display_cursor();
        test_cls();
        test_swap_keyboard();
    }
    smooth_quit();
}

void smooth_quit() {
    int nb=50;
    int i;
    double alpha=(double)255;
    double volume=(double)128;
    double step_vol=volume/(double)nb;
    double step_alpha=alpha/(double)nb;
    p_texture=SDL_CreateTextureFromSurface(p_renderer, p_fond_12);
    for(i=nb; i>=0; i--) {
        SDL_SetTextureAlphaMod(p_texture, (int)floor(alpha+0.5));
        Mix_VolumeMusic((int)floor(volume+0.5));
        SDL_RenderClear(p_renderer);
        SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        alpha-=step_alpha; volume-=step_vol;
        if(alpha<0) {
            alpha=(double)0;
        }
        if(volume<0) {
            volume=(double)0;
        }
    }
    SDL_DestroyTexture(p_texture);
}

void test_cls() {
    if(last_chars[0]=='c'&&last_chars[1]=='l'&&last_chars[2]=='s') {
        swap_keyboard=1;
        last_chars[0]=' ';
        last_chars[1]=' ';
        last_chars[2]=' ';
    }
}

void test_swap_keyboard() {
        if(swap_keyboard) {
        int nb=10, i;
        swap_keyboard=0;
        double r=(double)RED; double g=(double)GREEN; double b=(double)BLUE;
        double st_r=((double)255-r)/(double)nb;
        double st_g=((double)255-g)/(double)nb;
        double st_b=((double)255-b)/(double)nb;

        for(i=0; i<nb; i++) {
            color1.r=(int)r;
            color1.g=(int)g;
            color1.b=(int)b;
            typing=1;
            display_text(color1, color_omb, SDL_ALPHA_OPAQUE);
            r+=st_r; g+=st_g; b+=st_b;
        }
        for(i=SDL_ALPHA_OPAQUE; i>=SDL_ALPHA_TRANSPARENT; i-=20) {
            typing=1;
            display_text(color1, color_omb, i);
        }
        //
        for(i=0; i<nb_lines; i++) {
            array_car[i*200]='\0';
        }
        ind_car_x=0;
        ind_car_y=0;
    }
}

void display_text(SDL_Color color1, SDL_Color color2, int alpha) {
    if(typing) {
        typing=0;
        SDL_BlitSurface(p_kenet, NULL, p_fond_12, NULL);
        int i, w, h;
        int x_pos=10, y_pos=10;
        SDL_Surface *text_surface;
        SDL_Surface *text_surface_shade;
        SDL_Rect rect;
        for(i=0; i<nb_lines; i++) {
            TTF_SizeText(font, (const char*)array_car+200*i, &w, &h);
            text_surface_shade=TTF_RenderText_Solid(font, (const char*)array_car+200*i, color2);
            rect.x=x_pos+2;
            rect.y=y_pos+2;
            rect.w=w;
            rect.h=h;
            SDL_SetSurfaceAlphaMod(text_surface_shade, alpha);
            SDL_BlitSurface(text_surface_shade, NULL, p_fond_12, &rect);
            SDL_FreeSurface(text_surface_shade);
            //
            text_surface=TTF_RenderText_Solid(font, (const char*)array_car+200*i, color1);
            rect.x=x_pos;
            rect.y=y_pos;
            rect.w=w;
            rect.h=h;
            SDL_SetSurfaceAlphaMod(text_surface, alpha);
            SDL_BlitSurface(text_surface, NULL, p_fond_12, &rect);
            SDL_FreeSurface(text_surface);
            y_pos=y_pos+h+5;
        }
        p_texture=SDL_CreateTextureFromSurface(p_renderer, p_fond_12);
        SDL_RenderClear(p_renderer);
        SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        SDL_DestroyTexture(p_texture);
   }
}

void scan_keys() {
    car_ok=0;
    unsigned int car_int=array_keyboard[index_key++];
    if(car_int==255) {
        index_key=0;
        swap_keyboard=1;
    } else {
    if(car_int&0x40000000) {
        //printf("avant=%d\n", car_int);
        car_int=(car_int^0x40000000)/10;
        //printf("après=%d\n", car_int);
        SDL_Delay(car_int);
    } else if(car_int&0x20000000) {
        car_ok=car_int^0x20000000;
        treat_car_ok();
    } else {
        actual_char=(char)car_int;
        record_char();
    }
    }
}

void record_char() {
    strcpy(line_char, array_car+ind_car_y*200);
    if(line_char[ind_car_x]=='\0') {
        line_char[ind_car_x]=actual_char;
        line_char[ind_car_x+1]='\0';
    } else {
        strcpy(line_char_2, line_char+ind_car_x);
        line_char[ind_car_x]=actual_char;
        line_char[ind_car_x+1]='\0';
        strcat(line_char, line_char_2);
    }

    if(width_ok()) {
        strcpy(array_car+ind_car_y*200, line_char);
        ind_car_x++;
        writting=1; typing=1;
    }
}

int width_ok() {
    int w, h;
    TTF_SizeText(font, (const char*)array_car+200*ind_car_y, &w, &h);
    if(w<(980)) {
        return 1;
    }
    return 0;
}

void treat_car_ok() {
    typing=1;
    switch(car_ok) {
        case 2:
            key_backspace();
            break;
        case 3:
            key_up();
            break;
        case 4:
            key_left();
            break;
        case 5:
            key_right();
            break;
        case 6:
            key_delete();
            break;
        case 7:
            key_enter();
            break;
        case 8:
            key_down();
            break;
    }
}

void key_down() {
    if(ind_car_y<(nb_lines-1)) {
        ind_car_y++;
        int ind_x=0;
        while((array_car[ind_car_y*200+ind_x]!='\0')&&ind_x<ind_car_x) {
            ind_x++;
        }
        ind_car_x=ind_x;
    }
}


void key_enter() {
    int i;
    if(array_car[ind_car_y*200+ind_car_x]=='\0') {
        if(ind_car_y==(nb_lines-1)) {
            for(i=0; i<nb_lines-2; i++) {
                strcpy(array_car+i*200, array_car+(i+1)*200);
            }
            array_car[(nb_lines-1)*200]='\0';
            ind_car_x=0;
        } else {
            for(i=nb_lines-2; i>=(ind_car_y+1); i--) {
                strcpy(array_car+200*(i+1), array_car+200*i);
            }
            ind_car_y++;
            ind_car_x=0;
            array_car[200*ind_car_y]='\0';
        }
    } else {
        if(ind_car_y==nb_lines-1) {
            strcpy(line_char, array_car+ind_car_y*200);
            line_char[ind_car_x]='\0';
            strcpy(line_char_2, array_car+ind_car_y*200+ind_car_x);
            //
            for(i=0; i<nb_lines-2; i++) {
                strcpy(array_car+i*200, array_car+(i+1)*200);
            }
            strcpy(array_car+(nb_lines-2)*200, line_char);
            strcpy(array_car+(nb_lines-1)*200, line_char_2);
            ind_car_x=0;
        }
    }
}

void key_delete() {
    if(array_car[ind_car_y*200+ind_car_x]!='\0') {
        int ind_car=ind_car_x;
        while(array_car[ind_car_y*200+ind_car]!='\0') {
            array_car[ind_car_y*200+ind_car]=array_car[ind_car_y*200+ind_car+1];
            ind_car++;
        }
    }
}

void key_right() {
    if(array_car[ind_car_y*200+ind_car_x]!='\0') {
        ind_car_x++;
    }
}

void key_left() {
    ind_car_x--;
    if(ind_car_x<0) {
        ind_car_x=0;
    }
}

void key_up() {
    if(ind_car_y>0) {
        ind_car_y--;
        int ind_x=0;
        char ind_car;
        do {
           ind_car=array_car[ind_car_y*200+ind_x++];
        } while(ind_car!='\0'&&ind_x<ind_car_x);
        ind_x--;
        ind_car_x=ind_x;
        }
}

void key_backspace() {
    if(array_car[ind_car_y*200+ind_car_x]=='\0') {
        if(ind_car_x>0) {
            ind_car_x--;
            array_car[ind_car_y*200+ind_car_x]='\0';
        }
    } else {
        if(ind_car_x>0) {
            strcpy(line_char, array_car+ind_car_y*200+ind_car_x);
            ind_car_x--;
            array_car[ind_car_y*200+ind_car_x]='\0';
            strcat(array_car+ind_car_y*200, line_char);
        }
    }
}

void init_coords() {
    int w, h;
    //
    x=10; y=10;
    int i;
    for(i=0; i<ind_car_x; i++) {
        line_char[i]=array_car[ind_car_y*200+i];
    }
    line_char[i]='\0';
    TTF_SizeText(font, (const char*)line_char, &w, &h);
    x+=w;
    //
    for(i=0; i<ind_car_y; i++) {
        TTF_SizeText(font, (const char*)array_car+i*200, &w, &h);
        y=y+h+5;
    }
    //printf("x=%d y=%d\n", x, y);
}


void display_cursor() {
 //
    index_curv_curs++;
    if(index_curv_curs>=CURVCURS) {
        index_curv_curs=0;
    }
    SDL_Color color={array_curv_curs[index_curv_curs][0], array_curv_curs[index_curv_curs][1],array_curv_curs[index_curv_curs][2],SDL_ALPHA_OPAQUE};
    p_cursor=TTF_RenderText_Solid(font, "_", color);
    SDL_Rect omb2={x+1, y+5+1, w_cursor, h_cursor};
    SDL_BlitSurface(p_cursor_2, NULL, p_fond_12, &omb2);
    SDL_Rect rect={x, y+5, w_cursor, h_cursor};
    SDL_BlitSurface(p_cursor, NULL, p_fond_12, &rect);
    SDL_FreeSurface(p_cursor);
    p_cursor=NULL;
    p_texture=SDL_CreateTextureFromSurface(p_renderer, p_fond_12);
    SDL_RenderClear(p_renderer);
    SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
    SDL_RenderPresent(p_renderer);
    SDL_DestroyTexture(p_texture);
 }

void display_kenet() {
    p_texture_kenet=SDL_CreateTextureFromSurface(p_renderer, p_kenet);
    int alpha=255;
    int loop=1;
    while(loop) {
       if(alpha<0) {
            loop=0;
            alpha=0;
        }
        SDL_SetTextureAlphaMod(p_texture_fond_blanc, alpha);
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture_kenet, NULL, NULL);
        SDL_RenderCopy(p_renderer, p_texture_fond_blanc, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        alpha-=10;
    }
    SDL_RenderClear( p_renderer );
    SDL_RenderCopy(p_renderer, p_texture_kenet, NULL, NULL);
    SDL_RenderPresent(p_renderer);
    SDL_DestroyTexture(p_texture_fond_blanc);
    SDL_DestroyTexture(p_texture_kenet);
}


void flash_screen() {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    p_fond_12=SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, rmask, gmask, bmask, amask);
    p_fond_12=SDL_ConvertSurfaceFormat(p_fond_12, pixel_format, 0);
    SDL_FillRect(p_fond_12, NULL, SDL_MapRGB(p_fond_12->format, 255, 255, 255));
    p_texture_fond_blanc=SDL_CreateTextureFromSurface(p_renderer, p_fond_12);
    int alpha=0;
    int loop=1;
    while(loop) {
        if(alpha>255) {
            loop=0;
            alpha=255;
        }
        SDL_SetTextureAlphaMod(p_texture_fond_blanc, alpha);
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture_fond_blanc, NULL, NULL);
        SDL_RenderPresent(p_renderer);
        alpha+=15;
    }
}


void first_part() {
    init_first_part();
    swap_screen_1();
    index_draw=0;
    step_wait_coords=1;
    prevX=-1;
    prevY=-1;
    while(!lmb) {
        test_lmb();
        int coord=array_coords[index_draw];
        if(coord==-2) {
            index_draw=0;
            prevX=-1;
            prevY=-1;
            swap_screen_1();
        } else if (coord==-1) {
            prevX=-1;
            prevY=-1;
            tick=0;
            index_draw++;
            SDL_Delay(1000);
            swap_screen_1();
        } else if(coord==-3) {
            index_draw++;
            prevX=-1;
            prevY=-1;
            int delay=array_coords[index_draw++];
            SDL_Delay(delay/4);
         } else {
            x=array_coords[index_draw++];
            y=array_coords[index_draw++];
            //printf("x=%d y=%d\n", x, y);
            draw();
        }
    }
    quit_first_part();
}

void draw() {
    if(prevX<0 && prevY<0) {
        prevX=x;
        prevY=y;
        }
        double pos_x=(double)prevX;
        double pos_y=(double)prevY;
        double step_x=((double)x-pos_x)/(double)100;
        double step_y=((double)y-pos_y)/(double)100;
        int i=0;
        SDL_Rect rect;
        rect.w=10;
        rect.h=10;
        while(i<100 && !lmb) {
            test_lmb();
            rect.x=(int)pos_x;
            rect.y=(int)pos_y;
            SDL_BlitSurface(p_brush, NULL, p_fond_12, &rect);
            //SDL_UpdateTexture(p_texture, &rect, p_brush->pixels, p_brush->pitch);
            pos_x+=step_x;
            pos_y+=step_y;
            i++;
       }
        SDL_UpdateTexture(p_texture, NULL, p_fond_12->pixels, p_fond_12->pitch);
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
        //SDL_framerateDelay(&manager);
        SDL_RenderPresent(p_renderer);
    prevX=x;
    prevY=y;
}

void swap_screen_1() {
    double posY;
    SDL_Rect src;
    SDL_Rect dest;
    src.x=0;
    src.y=0;
    src.w=1024;
    dest.x=0;
    dest.w=1024;
    double x=0.0;
    int loop=1;
    while(loop&&!lmb) {
        test_lmb();
        posY=(double)768-(x*x);
        if(posY<0) {
            posY=0.0;
            loop=0;
        }
        x+=1.0;
        dest.y=(int)posY;
        src.h=768-(int)posY;
        dest.h=768-(int)posY;
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture_fond_1, &src, &dest);
        //SDL_framerateDelay(&manager);
        SDL_RenderPresent(p_renderer);
    }
    //SDL_FillRect(p_fond_12, NULL, SDL_MapRGB(p_fond_12->format, 0, 0, 0));
    SDL_BlitSurface(p_fond_1, NULL, p_fond_12, NULL);
    //p_texture=SDL_CreateTextureFromSurface(p_renderer, p_fond_1);
    //SDL_UpdateTexture(p_texture, NULL, p_fond_1->pixels, p_fond_1->pitch);
}

void quit_first_part() {
    SDL_FreeSurface(p_brush);
    p_brush=NULL;
    SDL_FreeSurface(p_fond_1);
    p_fond_1=NULL;
    SDL_FreeSurface(p_fond_12);
    p_fond_12=NULL;
    SDL_DestroyTexture(p_texture_fond_1);
    SDL_DestroyTexture(p_texture);
}

void init_first_part() {
    //
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    p_fond_12=SDL_CreateRGBSurface(0, 1024, 768, 32, rmask, gmask, bmask, amask);
    p_fond_12=SDL_ConvertSurfaceFormat(p_fond_12, pixel_format, 0);
    //
    //p_texture=SDL_CreateTexture(p_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1024, 768);
    p_texture = SDL_CreateTexture(p_renderer,
                                pixel_format,
                               SDL_TEXTUREACCESS_STREAMING,
                               1024, 768);
    p_texture_fond_1=SDL_CreateTextureFromSurface(p_renderer, p_fond_1);
}

void test_lmb() {
    SDL_Scancode key_scancode;
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_KEYDOWN:
            key_scancode=event.key.keysym.scancode;
        switch (key_scancode) {
                case SDL_SCANCODE_ESCAPE:
                if(first_fl==2) {
                lmb=1;
                }
                break;
                case SDL_SCANCODE_SPACE:
                if(first_fl==1) {
                    lmb=1;
                }
                break;
        }
        break;
    }
}

void intro() {
    p_texture_logo=SDL_CreateTextureFromSurface(p_renderer, p_logo);
    SDL_FreeSurface(p_logo);
    //
    SDL_SetTextureBlendMode(p_texture_logo, SDL_BLENDMODE_BLEND);
    //
    Mix_PlayMusic(p_module, -1);
    //
    SDL_Rect dest = { 1024/2 - 386/2,768/2 - 512/2, 386, 512};
    SDL_SetRenderDrawColor( p_renderer, 10, 19, 36, 0 );
    int alpha=0;
    double volume=0.0;
    double stepVol=(double)128/(double)510;
    while(alpha<=255) {
        Mix_VolumeMusic((int)floor(volume+0.5));
        volume+=stepVol;
        SDL_SetTextureAlphaMod(p_texture_logo, alpha);
        SDL_RenderClear( p_renderer );
        SDL_RenderCopy(p_renderer, p_texture_logo, NULL, &dest);
        SDL_RenderPresent(p_renderer);
       //SDL_framerateDelay(&manager);
        alpha+=1;
    }

    alpha=255;
    while(alpha>=0) {
        Mix_VolumeMusic((int)floor(volume+0.5));
        volume+=stepVol;
        SDL_RenderClear( p_renderer );
        SDL_SetTextureAlphaMod(p_texture_logo, alpha);
        SDL_RenderCopy(p_renderer, p_texture_logo, NULL, &dest);
        SDL_RenderPresent(p_renderer);
        alpha-=1;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    SDL_RenderClear( p_renderer );
    SDL_RenderPresent(p_renderer);
    SDL_DestroyTexture(p_texture_logo);
    first_fl=1;
}

void init() {
    pixel_format=SDL_PIXELFORMAT_ARGB8888;
    p_rw=SDL_RWFromMem(array_logo, array_logo_size);
    p_rw_mod=SDL_RWFromMem(array_module, array_module_size);
    p_rw_fond_1=SDL_RWFromMem(array_fond_1, array_fond_1_size);
    p_rw_brush=SDL_RWFromMem(array_brush, array_brush_size);

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS|SDL_INIT_TIMER);

    IMG_Init(IMG_INIT_PNG);

    p_window=SDL_CreateWindow("HMD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    p_renderer=SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(p_renderer, 1024, 768);
    //
    p_logo=IMG_LoadPNG_RW(p_rw);
    p_fond_1=IMG_LoadPNG_RW(p_rw_fond_1);
    p_brush=IMG_LoadPNG_RW(p_rw_brush);
    p_brush=SDL_ConvertSurfaceFormat(p_brush, pixel_format, 0);
    p_fond_1=SDL_ConvertSurfaceFormat(p_fond_1, pixel_format, 0);
    SDL_SetColorKey(p_brush, SDL_TRUE, SDL_MapRGB(p_brush->format, 255, 255, 255));
    p_module=Mix_LoadMUS_RW(p_rw_mod, array_module_size);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    //
    Mix_Init(MIX_INIT_MODPLUG);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    lmb=0; first_fl=0;
    //
}

void init_last() {
    //
    p_rw_font=SDL_RWFromMem(array_font, array_font_size);
    TTF_Init();
    font=TTF_OpenFontRW(p_rw_font, 1, 32);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    max_height=TTF_FontHeight(font);
    //
    p_rw_kenet=SDL_RWFromMem(array_darko, array_darko_size);
    p_kenet=IMG_LoadPNG_RW(p_rw_kenet);
    //
    nb_lines=(768-20)/(max_height+5);
    array_car=malloc(nb_lines*200*sizeof(char));
    int i;
    for(i=0; i<nb_lines; i++) {
        array_car[i*200]='\0';
    }
    car_ok=0; delay=0;
    line_char=malloc(200*sizeof(char));
    line_char_2=malloc(200*sizeof(char));
    ind_car_x=0; ind_car_y=0;
    last_chars[0]=' ';
    last_chars[1]=' ';
    last_chars[2]=' ';
    //
    color_omb.r=0; color_omb.g=0; color_omb.b=0;
    color_car.r=RED; color_car.g=GREEN; color_car.b=BLUE;
    p_cursor_2=TTF_RenderText_Blended(font, "_", color_omb);
    w_cursor=p_cursor_2->w; h_cursor=p_cursor_2->h;
     //
    x=10; y=10;
    typing=0; writting=0;
    //
    int red=RED, green=GREEN, blue=BLUE;
    double r=(double)red/(double)2, g=(double)green/(double)2, b=(double)blue/(double)2;
    double angle=0.0;
    double step=(double)2*M_PI/(double)CURVCURS;
    for(i=0; i<CURVCURS; i++) {
        array_curv_curs[i][0]=(int)floor(r*cos(angle)+r+0.5);
        array_curv_curs[i][1]=(int)floor(g*cos(angle)+g+0.5);
        array_curv_curs[i][2]=(int)floor(b*cos(angle)+b+0.5);
        angle+=step;
    }
    index_curv_curs=0;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    //
    tick=0; index_key=0; swap_keyboard=0;
}

void quit() {
    //
    free(line_char);
    free(line_char_2);
    free(array_car);
    TTF_CloseFont(font);
    Mix_FreeMusic(p_module);
    Mix_Quit();
    Mix_CloseAudio();
    SDL_DestroyRenderer(p_renderer);
    //SDL_RWclose(p_rw_font);
    SDL_RWclose(p_rw_kenet);
    SDL_RWclose(p_rw);
    SDL_FreeSurface(p_kenet);
    SDL_FreeSurface(p_fond_12);
    SDL_FreeSurface(p_cursor_2);
    SDL_DestroyWindow(p_window);
    IMG_Quit();
    SDL_Quit();
}
