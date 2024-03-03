/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 03-03-2024 21:00:14
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <time.h>
#include <errno.h>  
#include <curses.h> 
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

    vec3 v1 = {-0.5, -0.5, 1};
    vec3 v2 = {0,     0.5, 1};
    vec3 v3 = {0.5,  -0.5, 1};

    triangle3D t = {v1, v2, v3};

    s->objects[0] = t;
    
    while(true){   
        if (has_screen_modified_size(scr, w)){
            ioctl(0, TIOCGWINSZ, &w);
            change_size_screen(scr, w.ws_col, w.ws_row-1);
        }

        clear_screen(scr);
        render(scr, s);

        // print_camera_infos(s->cam);
        
        moveCamera(s->cam, getchar());
    }

    free_screen(scr);
    free_scene(s);
    
    return 0;
}