/*
 *  Name : Elowan
 *  Creation : 01-01-2024 13:49:50
 *  Last modified : 29-04-2024 21:03:27
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "engine.h"
#include "../utils/data_structures.h"

DEFINE_RESIZABLE(triangle3D, t3D)
DEFINE_RESIZABLE(char*, str)


extern const vec2 UNDEFINED_VEC2;
extern const vec3 UNDEFINED_VEC3;

const char DEFAULT_CHAR = ' ';
const float ANGLE_ROTATION = 10;
const double PROXIMITY = 0.1;
const int nbChar = 7;
const char* lightGradient = ".,;la#@";
float ASPECT_RATIO_CHARACTER_SHELL = 2.2;

// Screens
screen* init_screen(int width, int height){
    screen* s = calloc(1, sizeof(screen));
    s->height = height;
    s->width = width;
    s->pixels = calloc(width*height, sizeof(char));
    for(int i=0; i<width*height; i++){
        s->pixels[i] = DEFAULT_CHAR;
    }
    
    s->aspect_ratio_character = ASPECT_RATIO_CHARACTER_SHELL;
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
    v.x = s->aspect_ratio_character * ((float) s->height/ (float) s->width) * v.x + 1.0;
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
    Initialize a scene
*/
scene* init_scene(){
    scene* s = calloc(1, sizeof(scene));

    s->objects = init_resizable_array_t3D();
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
    int* nb_in, int* nb_out, bool* is_inverted){
    // May refactor this function in multiple functions

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
    Takes a resizable 3DTriangle array `objects` and a camera to 
    return an resizable array of 3Dtriangle that should be 
    drawn on the screen (Cutting, in some case, triangles by half)
*/
resizable_array_t3D clip(resizable_array_t3D objects, camera* cam){
    // Point in the limit plane
    vec3 planePoint = add_vec3(cam->pos, mul_vec3(getCameraLookingAt(cam), PROXIMITY));
    
    // Triangles that will be shown
    resizable_array_t3D results = init_resizable_array_t3D();
    
    for(int i = 0; i<objects.size; i++){
        triangle3D t = get_resizbl_arr_t3D(objects, i);

        if (!is_visible(t, cam)) continue;
        
        vec3* in = calloc(3, sizeof(vec3));
        vec3* out = calloc(3, sizeof(vec3));

        int nb_in = 0;
        int nb_out = 0;
        bool is_inverted = false;

        in_out_points(t, planePoint, getCameraLookingAt(cam),
            in, out, &nb_in, &nb_out, &is_inverted);

        // If the triangle is totally visible
        if (nb_out == 0) append_resizbl_arr_t3D(&results, t);

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

            if (is_visible(t1, cam)) 
                append_resizbl_arr_t3D(&results, t1);

            if (is_visible(t2, cam)) 
                append_resizbl_arr_t3D(&results, t2);
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

            if (is_visible(t1, cam))
                append_resizbl_arr_t3D(&results, t1);
        }
    }
    
    return results;
}   

double* triangles3DToDistsArray(triangle3D* triangles, int n, camera* cam){
    double* dists = calloc(n, sizeof(double));
    for(int i = 0; i < n; i++){
        dists[i] = distTriangle3DCamera(triangles[i], cam->pos);
    }
    return dists;
}

void render(screen* scr, scene* s, lightSource source){
    clear_screen(scr);

    // Computes triangles to be shown
    resizable_array_t3D clipped_triangles = clip(s->objects, s->cam);

    // Sorts descending the triangles by their distance to the camera 
    // to prevent further objects going over closer objects 
    double* dists = triangles3DToDistsArray(clipped_triangles.array, 
        clipped_triangles.size, s->cam);
    sortArrayDesc((void **)clipped_triangles.array, dists, clipped_triangles.size);

    // Draw triangles on the screen by their relative position to the camera
    for(int i = 0; i < clipped_triangles.size; i++){
        triangle3D t = get_resizbl_arr_t3D(clipped_triangles, i);
        triangle3D t_cam = changeTriangleReferenceToCamera(s->cam, t);
        
        char lightStrength = diffuseLight(source, normal_surface(t), t.v1);
        
        t_cam.v1 = sub_vec3(t_cam.v1, s->cam->pos);
        t_cam.v2 = sub_vec3(t_cam.v2, s->cam->pos);
        t_cam.v3 = sub_vec3(t_cam.v3, s->cam->pos);

        triangle2D t2 = project_triangle3D_to_2D(t_cam);
        draw_triangle2D(scr, triangle2D_to_screen(scr, t2), lightStrength);
    }
    update_screen(scr);

    free_resizbl_arr_t3D(clipped_triangles);
}

