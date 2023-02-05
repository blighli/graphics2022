#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<Windows.h>
#include"read_obj.h"
//globals


//other functions and main
//.obj loader code


void loadObj(char *fname)
{
   FILE *fp;
   int read;
   //GLfloat x, y, z;
   char ch;
  // object=glGenLists(1);
   fp=fopen(fname,"r");
   if (!fp)
   {
    printf("can't open file %s\n", fname);
    //exit(1);
   }
    glPushMatrix();
    while(!(feof(fp)))
    {
     read=fscanf(fp,"%c",&ch);
     switch(ch)
	 {
	  case 'v':
		  fscanf(fp,"%f %f %f",&vertex[ivertex].x,&vertex[ivertex].y,&vertex[ivertex].z);
		  ivertex++;
	      break;
	  case 'f':
	      fscanf(fp,"%d %d %d",&polygon[ipolygon].f1,&polygon[ipolygon].f2,&polygon[ipolygon].f3);
	      ipolygon++;
	      break;  
	 }	
	}
	 
   glPopMatrix();
   fclose(fp);
}
//.obj loader code ends here
void findobjcenter()
{
	for(int i=0;i<ivertex;i++)
	{
		if(xmin>vertex[i].x)
		{
			xmin = vertex[i].x;
		}
		if(xmax<vertex[i].x)
		{
			xmax = vertex[i].x;
		}
		if(ymin>vertex[i].y)
		{
			ymin = vertex[i].y;
		}
		if(ymax<vertex[i].y)
		{
			ymax = vertex[i].y;
		}
		if(zmin>vertex[i].z)
		{
			zmin = vertex[i].z;
		}
		if(ymax<vertex[i].y)
		{
			zmax = vertex[i].z;
		}
		xcenter=(xmin+xmax)/2;
		ycenter=(ymin+ymax)/2;
	}
}
void reshape(int w,int h)
{ 
   glViewport(0,0,w,h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   aspect = (GLfloat)w / (GLfloat)h;
   gluPerspective (field_of_view, aspect, zNear, zFar);
   //glOrtho(-25,25,-2,2,0.1,100);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void draw_object()
{
   glPushMatrix();
   findobjcenter();
   glTranslatef(-xcenter,-ycenter,0.0);
   glTranslatef(0,0,-105);
   glTranslatef(position[0],position[1],position[2]);
   glColor3f(1.0,0.23,0.27);
   glScalef(x_scale,y_scale,z_scale);
   glRotatef(xangle,1,0,0);
   glRotatef(yangle,0,1,0);
   glRotatef(zangle,0,0,1);
   for (int i=0;i<ipolygon;i++)
	 {
		 glBegin(mode);
		 glVertex3d(vertex[polygon[i].f1-1].x,vertex[polygon[i].f1-1].y,vertex[polygon[i].f1-1].z);
		 glVertex3d(vertex[polygon[i].f2-1].x,vertex[polygon[i].f2-1].y,vertex[polygon[i].f2-1].z);
		 glVertex3d(vertex[polygon[i].f3-1].x,vertex[polygon[i].f3-1].y,vertex[polygon[i].f3-1].z);
		 glEnd();
	 }
   glPopMatrix();
}


void display(void)
{
glClearColor (0.0,0.0,0.0,1.0);
glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity();
glRotated(camera_angle_v, 1.0, 0.0, 0.0);
glRotated(camera_angle_h, 0.0, 1.0, 0.0);

glShadeModel (GL_SMOOTH);
GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0  };
GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);

draw_object();
glutSwapBuffers(); //swap the buffers
}
//rotate cara use mouse
void mouseCB(int button, int state, int x, int y)
{
   if(button == GLUT_LEFT_BUTTON)
   {
        if(state == GLUT_DOWN)
		{
            isDragging = true;
            drag_x_origin = x;
            drag_y_origin = y;
        }
        else
            isDragging = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if(isDragging) 
	{
        camera_angle_v -= (y - drag_y_origin)*0.3;
        camera_angle_h -= (x - drag_x_origin)*0.3;
        drag_x_origin = x;
        drag_y_origin = y;
    }
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
      case 27:
         exit(0);break;
	  case '1':mode = GL_TRIANGLES; break; //represent object in surface
	  case '2':mode = GL_LINE_STRIP;break; //represent object with wireframe
	  case '3':mode = GL_POINTS;break; //represent object with points
	  case 'w':position[1]=position[1]+1.0;break;//UP
	  case 's':position[1]=position[1]-1.0;break;//DOWN
	  case 'a':position[0]=position[0]-1.0;break;//LEFT
	  case 'd':position[0]=position[0]+1.0;break;//RIGHT
	  case 'f':position[2]=position[2]+1.0;break;//NEAR
	  case 'b':position[2]=position[2]-1.0;break;//FAR
	  case 'x':xangle=xangle+2;break; //Rotate around x axis
	  case 'y':yangle=yangle+2;break; //Rotate around y axis
	  case 'z':zangle=zangle+2;break; //Rotate around z axis
	  case 'q':
		  if (x_scale<=5.0f)
			{
				x_scale+= 0.5f;
				y_scale+= 0.5f;
				z_scale+= 0.5f;
			}
      break;
	  case 'e':
		  if (x_scale>0)
			{
				x_scale-= 0.5f;
				y_scale-= 0.5f;
				z_scale-= 0.5f;
			}
	 break;
	  case 'j':field_of_view+=10;break;
	  case 'k':aspect+=2;break;
	  case 'i':zNear+=2;break;
	  case 'l':zFar+=10;break;

   }
   glutPostRedisplay();
}

int main(int argc,char **argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
glutInitWindowSize(screen_width,screen_height);
glutInitWindowPosition(20,20);
glutCreateWindow("ObjLoader");
glutReshapeFunc(reshape);
glutDisplayFunc(display);
glutIdleFunc(display);
loadObj("c:/users/lindsey/desktop/pig.obj");
glutMouseFunc(mouseCB);
glutMotionFunc(mouseMotionCB);
glutKeyboardFunc(keyboard);
glutMainLoop();
return 0;
}

