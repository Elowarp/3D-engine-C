/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:49:50
 *  Last modified : 06-03-2024 20:54:20
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "engine.h"

extern const vec2 UNDEFINED_VEC2;
extern const vec3 UNDEFINED_VEC3;

const char DEFAULT_CHAR = ' ';
const float ASPECT_RATIO_CHARACTER_SHELL = 1.5;

const float ANGLE_ROTATION = 10;


// Screens
screen* init_screen(int width, int height){
    screen* s = calloc(1, sizeof(screen));
    s->height = height;
    s->width = width;
    s->pixels = calloc(width*height, sizeof(char));
    for(int i=0; i<width*height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
    return s;
}

void change_size_screen(screen* s, int width, int height){
    free(s->pixels);
    s->width = width;
    s->height = height;
    s->pixels = calloc(width*height, sizeof(char)); 
    for(int i=0; i<width*height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
}

void clear_screen(screen *s){
    for(int i=0; i<s->width*s->height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
    printf("\e[1;1H\e[2J");

}

void update_screen(screen *s){
    // Write the new image
    for(int i=0; i<s->width*s->height; i++){
        printf("%c", s->pixels[i]);
    }
    printf("\n");
}

void change_pixel(screen *s, vec2 v, char c){
    // Checks if the pixel can be changed within the bondaries
    if (v.x>=0 && v.x<s->width && v.y>=0 && v.y<s->height)
        s->pixels[((int) v.y)*s->width + (int)(v.x)] = c;
}

vec2 vec2_to_screen(screen *s, vec2 v){
    // Normalization
    v.x = ASPECT_RATIO_CHARACTER_SHELL * ((float) s->height/ (float) s->width) * v.x + 1.0;
    v.y = -v.y + 1.0;

    // To screen size
    v.x = v.x * (float) s->width;
    v.y = v.y * (float) s->height;

    v = div_vec2(v, 2.0);

    return v;
}

triangle2D triangle2D_to_screen(screen *s, triangle2D t){
    if (is_undefined_triangle2D(t)){
        return t;
    }
    
    t.v1 = vec2_to_screen(s, t.v1);
    t.v2 = vec2_to_screen(s, t.v2);
    t.v3 = vec2_to_screen(s, t.v3);

    return t;
}

void draw_triangle2D(screen *s, triangle2D t){
    // If the triangle is not correct, we don't draw it
    if (is_undefined_triangle2D(t)) return;
    
    // Definition of the max zone where the pixel can be drawn
    vec2 br = get_bottom_right_corner(t);
    vec2 tl = get_top_left_corner(t);

    vec2 dims = sub_vec2(br, tl);

    for(int i=0; i <= dims.y; i++){
        for(int j=0; j <= dims.x; j++){
            vec2 pixelCoords = {tl.x+j, tl.y+i};
            
            if (is_point_inside_triangle(t, pixelCoords))
                change_pixel(s, pixelCoords, '@');
        }
    }
}

void free_screen(screen* s){
    free(s->pixels);
    free(s);
}


// Cameras
camera* init_camera(){
    camera *cam = calloc(1, sizeof(camera));
    vec3 pos = {0, 0, 0};
    
    vec3 v1 = {1, 0, 0};
    vec3 v2 = {0, 1, 0};
    vec3 v3 = {0, 0, 1};
    
    cam->pos = pos;
    cam->v1 = v1;
    cam->v2 = v2;
    cam->v3 = v3;
    return cam;
}

void rotate_camera_around_z(camera* c, int sign){
    // Vector and rotation matrix multiplication
    double** r = rotation_matrix_z(sign*ANGLE_ROTATION);

    c->v1 = multiply_matrix_vector_3D(r, c->v1); 
    c->v2 = multiply_matrix_vector_3D(r, c->v2);
    c->v3 = multiply_matrix_vector_3D(r, c->v3);

    free_matrix(3, r);
}

void moveCamera(camera *cam, char command){
    switch (command)    {
    case 'z':
        cam->pos = add_vec3(cam->pos, mul_vec3(cam->v2, 0.25));
        break;

    case 's':
        cam->pos = add_vec3(cam->pos, mul_vec3(cam->v2, -0.25));
        break;

    case 'q':
        cam->pos = add_vec3(cam->pos, mul_vec3(cam->v1, -0.25));
        break;

    case 'd':
        cam->pos = add_vec3(cam->pos, mul_vec3(cam->v1, 0.25));
        break;
    
    case 'a':
        cam->pos = add_vec3(cam->pos, mul_vec3(cam->v3, 0.25));
        break;

    case 'e':
        cam->pos = add_vec3(cam->pos, mul_vec3(cam->v3, -0.25));
        break;

    case 'c':
        rotate_camera_around_z(cam, -1);
        break;

    case 'w':
        rotate_camera_around_z(cam, 1);
        break;

    case 'r':
        vec3 pos = {0, 0, 0};
        vec3 v1 = {1, 0, 0};
        vec3 v2 = {0, 1, 0};
        vec3 v3 = {0, 0, 1};
        
        cam->pos = pos;
        cam->v1 = v1;
        cam->v2 = v2;
        cam->v3 = v3;
        break;

    default:
        break;
    }
}

triangle3D changeReferenceToCamera(camera* cam, triangle3D t){
    /*
    Cast coordinates from the canonic base into the camera base 
    (made of the vectors v1, v2 and v3)
    */
    triangle3D r = t;

    double** p = transition_matrix(cam->v1, cam->v2, cam->v3);

    r.v1 = sub_vec3(r.v1, cam->pos);
    r.v2 = sub_vec3(r.v2, cam->pos);
    r.v3 = sub_vec3(r.v3, cam->pos);

    r.v1 = multiply_matrix_vector_3D(p, r.v1);
    r.v2 = multiply_matrix_vector_3D(p, r.v2);
    r.v3 = multiply_matrix_vector_3D(p, r.v3);

    r.v1 = add_vec3(r.v1, cam->pos);
    r.v2 = add_vec3(r.v2, cam->pos);
    r.v3 = add_vec3(r.v3, cam->pos);

    free_matrix(3, p);
    return r;
}

void print_camera_infos(camera* cam){
    printf("Position camera : ");print_vec3(cam->pos);printf("\n");
    print_vec3(cam->v1);print_vec3(cam->v2);print_vec3(cam->v3);printf("\n");
}

void free_camera(camera *cam){
    free(cam);
}


// Scenes
scene* init_scene(int n){
    scene* s = calloc(1, sizeof(scene));

    s->n = n;
    s->objects = calloc(s->n, sizeof(triangle3D));
    s->cam = init_camera(0, 0, 0);

    return s;
}

void render(screen* scr, scene* s){
    clear_screen(scr);
    
    for(int i = 0; i < s->n; i++){
        // /!\ no distinction between triangles shown and the others
        triangle3D t = changeReferenceToCamera(s->cam, s->objects[i]);
        
        t.v1 = sub_vec3(t.v1, s->cam->pos);
        t.v2 = sub_vec3(t.v2, s->cam->pos);
        t.v3 = sub_vec3(t.v3, s->cam->pos);
                
        // Checks if the triangle is behind the camera or not
        if (t.v1.y <= 0 || t.v2.y <= 0 || t.v3.y <= 0) continue;

        triangle2D t2 = project_triangle3D_to_2D(t);
        draw_triangle2D(scr, triangle2D_to_screen(scr, t2));
    }
    update_screen(scr);
}

void free_scene(scene* s){
    free(s->objects);
    free_camera(s->cam);
    free(s);
}