/*
    Transform a .obj file into a resizable Triangle3D array
    See https://en.wikipedia.org/wiki/Wavefront_.obj_file for better 
    understanding of the parsing
*/
resizable_array_t3D loadObj(char* filepath){
    /*
    Loads the file and save into `vertices_char` and `faces_char` each line 
    containing import stuff to create the mesh
    */ 
    char* contents = NULL;

    FILE* file = fopen(filepath, "r");
    size_t len = 0;

    // Array of Arrays of words separated by a space
    resizable_array_str vertices_char = init_resizable_array_str();
    resizable_array_str faces_char = init_resizable_array_str();
    
    // For each lines, if vertex or face, it will copy the entire line into
    // the corresponding array
    while (getline(&contents, &len, file) != -1) {                
        // If vertex (and not )
        if (contents[0] == 'v' && contents[1] == ' '){
            append_resizbl_arr_str(&vertices_char, calloc(len, sizeof(char *)));
            
            strcpy(
                get_resizbl_arr_str(vertices_char, vertices_char.size-1), 
                contents
            );

        }
        // If face
        else if (contents[0] == 'f'){
            append_resizbl_arr_str(&faces_char, calloc(len, sizeof(char *)));
            
            strcpy(
                get_resizbl_arr_str(faces_char, faces_char.size-1),
                contents
            );
        }
    }

    fclose(file);
    
    
    /*
    Converts the vertices lines into real vectors 
    */
    vec3* vertices = calloc(vertices_char.size, sizeof(vec3));    
    for(int i=0; i<vertices_char.size; i++){
        char* line = get_resizbl_arr_str(vertices_char, i);

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

    /*
        Converts the vertices & faces into triangles
    */
    resizable_array_t3D triangles = init_resizable_array_t3D();

    for(int i=0; i<faces_char.size; i++){
        char* line = get_resizbl_arr_str(faces_char, i);
        
        // Builds a realtime string of the numbers
        char** numbers_char = calloc(4, sizeof(char*));
        for(int k=0; k<4; k++) numbers_char[k] = calloc(256, sizeof(char));
        int n_numbers = 0;
        int j = 2;

        // Reads the line
        while(line[j] != '\0' && n_numbers < 4){
            // Takes only the x, y and z of each 
            // f x/xt/xn y/yt/yn z/zt/zn line
            if (line[j] == '/'){
                while(line[j] != ' ' && line[j] != '\0') j++;
            }
            printf("j %d\n", j);
            
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
            triangle3D t = {
                vertices[numbers[0]-1],
                vertices[numbers[1]-1],
                vertices[numbers[2]-1]
            };
            append_resizbl_arr_t3D(&triangles, t);
            
        } else {
            triangle3D t1 = {
                vertices[numbers[0]-1],
                vertices[numbers[1]-1],
                vertices[numbers[2]-1]
            };
            append_resizbl_arr_t3D(&triangles, t1);

            triangle3D t2 = {
                vertices[numbers[2]-1],
                vertices[numbers[3]-1],
                vertices[numbers[0]-1]
            };
            append_resizbl_arr_t3D(&triangles, t2);
        }
    }
    
    free_resizbl_arr_str(vertices_char);
    free_resizbl_arr_str(faces_char);
    
    return triangles;
}

/*
Adds a mesh (an resizable array of 3D Triangles) to the scene

`s` : Scene
`mesh` : Resizable Triangle3D array of the triangles of the mesh
*/
void addMesh(scene* s, resizable_array_t3D mesh){
    for(int i = 0; i < mesh.size; i++){
        append_resizbl_arr_t3D(&(s->objects), get_resizbl_arr_t3D(mesh, i));
    }
}

void free_scene(scene* s){
    free_resizbl_arr_t3D(s->objects);
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
    if(intensity < 0) return lightGradient[(int) round(-intensity*(nbChar-1))];
    else return lightGradient[0];
}