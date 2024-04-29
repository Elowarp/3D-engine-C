/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 29-04-2024 19:27:48
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <ncurses.h> 

#include "./engine/engine.h"
#include "./utils/math.h"
#include "./utils/data_structures.h"

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
    scene* s = init_scene(256);

    // Creates a light source
    lightSource l;
    vec3 lv = {5, 6, -3};
    l.pos = lv;
    
    // Imports a cube from the disk
    resizable_array_t3D cube = loadObj("objects/cube.obj");
    addMesh(s, cube);
    
    // Main loop
    while(true){   
        if (has_screen_modified_size(scr, w)){
            ioctl(0, TIOCGWINSZ, &w);
            change_size_screen(scr, w.ws_col, w.ws_row-1);
        }

        render(scr, s, l);
        
        moveCamera(s->cam, getchar());
    }

    free_resizbl_arr_t3D(cube);
    free_screen(scr);
    free_scene(s);
    
    return 0;
}