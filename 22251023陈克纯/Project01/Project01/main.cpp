#include<GL\glut.h>
#include<iostream>
using std::cout;
using std::endl;

#define  GLUT_WHEEL_UP 3           //¶¨Òå¹öÂÖ²Ù×÷
#define  GLUT_WHEEL_DOWN 4

void DrawCube(int * a, int * b, int * level)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0f);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0F0F);
	for (int i = 0; i <= level[1]; i++)
		for (int j = 0; j <= level[2]; j++)
		{
			int temp1 = a[1] + i * 1.0 / level[1] * (b[1] - a[1]);
			int temp2 = a[2] + j * 1.0 / level[2] * (b[2] - a[2]);
			glBegin(GL_LINES);
			glVertex3f(a[0], temp1, temp2);
			glVertex3f(b[0], temp1, temp2);
			glEnd();
		}
	glLineStipple(1, 0x0101);
	for (int i = 0; i <= level[0]; i++)
		for (int j = 0; j <= level[2]; j++)
		{
			int temp1 = a[0] + i * 1.0 / level[0] * (b[0] - a[0]);
			int temp2 = a[2] + j * 1.0 / level[2] * (b[2] - a[2]);
			glBegin(GL_LINES);
			glVertex3f(temp1, a[1], temp2);
			glVertex3f(temp1, b[1], temp2);
			glEnd();
		}
	glLineStipple(1, 0x0F0F);
	for (int i = 0; i <= level[1]; i++)
		for (int j = 0; j <= level[0]; j++)
		{
			int temp1 = a[1] + i * 1.0 / level[1] * (b[1] - a[1]);
			int temp2 = a[0] + j * 1.0 / level[0] * (b[0] - a[0]);
			glBegin(GL_LINES);
			glVertex3f(temp2, temp1, a[2]);
			glVertex3f(temp2, temp1, b[2]);
			glEnd();
		}

}

double anger_x = 0;
double anger_y = 0;
double anger_move_x = 0;
double anger_move_y = 0;

double shift_x = 0;
double shift_y = 0;
double shift_move_x = 0;
double shift_move_y = 0;
double scale = 1;

void draw(void)
{
	int a[3] = { -100, -100, 100 };
	int b[3] = { 100, 100, -100 };
	int level[3] = { 3, 3, 3 };

	glPushMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(10.0f, 0.0f, 1.0f, 0.0f);

	glRotatef((anger_x + anger_move_x) * 90, 1.0f, 0.0f, 0.0f);
	glRotatef((anger_y + anger_move_y) * 90, 0.0f, 1.0f, 0.0f);

	glTranslatef((shift_x + shift_move_x) * 0.5, 0.0f, 0.0f);
	glTranslatef(0.0f, (shift_y + shift_move_y) * 0.5, 0.0f);

	glScalef(scale, scale, scale);

	DrawCube(a, b, level);


	glPopMatrix();
	glutSwapBuffers();

	//	glFlush();
}

bool left_tag = false;
bool right_tag = false;
int mouse_x = 0;
int mouse_y = 0;

void processMouse(int button, int state, int x, int y) {

	if (button == GLUT_WHEEL_UP)
		scale -= 0.05;
	if (button == GLUT_WHEEL_DOWN)
		scale += 0.05;
	if (state == GLUT_DOWN)
	{
		mouse_x = x; mouse_y = y;

		if (button == GLUT_LEFT_BUTTON)
			left_tag = true;
		if (button == GLUT_RIGHT_BUTTON)
			right_tag = true;
		//	cout << "left down!" << endl;
	}

	if (state == GLUT_UP)
	{
		left_tag = false;
		right_tag = false;
		if (button == GLUT_LEFT_BUTTON)
		{
			anger_x += anger_move_x;
			anger_y += anger_move_y;
			anger_move_x = 0;
			anger_move_y = 0;
		}

		if (button == GLUT_RIGHT_BUTTON)
		{
			shift_x += shift_move_x;
			shift_y += shift_move_y;
			shift_move_x = 0;
			shift_move_y = 0;
		}
		//	cout << "left up!" << endl;
	}

}

void processMouseActiveMotion(int x, int y) {

	if (left_tag)
	{
		anger_move_x = (y - mouse_y) * 1.0 / 800;
		anger_move_y = (x - mouse_x) * 1.0 / 800;
		//	cout << anger_x << endl;
	}
	if (right_tag)
	{
		shift_move_x = x - mouse_x;
		shift_move_y = mouse_y - y;
	}
}


int   main(int   argc, char   **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowPosition(300, 30);
	glutInitWindowSize(700, 700);
	glutCreateWindow("&Ntilde; ");

	glViewport(0, 400, 400, 400);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-200, 200, -200, 200, 10, 2500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(0, 0, 2000, 0, 0, -20, 0, 1.0, 0);

	glutDisplayFunc(draw);
	glutIdleFunc(draw);

	glEnable(GL_DEPTH_TEST);

	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);

	glutMainLoop();
	return   0;
}