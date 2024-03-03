/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:50:05
 *  Last modified : 03-03-2024 22:58:13
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
    vec3 v1;
    vec3 v2; // Where the camera is looking
    vec3 v3;
} camera;

typedef struct {
    int n;
    camera* cam;
    triangle3D* objects;
} scene;

// Screen
screen* init_screen(int width, int height);
void change_size_screen(screen* s, int width, int height);
void clear_screen(screen *s);
void update_screen(screen *s);
void change_pixel(screen *s, vec2 v, char c);
vec2 vec2_to_screen(screen *s, vec2 v);
triangle2D triangle2D_to_screen(screen *s, triangle2D t);
void draw_triangle2D(screen *s, triangle2D t);
void free_screen(screen *s);

// Camera
camera* init_camera();
void moveCamera(camera *cam, char command);
void print_camera_infos(camera* cam);
void free_camera(camera *cam);

// Scene
scene* init_scene(int n);
void render(screen* scr, scene* s);
void free_scene(scene* s);

#endif