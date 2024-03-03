/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:50:05
 *  Last modified : 03-03-2024 13:17:57
 */
#ifndef SCREEN_H
#define SCREEN_H

#include "../utils/math.h"

typedef struct {
    int width;
    int height;
    char* pixels;
} screen;

typedef struct {
    vec3 pos;
} camera;

screen* init_screen(int width, int height);
void change_size_screen(screen* s, int width, int height);
void clear_screen(screen *s);
void update_screen(screen *s);
void change_pixel(screen *s, vec2 v, char c);
vec2 vec2_to_screen(screen *s, vec2 v);
triangle2D triangle2D_to_screen(screen *s, triangle2D t);
void draw_triangle2D(screen *s, triangle2D t);
void free_screen(screen *s);

camera* init_camera(int x, int y, int z);
void moveCamera(camera *cam, char command);
void free_camera(camera *cam);

#endif