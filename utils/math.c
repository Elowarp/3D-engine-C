/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 03-03-2024 20:59:32
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "math.h"
#define PI 3.141592654

const vec2 UNDEFINED_VEC2 = {__FLT_MAX__, __FLT_MAX__};
const vec3 UNDEFINED_VEC3 = {__FLT_MAX__, __FLT_MAX__, __FLT_MAX__};


/*
    Vectors 2 dimensions
*/
vec2 add_vec2(vec2 v1, vec2 v2){
    vec2 r = {v1.x, v1.y};
    r.x = r.x + v2.x;
    r.y = r.y + v2.y;
    return r;
}

vec2 sub_vec2(vec2 v1, vec2 v2){
    vec2 r = {v1.x, v1.y};
    r.x = r.x - v2.x;
    r.y = r.y - v2.y;
    return r;
}

vec2 mul_vec2(vec2 v, float s){
    v.x *= s;
    v.y *= s;
    return v;
}

vec2 div_vec2(vec2 v, int s){
    if (s==0) return UNDEFINED_VEC2;

    v.x /= s;
    v.y /= s;
    return v;
}

void print_vec2(vec2 v){
    printf("{x: %.2f, y: %.2f}", v.x, v.y);
}

bool is_undefined_vec2(vec2 v){
    return v.x == UNDEFINED_VEC2.x || v.y == UNDEFINED_VEC2.y; 
}



/*
    Vectors 3 dimensions
*/
vec3 add_vec3(vec3 v1, vec3 v2){
    vec3 r = {v1.x, v1.y, v1.z};
    r.x = r.x + v2.x;
    r.y = r.y + v2.y;
    r.z = r.z + v2.z;

    return r;
}

vec3 sub_vec3(vec3 v1, vec3 v2){
    vec3 r = {v1.x, v1.y, v1.z};
    r.x = r.x - v2.x;
    r.y = r.y - v2.y;
    r.z = r.z - v2.z;
    return r;
}

