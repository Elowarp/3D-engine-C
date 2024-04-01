/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 01-04-2024 16:18:18
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
    // Initializes keyboard binding
    initscr();

    // Gets shell size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    
    // Engine initialization
    screen* scr = init_screen(w.ws_col, w.ws_row-1);
    scene* s = init_scene(60);

    // Creates a light source
    lightSource l;
    vec3 lv = {5, 6, -3};
    l.pos = lv;
    
    // Imports a cube from the disk
    int n;
    triangle3D* cube = loadObj("objects/cube.obj", &n);
    addMesh(s, cube, n);
    
    // Main loop
    while(true){   
        if (has_screen_modified_size(scr, w)){
            ioctl(0, TIOCGWINSZ, &w);
            change_size_screen(scr, w.ws_col, w.ws_row-1);
        }

        render(scr, s, l);
        
        moveCamera(s->cam, getchar());
    }

    free_screen(scr);
    free_scene(s);
    
    return 0;
}