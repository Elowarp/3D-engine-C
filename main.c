/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 05-05-2024 18:52:45
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <ncurses.h> 
#include <time.h>
#include <errno.h>  

#include "./engine/engine.h"
#include "./utils/math.h"
#include "./utils/data_structures.h"

// From 
// https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

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
    scene* s = init_scene();

    vec3 pos = {0, -5, 0};
    s->cam->pos = pos;

    // Creates a light source
    lightSource l;
    vec3 lv = {40, -200, 100};
    l.pos = lv;
    
    // Imports a mesh from the disk
    mesh my_mesh = loadObj("objects/cube&sphere.obj");
    addMesh(s, my_mesh);
    
    // Main loop
    while(true){   
        if (has_screen_modified_size(scr, w)){
            ioctl(0, TIOCGWINSZ, &w);
            change_size_screen(scr, w.ws_col, w.ws_row-1);
        }

        render(scr, s, l);
        
        rotate_mesh_around_z(&my_mesh, 7);
        msleep(300);

        //moveCamera(s->cam, getchar());
    }

    free_mesh(my_mesh);
    free_screen(scr);
    free_scene(s);
    
    return 0;
}