vec3 mul_vec3(vec3 v, float s){
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

vec3 div_vec3(vec3 v, int s){
    if (s==0) return UNDEFINED_VEC3;
    
    v.x /= s;
    v.y /= s;
    v.z /= s;
    return v;
}

double prod_vec3(vec3 v, vec3 u){
    // Scalar product
    return v.x*u.x + v.y*u.y + v.z+u.z;
}


vec3 project_vec_on_hyperplan(vec3 v, vec3 n){    
    vec3 u = sub_vec3(v, n);
    float dist = prod_vec3(u, n);
    vec3 res = sub_vec3(v, mul_vec3(n, dist));
    return res;
}

vec2 projection_to_2D(vec3 v){
    if (v.z == 0) return UNDEFINED_VEC2;
    vec2 res = {v.x/v.z, v.y/v.z};
    return res;
}

void print_vec3(vec3 v){
    printf("{x: %.2f, y: %.2f, z: %.2f}", v.x, v.y, v.z);
}

bool is_undefined_vec3(vec3 v){
    return v.x == UNDEFINED_VEC3.x || v.y == UNDEFINED_VEC3.y 
        || v.z == UNDEFINED_VEC3.z; 
}

bool is_undefined_triangle2D(triangle2D t){
    return is_undefined_vec2(t.v1) || is_undefined_vec2(t.v2)
        || is_undefined_vec2(t.v3);
}

bool is_undefined_triangle3D(triangle3D t){
    return is_undefined_vec3(t.v1) || is_undefined_vec3(t.v2)
        || is_undefined_vec3(t.v3);
}


void print_triangle2D(triangle2D t){
    printf("(%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f)\n",
        t.v1.x, t.v1.y, t.v2.x, t.v2.y, t.v3.x, t.v3.y);
}

void print_triangle3D(triangle3D t){
    printf("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n",
        t.v1.x, t.v1.y, t.v1.z, t.v2.x, t.v2.y, t.v2.z, t.v3.x, t.v3.y, t.v3.z);
}

triangle3D project_triangle_on_hyperplan(triangle3D t, vec3 n){
    triangle3D r = {
        project_vec_on_hyperplan(t.v1, n),
        project_vec_on_hyperplan(t.v2, n),
        project_vec_on_hyperplan(t.v3, n),
    };
    return r;
}

triangle2D project_triangle3D_to_2D(triangle3D t){
    triangle2D r = {
        projection_to_2D(t.v1),
        projection_to_2D(t.v2),
        projection_to_2D(t.v3)
    };
    return r;
}

int calc_coef(int** m, int** n, int i, int j, int k){
    int s = 0;
    for(int q = 0; q<j; q++){
        s += m[i][q]*n[q][k];
    }
    return s;
}

int** mul_matrix(int** m, int** n, int i, int j, int k){
    // m * n
    int** r = calloc(i, sizeof(int*));
    for(int q=0; q<i; q++){
        r[q] = calloc(k, sizeof(int));
    }

    for(int l = 0; l<i; l++){
        for(int c = 0; c<k; c++){
            r[l][c] = calc_coef(m, n, l, j, c);
        }
    }
    return r;
}

double** rotation_matrix_x(double angle){
    double radian = angle*PI/180;
    double** r = calloc(3, sizeof(int*));
    for(int i=0; i<3; i++){
        r[i] = calloc(3, sizeof(int));
    }

    r[0][0] = 1;
    r[1][1] = cos(radian);
    r[1][2] = -sin(radian);
    r[2][1] = sin(radian);
    r[2][2] = cos(radian);

    return r;
}

double** rotation_matrix_y(double angle){
    double radian = angle*PI/180;
    double** r = calloc(3, sizeof(double*));
    for(int i=0; i<3; i++){
        r[i] = calloc(3, sizeof(double));
    }

    r[0][0] = cos(radian);
    r[0][2] = -sin(radian);
    r[1][1] = 1;
    r[2][0] = sin(radian);
    r[2][2] = cos(radian);

    return r;
}

double** rotation_matrix_z(double angle){
    double radian = angle*PI/180;
    double** r = calloc(3, sizeof(double*));
    for(int i=0; i<3; i++){
        r[i] = calloc(3, sizeof(double));
    }

    r[0][0] = cos(radian);
    r[0][1] = -sin(radian);
    r[1][0] = sin(radian);
    r[1][1] = cos(radian);
    r[2][2] = 1;

    return r;
}

vec2 get_bottom_right_corner(triangle2D t){
    vec2 br = {t.v1.x, t.v1.y};
    if (t.v2.x > br.x) br.x = t.v2.x;
    if (t.v3.x > br.x) br.x = t.v3.x;

    if (t.v2.y > br.y) br.y = t.v2.y;
    if (t.v3.y > br.y) br.y = t.v3.y;

    return br;
}

vec2 get_top_left_corner(triangle2D t){
    vec2 tl = {t.v1.x, t.v1.y};
    if (t.v2.x < tl.x) tl.x = t.v2.x;
    if (t.v3.x < tl.x) tl.x = t.v3.x;

    if (t.v2.y < tl.y) tl.y = t.v2.y;
    if (t.v3.y < tl.y) tl.y = t.v3.y;

    return tl;
}


// Solution from 
// https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
float sign (vec2 v1, vec2 v2, vec2 v3){
    return (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
}

bool is_point_inside_triangle(triangle2D t, vec2 v){
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(v, t.v1, t.v2);
    d2 = sign(v, t.v2, t.v3);
    d3 = sign(v, t.v3, t.v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

vec3 multiply_matrix_vector_3D(double** m, vec3 v){
    vec3 p;
    p.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
    p.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
    p.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
    return p;
}

void free_matrix(int n, int **m){
    for(int i = 0; i<n; i++) free(m[i]);
    free(m);
}