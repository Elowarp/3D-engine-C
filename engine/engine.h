/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:50:05
 *  Last modified : 05-05-2024 17:48:36
 */
#ifndef SCREEN_H
#define SCREEN_H

#include "../utils/math.h"
#include "../utils/data_structures.h"

EXPORT_RESIZABLE(triangle3D, t3D)
EXPORT_RESIZABLE(char*, str)

typedef struct {
    int width;
    int height;
    char* pixels; // Top left corner is 0, bottom right corner is width*height-1
    double aspect_ratio_character;
} screen;

typedef struct {
    vec3 pos;
    vec3 v1;
    vec3 v2; // Where the camera is looking
    vec3 v3;
} camera;

typedef struct {
    vec3 pos;
} lightSource;

typedef struct {
    vec3 pos;
    triangle3D* triangles;
    int size;
} mesh;

EXPORT_RESIZABLE(mesh, mesh)

typedef struct {
    camera* cam;
    resizable_array_mesh objects;
} scene;

// Screen
screen* init_screen(int width, int height);
void change_size_screen(screen* s, int width, int height);
void clear_screen(screen *s);
void update_screen(screen *s);
void change_pixel(screen *s, vec2 v, char c);
vec2 vec2_to_screen(screen *s, vec2 v);
triangle2D triangle2D_to_screen(screen *s, triangle2D t);
void draw_triangle2D(screen *s, triangle2D t, char c);
void free_screen(screen *s);

// Camera
camera* init_camera();
void moveCamera(camera *cam, char command);
void print_camera_infos(camera* cam);
vec3 getCameraLookingAt(camera* cam);
void free_camera(camera *cam);

// Scene
scene* init_scene();
void render(screen* scr, scene* s, lightSource source);
mesh loadObj(char* filepath);
void free_scene(scene* s);

// Mesh
void addMesh(scene* s, mesh m);
void rotate_mesh_around_z(mesh* m, double angle);
void free_mesh(mesh m);

// Light
char diffuseLight(lightSource source, vec3 normal, vec3 v);

#endif