/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 03-03-2024 12:51:23
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
    // Initialises keyboard binding
    initscr();

    // Gets shell size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    screen* s = init_screen(w.ws_col, w.ws_row-1);
    
    camera* cam = init_camera(0, 0, 0);
    
    while(true){   
        if (has_screen_modified_size(s, w)){
            ioctl(0, TIOCGWINSZ, &w);
            change_size_screen(s, w.ws_col, w.ws_row-1);
        }

        clear_screen(s);
        vec3 v1 = {-0.5, -0.5, 1};
        vec3 v2 = {0,     0.5, 1};
        vec3 v3 = {0.5,  -0.5, 1};
        
        triangle3D t = {v1, v2, v3};
        t.v1 = sub_vec3(t.v1, cam->pos);
        t.v2 = sub_vec3(t.v2, cam->pos);
        t.v3 = sub_vec3(t.v3, cam->pos);
        triangle2D t2 = projection_triangle3D(t);
        
        draw_triangle2D(s, triangle2D_to_screen(s, t2));
        update_screen(s);
        printf("Position camera : ");print_vec3(cam->pos);
        moveCamera(cam, getchar());
    }

    free_screen(s);
    return 0;
}