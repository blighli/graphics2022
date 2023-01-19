#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>

#define MAX_VERTICES 8000 // Max number of vertices (for each object)
#define MAX_POLYGONS 8000 // Max number of polygons (for each object)

GLenum mode;

typedef struct{
float x,y,z;
}vertex_type;

typedef struct{
int f1,f2,f3;
}polygon_type;

vertex_type vertex[MAX_VERTICES]; 
polygon_type polygon[MAX_POLYGONS];

GLuint object;
float xangle,yangle,zangle;

// Mouse drag control
bool isDragging; // true when dragging
double camera_angle_h = 0;
double camera_angle_v = 0;

int drag_x_origin;
int drag_y_origin;

GLfloat x_scale=1.0,y_scale=1.0,z_scale=1.0;
int ivertex=0;
int ipolygon=0;
float lastx, lasty;
GLfloat position[] = {0.0, 0.0, 0.0, 0.0};
float xmin=0.0; 
float xmax=0.0; 
float ymin=0.0;
float ymax=0.0;
float zmin=0.0;
float zmax=0.0;
float xcenter,ycenter;
int screen_width=600,screen_height=600;

int field_of_view=60;
double aspect;
double zNear=0.1;
double zFar=1000.0;
