#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "math.h"

#include "datas.h"
#define CURVCURS 60
#define RED 30
#define GREEN 134
#define BLUE 6
#define WIDTH 1024
#define HEIGHT 768
#define NB_RECORDS 100000

extern unsigned char array_brush[];
extern int array_brush_size;
extern unsigned char array_fond_1[];
extern unsigned int array_fond_1_size;
extern unsigned char array_logo[];
extern unsigned int array_logo_size;
extern unsigned char array_module[];
extern unsigned int array_module_size;
extern int array_coords[];

SDL_Window *p_window;
SDL_RWops *p_rw;
SDL_RWops *p_rw_mod;
SDL_RWops *p_rw_fond_1;
SDL_RWops *p_rw_brush;
SDL_RWops *p_rw_font;
SDL_RWops *p_rw_kenet;
SDL_Surface *p_logo;
SDL_Surface *p_fond_1;
SDL_Surface *p_fond_12;
SDL_Surface *p_brush;
SDL_Surface *p_kenet;
SDL_Surface *p_cursor;
SDL_Surface *p_cursor_2;
SDL_Surface *p_car;
SDL_Surface *p_car_2;
SDL_Renderer *p_renderer;
SDL_Texture *p_texture;
SDL_Texture *p_texture_logo;
SDL_Texture *p_texture_fond_1;
SDL_Texture *p_texture_brush;
SDL_Texture *p_texture_kenet;
SDL_Texture *p_texture_fond_blanc;
SDL_Texture *p_texture_cursor;
SDL_Texture *p_texture_cursor_2;
SDL_Texture *p_texture_car;
SDL_Texture *p_texture_car_2;
SDL_Color color_car;
SDL_Color color_omb;
SDL_Color color1, color2;
Mix_Music *p_module;
SDL_Event event;
//
TTF_Font *font;
//
int lmb;
int index_draw;
int step_wait_coords;
int x, y, prevX, prevY;
Uint32 pixel_format;
// last part
int first_fl;
int x, y, w_cursor, h_cursor, width, height, max_height, nb_lines;
int array_curv_curs[CURVCURS][3];
int index_curv_curs;
char* array_car;
char* line_char;
char* line_char_2;
char last_chars[3];
char actual_char;
int ind_car_x, ind_car_y;
int typing, writting;
int tick;
int car_ok, delay;
int index_key;
int swap_keyboard;

//
void init();
void intro();
void quit();
void first_part();
void init_first_part();
void quit_first_part();
void swap_screen_1();
void test_lmb();
void draw();
//
void init_last();
void last_part();
void flash_screen();
void display_kenet();
void display_cursor();
void init_coords();
void scan_keys();
void treat_car_ok();
void key_backspace();
void key_up();
void key_right();
void key_down();
void key_left();
void key_enter();
void key_delete();
int width_ok();
void record_char();
void display_text(SDL_Color color1, SDL_Color color2, int alpha);
void test_cls();
void test_swap_keyboard();
void smooth_quit();
#endif // FUNCTIONS_H_INCLUDED
