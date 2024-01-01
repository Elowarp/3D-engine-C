/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:50:05
 *  Last modified : 01-01-2024 15:47:34
 */
#ifndef SCREEN_H
#define SCREEN_H

#include "math.h"

typedef struct {
    int width;
    int height;
    char* pixels;
} screen;

screen* init_screen(int width, int height);
void clear_screen(screen *s);
void update_screen(screen *s);
void change_pixel(screen *s, vec2 v, char c);
void draw_triangle(screen *s, triangle t);
void free_screen(screen *s);

#endif