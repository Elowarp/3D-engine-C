/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 04-05-2024 20:45:46
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

// Orientation convention : Clockwise direction 
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

// 2D Vector
vec2 add_vec2(vec2 v1, vec2 v2);
vec2 sub_vec2(vec2 v1, vec2 v2);
vec2 mul_vec2(vec2 v, double s);
vec2 div_vec2(vec2 v, double s);
void print_vec2(vec2 v);

// 3D Vector
vec3 add_vec3(vec3 v1, vec3 v2);
vec3 sub_vec3(vec3 v1, vec3 v2);
vec3 mul_vec3(vec3 v, double s);
vec3 div_vec3(vec3 v, double s);
double dot_vec3(vec3 v, vec3 u);
vec3 cross_vec3(vec3 v, vec3 u);
float lengthVec3(vec3 v);
vec3 normal_surface(triangle3D t);
vec3 point_intersec_plane_line(vec3 normal, vec3 planePoint, vec3 v1, vec3 v2);
void print_vec3(vec3 v);
vec3 normalize(vec3 v);

// 2D Triangle
triangle2D project_triangle3D_to_2D(triangle3D t);
void print_triangle2D(triangle2D t);

// 3D Triangle
void print_triangle3D(triangle3D t);
float distTriangleVec3(triangle3D t, vec3 v);

// Utils triangles
vec2 get_bottom_right_corner(triangle2D t);
vec2 get_top_left_corner(triangle2D t);
bool is_point_inside_triangle(triangle2D t, vec2 v);

// Matrices
double** rotation_matrix_x(double angle);
double** rotation_matrix_y(double angle);
double** rotation_matrix_z(double angle);
double** mul_matrix(double** m, double** n, int i, int j, int k);
double** transpose(double** t, int n);
double** transition_matrix(vec3 e1, vec3 e2, vec3 e3);
vec3 multiply_matrix_vector_3D(double** m, vec3 v);
void print_matrix(int rows, int cols, double** matrix);
void free_matrix(int n, double **m);

#endif