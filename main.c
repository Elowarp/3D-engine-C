/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:44:27
 *  Last modified : 01-01-2024 15:56:25
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <time.h>
#include <errno.h>   

#include "engine.h"
#include "math.h"

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


int main(){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    screen* s = init_screen(w.ws_col, w.ws_row);
    
    while(true){
        clear_screen(s);
        vec2 v1 = {10, 10};
        vec2 v2 = {10, 28};
        vec2 v3 = {25, 22};

        triangle t = {v1, v2, v3};
        draw_triangle(s, t);
        update_screen(s);
        msleep(300);
    }

    free(s);
    return 0;
}