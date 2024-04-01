/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:49:50
 *  Last modified : 01-04-2024 16:25:42
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "engine.h"

extern const vec2 UNDEFINED_VEC2;
extern const vec3 UNDEFINED_VEC3;

const char DEFAULT_CHAR = ' ';
const float ASPECT_RATIO_CHARACTER_SHELL = 1.5;

const float ANGLE_ROTATION = 10;
const double PROXIMITY = 0.5;

const int nbChar = 7;
const char* lightGradient = ".,;la#@";


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
    t.v1 = vec2_to_screen(s, t.v1);
    t.v2 = vec2_to_screen(s, t.v2);
    t.v3 = vec2_to_screen(s, t.v3);

    return t;
}

void draw_triangle2D(screen *s, triangle2D t, char c){
    // Definition of the max zone where the pixel can be drawn
    vec2 br = get_bottom_right_corner(t);
    vec2 tl = get_top_left_corner(t);

    vec2 dims = sub_vec2(br, tl);

    for(int i=0; i <= dims.y; i++){
        for(int j=0; j <= dims.x; j++){
            vec2 pixelCoords = {tl.x+j, tl.y+i};
            
            if (is_point_inside_triangle(t, pixelCoords))
                change_pixel(s, pixelCoords, c);
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

/*
Cast coordinates of the vector `v` from the canonic base into the camera base 
(made of the vectors v1, v2 and v3)
*/
vec3 changeVec3ReferenceToCamera(camera* cam, vec3 v){
    vec3 r = v;
    double** p = transition_matrix(cam->v1, cam->v2, cam->v3);

    r = sub_vec3(r, cam->pos);
    r = multiply_matrix_vector_3D(p, r);
    r = add_vec3(r, cam->pos);

    free_matrix(3, p);
    return r;
}

/*
Cast coordinates of the triangle 3D`t` from the canonic base into the camera base 
(made of the vectors v1, v2 and v3)
*/
triangle3D changeTriangleReferenceToCamera(camera* cam, triangle3D t){
    triangle3D r = t;
    
    r.v1 = changeVec3ReferenceToCamera(cam, r.v1);
    r.v2 = changeVec3ReferenceToCamera(cam, r.v2);
    r.v3 = changeVec3ReferenceToCamera(cam, r.v3);

    return r;
}

void print_camera_infos(camera* cam){
    printf("Position camera : ");print_vec3(cam->pos);printf("\n");
    print_vec3(cam->v1);print_vec3(cam->v2);print_vec3(cam->v3);printf("\n");
}

vec3 getCameraLookingAt(camera* cam){
    return cam->v2;
}

void free_camera(camera *cam){
    free(cam);
}


// Scenes
/*
    Initialize a scene of `n` triangles
*/
scene* init_scene(int n){
    scene* s = calloc(1, sizeof(scene));

    s->capacity = n;
    s->size = 0;

    s->objects = calloc(s->capacity, sizeof(triangle3D));
    s->cam = init_camera(0, 0, 0);

    return s;
}


/*
    Computes the points of the triangle that can be seen (in array)
    or that are too close to the camera to be shown (out array). 
    The border being the `p` point
    It puts the size of the array respectively in `nb_in` and `nb_out`
*/
void in_out_points(triangle3D t, vec3 planePoint, vec3 normal, vec3* in, vec3* out, 
    // May refactor this function in multiple functions

    int* nb_in, int* nb_out, bool* is_inverted){
    // Dot product of the points of the triangle and the point p to 
    // know if the point is out of the range of view(too close to 
    // the camera) or not 
    float p_v1 = dot_vec3(sub_vec3(planePoint, t.v1), normal);
    float p_v2 = dot_vec3(sub_vec3(planePoint, t.v2), normal);
    float p_v3 = dot_vec3(sub_vec3(planePoint, t.v3), normal);

    if (p_v1 < 0){
        in[*nb_in] = t.v1;
        (*nb_in)++;
    } else {
        out[*nb_out] = t.v1;
        (*nb_out)++;
    }

    if (p_v2 < 0){
        in[*nb_in] = t.v2;
        (*nb_in)++;
    } else {
        out[*nb_out] = t.v2;
        (*nb_out)++;
    }

    if (p_v3 < 0){
        in[*nb_in] = t.v3;
        (*nb_in)++;
    } else {
        out[*nb_out] = t.v3;
        (*nb_out)++;
    }

    // The triangle orientation is counterclockwise
    *is_inverted = p_v1*p_v3 < 0;
}

/*
    Returns whether or not the triangle is going to be shown or not 
    decided by the orientation of it
*/
bool is_visible(triangle3D t, camera* cam){
    vec3 normal = normal_surface(t);
    return dot_vec3(normal, sub_vec3(t.v1, cam->pos)) > 0;
}

/*
    Takes a 3DTriangle array `objects` of size `n` and a camera to 
    return an array of 3Dtriangle of size `nbTriangles`(side effect) that should be 
    drawn on the screen (Cutting, in some case, triangles by half)
*/
triangle3D* clip(triangle3D* objects, int n, camera* cam, int* nbTriangles){
    // Point in the limit plane
    vec3 planePoint = add_vec3(cam->pos, mul_vec3(getCameraLookingAt(cam), PROXIMITY));
    
    // Todo : Resizable array
    // Triangles that will be shown
    triangle3D* results = calloc(n, sizeof(triangle3D));
    *nbTriangles = 0;
    
    for(int i = 0; i<n; i++){
        triangle3D t = objects[i];
        
        vec3* in = calloc(3, sizeof(vec3));
        vec3* out = calloc(3, sizeof(vec3));

        int nb_in = 0;
        int nb_out = 0;
        bool is_inverted = false;

        in_out_points(t, planePoint, getCameraLookingAt(cam),
            in, out, &nb_in, &nb_out, &is_inverted);

        // If the triangle is totally visible
        if (nb_out == 0) {
            if (is_visible(t, cam)){
                results[*nbTriangles] = t;
                (*nbTriangles)++;
            }
        }

        // If the triangle is totally hidden
        else if (nb_out == 3) continue;

        // Computes 2 points of the limit plane to create 2 new triangles
        // that are in the visible area 
        else if (nb_out == 1){
            vec3 planeV1 = point_intersec_plane_line(getCameraLookingAt(cam), 
                                planePoint, out[0], in[0]);
            vec3 planeV2 = point_intersec_plane_line(getCameraLookingAt(cam), 
                                planePoint, out[0], in[1]);

            triangle3D t1;
            triangle3D t2;

            if (is_inverted){
                t1.v1 = planeV2;
                t1.v2 = in[1];
                t1.v3 =  planeV1;
                
                t2.v1 = planeV1;
                t2.v2 =  in[1];
                t2.v3 = in[0];

            } else {
                t1.v1 = planeV1;
                t1.v2 = in[0];
                t1.v3 =  planeV2;
                
                t2.v1 = planeV2;
                t2.v2 =  in[0];
                t2.v3 = in[1];
            }

            if (is_visible(t1, cam)){
                results[*nbTriangles] = t1;
                (*nbTriangles)++;
            }

            if (is_visible(t2, cam)){
                results[*nbTriangles] = t2;
                (*nbTriangles)++;
            }
        }
        
        // Computes 2 points of the limit plane to create 1 new triangle
        // that is in the visible area 
        else if (nb_out == 2){
            vec3 planeV1 = point_intersec_plane_line(getCameraLookingAt(cam), 
                                planePoint, out[0], in[0]);
            vec3 planeV2 = point_intersec_plane_line(getCameraLookingAt(cam), 
                                planePoint, out[1], in[0]);
                                
            triangle3D t1;

            if (is_inverted){
                t1.v1 = planeV1;
                t1.v2 = in[0];
                t1.v3 = planeV2;
            } else {
                t1.v1 = planeV1;
                t1.v2 = planeV2;
                t1.v3 = in[0];
            }

            if (is_visible(t1, cam)){
                results[*nbTriangles] = t1;
                (*nbTriangles)++;
            }
        }
    }
    
    return results;
}   

void render(screen* scr, scene* s, lightSource source){
    clear_screen(scr);

    int n = 0;
    triangle3D* clipped_triangles = clip(s->objects, s->size, s->cam, &n);
    
    lightSource newSource;
    newSource.pos = changeVec3ReferenceToCamera(s->cam, source.pos); 

    for(int i = 0; i < n; i++){
        triangle3D t = changeTriangleReferenceToCamera(s->cam, clipped_triangles[i]);
        char lightStrength = diffuseLight(newSource, normal_surface(t), t.v1);
        
        t.v1 = sub_vec3(t.v1, s->cam->pos);
        t.v2 = sub_vec3(t.v2, s->cam->pos);
        t.v3 = sub_vec3(t.v3, s->cam->pos);

        triangle2D t2 = project_triangle3D_to_2D(t);
        draw_triangle2D(scr, triangle2D_to_screen(scr, t2), lightStrength);
    }
    update_screen(scr);
}

/*
    Transform a .obj file into a Triangle3D array of size `triangles_number`
    (side effect)
*/
triangle3D* loadObj(char* filepath, int* triangles_number){

    // Loads the file
    char* contents = NULL;

    FILE* file = fopen(filepath, "r");
    size_t len = 0;

    // Array of Arrays of words separated by a space
    char** vertices_char = calloc(4, sizeof(char*));
    int n_vertices = 0; 
    int capacity_vertices = 4;

    char** faces_char = calloc(4, sizeof(char*));
    int n_faces = 0;
    int capacity_faces = 4;
    
    // For each lines
    while (getline(&contents, &len, file) != -1) {
        // Resize arrays if needed
        if (n_vertices>=capacity_vertices){
            vertices_char = (char **) realloc(vertices_char, capacity_vertices*2*sizeof(char *));
            capacity_vertices = capacity_vertices*2;
        }

        if (n_faces>=capacity_faces){
            faces_char = (char **) realloc(faces_char, capacity_faces*2*sizeof(char *));
            capacity_faces = capacity_faces*2;
        }
                
        // If vertex
        if (contents[0] == 'v'){
            vertices_char[n_vertices] = calloc(len, sizeof(char *));
            strcpy(vertices_char[n_vertices], contents);
            n_vertices++;

        }
        // If face
        else if (contents[0] == 'f'){
            faces_char[n_faces] = calloc(len, sizeof(char *));
            strcpy(faces_char[n_faces], contents);
            n_faces++;
        }
    }

    fclose(file);
    
    
    // Converts the vertors lines into real vectors 
    vec3* vertices = calloc(n_vertices, sizeof(vec3));    
    for(int i=0; i<n_vertices; i++){
        char* line = vertices_char[i];

        // Builds a realtime string of the numbers
        char** coords = calloc(3, sizeof(char*));
        for(int k=0; k<3; k++) coords[k] = calloc(256, sizeof(char));
        int n_coords = 0;
        int j = 2;

        // Reads the line
        while(line[j] != '\0' && n_coords < 3){
            if (line[j] != ' '){
                // Converts a char into a string
                char str[2] = "\0"; /* gives {\0, \0} */
                str[0] = line[j];

                strcat(coords[n_coords], str);
                
            } else {
                n_coords++;
            }
            j++;
        }

        // Converts str to double
        sscanf(coords[0], "%f", &vertices[i].x);
        sscanf(coords[1], "%f", &vertices[i].y);
        sscanf(coords[2], "%f", &vertices[i].z);
    }

    // Converts the vectors & faces into triangles
    triangle3D* triangles = calloc(n_faces*2, sizeof(triangle3D));
    int n_triangles = 0;
    for(int i=0; i<n_faces; i++){
        char* line = faces_char[i];
        
        // Builds a realtime string of the numbers
        char** numbers_char = calloc(4, sizeof(char*));
        for(int k=0; k<4; k++) numbers_char[k] = calloc(256, sizeof(char));
        int n_numbers = 0;
        int j = 2;

        // Reads the line
        while(line[j] != '\0' && n_numbers < 4){
            if (line[j] != ' '){
                // Converts a char into a string
                char str[2] = "\0"; /* gives {\0, \0} */
                str[0] = line[j];

                strcat(numbers_char[n_numbers], str);
                
            } else {
                n_numbers++;
            }
            j++;
        }
        
        // Counts the last number of the line
        n_numbers++;
        
        // Converts string into integer
        int* numbers = calloc(4, sizeof(int));
        for(int j=0; j<n_numbers; j++){
            sscanf(numbers_char[j], "%d", &numbers[j]);
        }

        // Creates the triangles according to the .obj standard 
        if (n_numbers == 3){
            triangles[n_triangles].v1 = vertices[numbers[0]-1];
            triangles[n_triangles].v2 = vertices[numbers[1]-1];
            triangles[n_triangles].v3 = vertices[numbers[2]-1];
            n_triangles++;
            
        } else {
            triangles[n_triangles].v1 = vertices[numbers[0]-1];
            triangles[n_triangles].v2 = vertices[numbers[1]-1];
            triangles[n_triangles].v3 = vertices[numbers[2]-1];
            n_triangles++;

            triangles[n_triangles].v1 = vertices[numbers[2]-1];
            triangles[n_triangles].v2 = vertices[numbers[3]-1];
            triangles[n_triangles].v3 = vertices[numbers[0]-1];
            n_triangles++;

        }
    }

    *triangles_number = n_triangles;
    return triangles;
}

/*
Adds a mesh (an array of 3D Triangles) to the scene

`s` : Scene
`mesh` : Array of 3DTriangle
`n` :  Size of the array
*/
void addMesh(scene* s, triangle3D* mesh, int n){
    // Temporary solution before adding resizable array
    if (s->capacity < s->size + n) printf("Warning : No space left");

    for(int i = 0; i < n; i++){
        s->objects[s->size + i] = mesh[i];
    }

    s->size += n;
}

void free_scene(scene* s){
    free(s->objects);
    free_camera(s->cam);
    free(s);
}

/*
Computes the intensity of the light `source` on a triangle, represented by
its normal vector and one of its edges `v`, and returns a char corresponding
to this intensity
*/
char diffuseLight(lightSource source, vec3 normal, vec3 v){
    vec3 lightDir = sub_vec3(source.pos, v);
    
    double intensity = dot_vec3(normalize(lightDir), normalize(normal));
    if(intensity > 0) return lightGradient[(int) round(intensity*(nbChar-1))];
    else return lightGradient[0];
}