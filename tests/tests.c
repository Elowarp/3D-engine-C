/*
 *  Name : Elowan
 *  Creation : 01-01-2024 15:08:15
 *  Last modified : 29-04-2024 18:18:19
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>

#include "../engine/engine.h"
#include "../utils/math.h"
#include "../utils/data_structures.h"

DEFINE_RESIZABLE(int)

void test_triangles(){
    printf("--- Tests triangles ---\n");
    // Gets shell size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    screen* s = init_screen(w.ws_col, w.ws_row-1);
    printf("Shell size : %d %d\n", w.ws_col, w.ws_col-1);
    
    vec2 v1 = {-0.9, -0.5};
    vec2 v2 = {0, 0.7};
    vec2 v3 = {0.9, -0.5};

    triangle2D t = {v1, v2, v3};
    printf("Triangle (-0.9, -0.5) (0, 0.7) (0.9, -0.5)\n\n");
    
    printf("Convertion to screen size : \n");
    triangle2D t_screen = triangle2D_to_screen(s, t);
    print_triangle2D(t_screen);

    printf("Square containing the triangle : \n");
    vec2 tl = get_top_left_corner(t_screen);
    printf("Top left corner : %f %f\n", tl.x, tl.y);
    vec2 br = get_bottom_right_corner(t_screen);
    printf("Bottom right corner : %f %f\n\n", br.x, br.y);
    vec2 v = {12, 18};
    printf("Is the point (12, 18) in the triangle ? : %b\n (should be 0)", is_point_inside_triangle(t, v));
    printf("\n");
}

void test_resizable_array(){
    printf("--- Tests resizable array ---\n");
    resizable_array_int a = init_resizable_array_int();
    for(int i = 0; i < 4; i++){
        append_resizbl_arr_int(&a, i+1);
    }

    set_resizbl_arr_int(&a, 2, 5);

    printf("The array should be [1, 2, 5, 4, ] : [");
    for(int i = 0; i < 4; i++){
        printf("%d, ", get_resizbl_arr_int(a, i));
    }
    printf("]\n");

    free_resizbl_arr_int(a);
    
    printf("\n");
}

int main(){
    test_triangles();
    test_resizable_array();
    return 0;
}