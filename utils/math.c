/*
 *  Name : Elowan
 *  Creation : 01-01-2024 14:21:11
 *  Last modified : 01-04-2024 16:28:24
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "math.h"
#define PI 3.141592654

const double EPSILON = 0.0001;

// 2D Vectors
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

vec2 mul_vec2(vec2 v, double s){
    v.x *= s;
    v.y *= s;
    return v;
}

vec2 div_vec2(vec2 v, double s){
    assert(s!=0);

    v.x /= s;
    v.y /= s;
    return v;
}

void print_vec2(vec2 v){
    printf("{x: %.2f, y: %.2f}", v.x, v.y);
}


// 3D Vectors
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

vec3 mul_vec3(vec3 v, double s){
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

vec3 div_vec3(vec3 v, double s){
    assert(fabs(s)>EPSILON);
    
    v.x /= s;
    v.y /= s;
    v.z /= s;
    return v;
}

/*
Dot product
*/
double dot_vec3(vec3 v, vec3 u){
    return v.x*u.x + v.y*u.y + v.z*u.z;
}

/*
Cross product
*/
vec3 cross_vec3(vec3 v, vec3 u){
    vec3 p = {
        v.y * u.z - v.z * u.y,
        v.z * u.x - v.x * u.z,
        v.x * u.y - v.y * u.x
    };
    
    return p;
}

/*
Returns the normal vector of the surface of a triangle
*/
vec3 normal_surface(triangle3D t){
    vec3 line1 = sub_vec3(t.v2, t.v1);
    vec3 line2 = sub_vec3(t.v3, t.v1);

    vec3 normal = cross_vec3(line2, line1);
    return normal;
}


/*
    Returns a point at the intersection of a plane (described by a point 
    `planePoint` and its normal vector `normal`) and a line (described
    by the two edges `v1` and `v2`)
*/
vec3 point_intersec_plane_line(vec3 normal, vec3 planePoint, vec3 v1, vec3 v2){
    vec3 u = sub_vec3(v2, v1);
    double dp = dot_vec3(u, normal);

    // If parallel then no intersection
    if (fabs(dp) < EPSILON){
        vec3 null = {0, 0, 0};
        return null;
    } 

    vec3 w = sub_vec3(v1, planePoint);
    double si = -dot_vec3(w, normal)/dp;

    u = mul_vec3(u, si);
    return add_vec3(v1, u);     
}

vec2 projection_to_2D(vec3 v){
    // Prevents the division by 0
    assert (fabs(v.y) > EPSILON);

    vec2 res = {v.x/v.y, v.z/v.y};
    return res;
}

void print_vec3(vec3 v){
    printf("{x: %.2f, y: %.2f, z: %.2f}", v.x, v.y, v.z);
}

vec3 normalize(vec3 v){
    double norm = sqrt(dot_vec3(v, v));
    return div_vec3(v, norm);
}

// Triangles
void print_triangle2D(triangle2D t){
    printf("(%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f)\n",
            t.v1.x, t.v1.y, t.v2.x, t.v2.y, t.v3.x, t.v3.y);
}

void print_triangle3D(triangle3D t){
    printf("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n",
        t.v1.x, t.v1.y, t.v1.z, t.v2.x, t.v2.y, t.v2.z, t.v3.x, t.v3.y, t.v3.z);
}

triangle2D project_triangle3D_to_2D(triangle3D t){
    triangle2D r = {
        projection_to_2D(t.v1),
        projection_to_2D(t.v2),
        projection_to_2D(t.v3),
    };

    return r;
}


// Matrices
double calc_coef(double** m, double** n, int i, int j, int k){
    double s = 0;
    for(int q = 0; q<j; q++){
        s += m[i][q]*n[q][k];
    }
    return s;
}

double** mul_matrix(double** m, double** n, int i, int j, int k){
    // m * n
    double** r = calloc(i, sizeof(double*));
    for(int q=0; q<i; q++){
        r[q] = calloc(k, sizeof(double));
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

double** transpose(double** t, int n){
    double** m = calloc(n, sizeof(double*));
    for(int i=0; i<n; i++){
        m[i] = calloc(n, sizeof(double));
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            m[i][j] = t[j][i];
        }
    }

    return m;
}

/*
Returns the transition matrix (which is orthogonal) of the canonic base into the 
(e1, e2, e3) base
*/
double** transition_matrix(vec3 e1, vec3 e2, vec3 e3){
    double** m = calloc(3, sizeof(double*));
    for(int i=0; i<3; i++){
        m[i] = calloc(3, sizeof(double));
    }

    m[0][0] = e1.x;
    m[1][0] = e1.y;
    m[2][0] = e1.z;

    m[0][1] = e2.x;
    m[1][1] = e2.y;
    m[2][1] = e2.z;

    m[0][2] = e3.x;
    m[1][2] = e3.y;
    m[2][2] = e3.z;

    m = transpose(m, 3);
    return m;
}

vec3 multiply_matrix_vector_3D(double** m, vec3 v){
    vec3 p;
    p.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
    p.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
    p.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
    return p;
}

void print_matrix(int rows, int cols, double** matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(int n, double **m){
    for(int i = 0; i<n; i++) free(m[i]);
    free(m);
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


float sign (vec2 v1, vec2 v2, vec2 v3){
    return (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
}

// Solution from 
// https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
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
