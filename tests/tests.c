/*
 *  Name : Elowan
 *  Creation : 01-01-2024 15:08:15
 *  Last modified : 05-05-2024 16:27:37
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>

#include "../utils/data_structures.h"
#include "../engine/engine.h"
#include "../utils/math.h"

EXPORT_RESIZABLE(int, int)
DEFINE_RESIZABLE(int, int)

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
    
    vec2 v_out = {1, 0};
    vec2 v_in = {0, 0};
    vec2 v_vertex = {0, 0.7};
    vec2 v_edge = {0, -0.5};
    printf("Is the point (1, 0) in the triangle ? : %b (should be 0)\n",
        is_point_inside_triangle(t, v_out));
    printf("Is the point (0, 0) in the triangle ? : %b (should be 1)\n", 
        is_point_inside_triangle(t, v_in));
    printf("Is the vertex point (0, 0.7) in the triangle ? : %b (should be 1)\n",
        is_point_inside_triangle(t, v_vertex));
    printf("Is the edge point (0, -0.5) in the triangle ? : %b (should be 1)\n",
        is_point_inside_triangle(t, v_edge));
        
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

void test_sorting_array(){
    printf("--- Tests resizable array ---\n");
    int n = 5;
    int* keys = calloc(n, sizeof(int));
    double* values = calloc(n, sizeof(double));

    keys[0] = 0;
    keys[1] = 9;
    keys[2] = 6;
    keys[3] = 7;
    keys[4] = 10;

    values[0] = 0;
    values[1] = 16;
    values[2] = 4.5;
    values[3] = 4.6;
    values[4] = 15.9;

    sortArrayDesc(keys, values, n, sizeof(int));
    printf("Tri du tableau [0, 9, 6, 7, 10] par le tableau de valeurs [0, 16, 4.5, 4.6, 15.9]\n");
    printf("Tableau : \n");
    for(int i = 0; i < n; i++){
        printf("%d ", keys[i]);
    }
    printf("\nValeurs : \n");
    for(int i = 0; i < n; i++){
        printf("%f ", values[i]);
    }
    printf("\n");
    printf("(Sould be [9, 10, 7, 6, 0] and [16, 15.9, 4.6, 4.5, 0])\n\n");
    free(keys);
    free(values);
}

int main(){
    test_triangles();
    test_resizable_array();
    test_sorting_array();
    return 0;
}