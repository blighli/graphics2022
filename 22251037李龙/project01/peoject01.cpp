
#include <GL/glut.h>
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, 0.1, 9999);
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);  //控制视角
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init()
{
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0_diffuse[] = { 1.0, 0.0, 0, 1.0 };//红色点光源
	GLfloat light0_specular[] = { 1.0, 0, 0, 1.0 };
	GLfloat light0_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glShadeModel(GL_SMOOTH);//光滑着色，过渡色效果
	glEnable(GL_LIGHTING);//启动光照，总开关
	glEnable(GL_LIGHT0);//打开0号光源
	glEnable(GL_DEPTH_TEST);

}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSolidTeapot(0.5);
	glFlush();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(500, 500);
	glutCreateWindow("project01");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
