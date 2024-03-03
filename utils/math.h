/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 03-03-2024 20:59:46
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
    double qw;
    double qi;
    double qj;
    double qk;
} quaternion;


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
vec2 mul_vec2(vec2 v, float s);
vec2 div_vec2(vec2 v, int s);
void print_vec2(vec2 v);
bool is_undefined_vec2(vec2 v);

vec3 add_vec3(vec3 v1, vec3 v2);
vec3 sub_vec3(vec3 v1, vec3 v2);
vec3 mul_vec3(vec3 v, float s);
vec3 div_vec3(vec3 v, int s);
double prod_vec3(vec3 v, vec3 u);
void print_vec3(vec3 v);
bool is_undefined_vec3(vec3 v);


bool is_undefined_triangle2D(triangle2D t);
bool is_undefined_triangle3D(triangle3D t);
vec3 project_vec_on_hyperplan(vec3 v, vec3 n);
triangle3D project_triangle_on_hyperplan(triangle3D t, vec3 n);
triangle2D project_triangle3D_to_2D(triangle3D t);

void print_triangle2D(triangle2D t);
void print_triangle3D(triangle3D t);

triangle2D projection_triangle3D(triangle3D t, vec3 n);

vec2 get_bottom_right_corner(triangle2D t);
vec2 get_top_left_corner(triangle2D t);
bool is_point_inside_triangle(triangle2D t, vec2 v);

// Matrices
double** rotation_matrix_x(double angle);
double** rotation_matrix_y(double angle);
double** rotation_matrix_z(double angle);
vec3 multiply_matrix_vector_3D(double** m, vec3 v);
void free_matrix(int n, int **m);

#endif