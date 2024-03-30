/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 30-03-2024 16:53:07
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <ncurses.h> 

#include "./engine/engine.h"
#include "./utils/math.h"

bool has_screen_modified_size(screen* s, struct winsize w){
    ioctl(0, TIOCGWINSZ, &w);
    return s->height != w.ws_row-1 || s->width != w.ws_col;
}

int main(){
    // Initialises keyboard binding
    initscr();

    // Gets shell size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    screen* scr = init_screen(w.ws_col, w.ws_row-1);

    scene* s = init_scene(1);

    // Init Triangle
    vec3 v1 = {-0.5, 2, -0.5};
    vec3 v2 = {0,    2,  0.5};
    vec3 v3 = {0.5,  2, -0.5};

    triangle3D t = {v1, v2, v3};
    
    triangle3D* mesh = calloc(1, sizeof(triangle3D));
    mesh[0] = t;

    addMesh(s, mesh, 1);
    
    while(true){   
        if (has_screen_modified_size(scr, w)){
            ioctl(0, TIOCGWINSZ, &w);
            change_size_screen(scr, w.ws_col, w.ws_row-1);
        }

        render(scr, s);
        
        moveCamera(s->cam, getchar());
    }

    free_screen(scr);
    free_scene(s);
    
    return 0;
}