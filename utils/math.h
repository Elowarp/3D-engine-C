/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 03-03-2024 13:18:02
 */

#ifndef MATH_H
#define MATH_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
} vec2;


typedef struct {
    float x;
    float y;
    float z;
} vec3;


typedef struct {
    vec2 v1;
    vec2 v2;
    vec2 v3;
} triangle2D;

typedef struct {
    vec3 v1;
    vec3 v2;
    vec3 v3;
} triangle3D;

vec2 add_vec2(vec2 v1, vec2 v2);
vec2 sub_vec2(vec2 v1, vec2 v2);
vec2 mul_vec2(vec2 v, int s);
vec2 div_vec2(vec2 v, int s);
void print_vec2(vec2 v);
bool is_undefined_vec2(vec2 v);

vec3 add_vec3(vec3 v1, vec3 v2);
vec3 sub_vec3(vec3 v1, vec3 v2);
vec3 mul_vec3(vec3 v, int s);
vec3 div_vec3(vec3 v, int s);
void print_vec3(vec3 v);
bool is_undefined_vec3(vec3 v);

bool is_undefined_triangle2D(triangle2D t);
bool is_undefined_triangle3D(triangle3D t);
vec2 projection_vec3(vec3 v);

void print_triangle2D(triangle2D t);
triangle2D projection_triangle3D(triangle3D t);

vec2 get_bottom_right_corner(triangle2D t);
vec2 get_top_left_corner(triangle2D t);
bool is_point_inside_triangle(triangle2D t, vec2 v);

#endif