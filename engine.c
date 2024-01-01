/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:49:50
 *  Last modified : 01-01-2024 15:56:53
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "engine.h"
#include "math.h"

const char DEFAULT_CHAR = ' ';

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
}

void change_pixel(screen *s, vec2 v, char c){
    assert(v.x>=0 && v.x<s->width);
    assert(v.y>=0 && v.y<s->height);
    s->pixels[v.y*s->width + v.x] = c;
}

void draw_triangle(screen *s, triangle t){
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