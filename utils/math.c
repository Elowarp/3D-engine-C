/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 03-03-2024 13:18:07
 */
#include <stdio.h>
#include <stdbool.h>

#include "math.h"

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

vec2 mul_vec2(vec2 v, int s){
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
    printf("{x: %.2f, y: %.2f}\n", v.x, v.y);
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

vec3 mul_vec3(vec3 v, int s){
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

vec2 projection_vec3(vec3 v){
    if (v.z == 0) return UNDEFINED_VEC2;
    
    vec2 r = {v.x/v.z, v.y/v.z};
    return r;
}

void print_vec3(vec3 v){
    printf("{x: %.2f, y: %.2f, z: %.2f}\n", v.x, v.y, v.z);
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

triangle2D projection_triangle3D(triangle3D t){
    triangle2D r = {
        projection_vec3(t.v1),
        projection_vec3(t.v2),
        projection_vec3(t.v3),
    };
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