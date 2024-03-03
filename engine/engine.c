/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:49:50
 *  Last modified : 03-03-2024 13:17:52
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "engine.h"

extern const vec2 UNDEFINED_VEC2;
extern const vec3 UNDEFINED_VEC3;

const char DEFAULT_CHAR = ' ';
const float ASPECT_RATIO_CHARACTER_SHELL = 1.5;

screen* init_screen(int width, int height){
    screen* s = calloc(1, sizeof(screen));
    s->height = height;
    s->width = width;
    s->pixels = calloc(width*height, sizeof(char));
    for(int i=0; i<width*height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
    return s;
}

void change_size_screen(screen* s, int width, int height){
    free(s->pixels);
    s->width = width;
    s->height = height;
    s->pixels = calloc(width*height, sizeof(char)); 
    for(int i=0; i<width*height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
}

void clear_screen(screen *s){
    for(int i=0; i<s->width*s->height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
    printf("\e[1;1H\e[2J");

}

void update_screen(screen *s){
    // Write the new image
    for(int i=0; i<s->width*s->height; i++){
        printf("%c", s->pixels[i]);
    }
    printf("\n");
}

void change_pixel(screen *s, vec2 v, char c){
    // Checks if the pixel can be changed within the bondaries
    if (v.x>=0 && v.x<s->width && v.y>=0 && v.y<s->height)
        s->pixels[((int) v.y)*s->width + (int)(v.x)] = c;
}

vec2 vec2_to_screen(screen *s, vec2 v){
    // Normalisation
    v.x = ASPECT_RATIO_CHARACTER_SHELL * ((float) s->height/ (float) s->width) * v.x + 1.0;
    v.y = -v.y + 1.0;

    // To screen size
    v.x = v.x * (float) s->width;
    v.y = v.y * (float) s->height;

    v = div_vec2(v, 2.0);

    return v;
}

triangle2D triangle2D_to_screen(screen *s, triangle2D t){
    if (is_undefined_triangle2D(t)) return t;
    
    t.v1 = vec2_to_screen(s, t.v1);
    t.v2 = vec2_to_screen(s, t.v2);
    t.v3 = vec2_to_screen(s, t.v3);

    return t;
}

void draw_triangle2D(screen *s, triangle2D t){
    // If the triangle is not correct, we don't draw it
    if (is_undefined_triangle2D(t)) return;
    
    // Definition of the max zone where the pixel can be drawn
    vec2 br = get_bottom_right_corner(t);
    vec2 tl = get_top_left_corner(t);

    vec2 dims = sub_vec2(br, tl);

    for(int i=0; i <= dims.y; i++){
        for(int j=0; j <= dims.x; j++){
            vec2 pixelCoords = {tl.x+j, tl.y+i};
            
            if (is_point_inside_triangle(t, pixelCoords))
                change_pixel(s, pixelCoords, '@');
        }
    }
}

void free_screen(screen* s){
    free(s->pixels);
    free(s);
}

camera* init_camera(int x, int y, int z){
    camera *cam = calloc(1, sizeof(camera));
    cam->pos.x = x;
    cam->pos.y = y;
    cam->pos.z = z;
    return cam;
}

void moveCamera(camera *cam, char command){
    switch (command)    {
    case 'z':
        cam->pos.y -= 0.25;
        break;

    case 's':
        cam->pos.y += 0.25;
        break;

    case 'q':
        cam->pos.x += 0.25;
        break;

    case 'd':
        cam->pos.x -= 0.25;
        break;
    
    case 'a':
        cam->pos.z -= 0.25;
        break;

    case 'e':
        cam->pos.z += 0.25;
        break;

    default:
        break;
    }
}

void free_camera(camera *cam){
    free(cam);
}