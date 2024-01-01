/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 01-01-2024 15:34:30
 */

#ifndef MATH_H
#define MATH_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
} vec2;

typedef struct {
    vec2 v1;
    vec2 v2;
    vec2 v3;
} triangle;

vec2 sub_vec2(vec2 v1, vec2 v2);
void print_vec2(vec2 v);
vec2 get_bottom_right_corner(triangle t);
vec2 get_top_left_corner(triangle t);
bool is_point_inside_triangle(triangle t, vec2 v);

#endif