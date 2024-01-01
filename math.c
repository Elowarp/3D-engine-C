/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 01-01-2024 15:34:08
 */
#include <stdio.h>
#include <stdbool.h>

#include "math.h"

vec2 sub_vec2(vec2 v1, vec2 v2){
    vec2 r = {v1.x, v1.y};
    r.x = r.x - v2.x;
    r.y = r.y - v2.y;
    return r;
}

void print_vec2(vec2 v){
    printf("{x: %d, y: %d}\n", v.x, v.y);
}

vec2 get_bottom_right_corner(triangle t){
    vec2 br = {t.v1.x, t.v1.y};
    if (t.v2.x > br.x) br.x = t.v2.x;
    if (t.v3.x > br.x) br.x = t.v3.x;

    if (t.v2.y > br.y) br.y = t.v2.y;
    if (t.v3.y > br.y) br.y = t.v3.y;

    return br;
}

vec2 get_top_left_corner(triangle t){
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

bool is_point_inside_triangle(triangle t, vec2 v){
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(v, t.v1, t.v2);
    d2 = sign(v, t.v2, t.v3);
    d3 = sign(v, t.v3, t.v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}