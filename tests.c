/*
 *  Name : Elowan
 *  Creation : 01-01-2024 15:08:15
 *  Last modified : 01-01-2024 15:24:08
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "engine.h"
#include "math.h"

void test_triangles(){
    printf("--- Tests triangles ---\n");
    vec2 v1 = {2, 10};
    vec2 v2 = {10, 25};
    vec2 v3 = {25, 17};

    triangle t = {v1, v2, v3};
    printf("Triangle (2,10) (10,25) (25,17)\n\n");
    printf("Square containing the triangle : \n");
    vec2 tl = get_top_left_corner(t);
    printf("Top left corner : %d %d\n", tl.x, tl.y);
    vec2 br = get_bottom_right_corner(t);
    printf("Bottom right corner : %d %d\n\n", br.x, br.y);
    vec2 v = {12, 18};
    printf("Is the point (12, 18) in the triangle ? : %b\n", is_point_inside_triangle(t, v));
}

int main(){
    test_triangles();
    return 0;
}