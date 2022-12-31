#include<GL/glut.h>
#include<math.h>
int faces[6][4] = { {0,3,2,1},{3,7,6,2},{7,4,5,6},{4,0,1,5},{1,2,6,5},{4,7,3,0} };//六个面，每个面有四个顶点
GLfloat points[8][3] = { {-0.5,-0.5,0.5},{-0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,-0.5,0.5},{-0.5,-0.5,-0.5},{-0.5,0.5,-0.5},{0.5,0.5,-0.5},{0.5,-0.5,-0.5} };//每个顶点的位置
GLfloat faceColor[6][3] = { {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0},{0.0,1.0,1.0},{1.0,0.0,1.0} };
GLfloat pointColor[8][3] = { {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0},{0.0,1.0,1.0},{1.0,0.0,1.0},{0.5,0.5,1.0},{1.0,0.3,0.5} };
GLfloat r = 1, theta = 30, phi = 30;
void drawFace(int* face) {
	glBegin(GL_POLYGON);
	glColor3fv(pointColor[*face]);
	glVertex3fv(points[*face]);
	glColor3fv(pointColor[*(face + 1)]);
	glVertex3fv(points[*(face + 1)]);
	glColor3fv(pointColor[*(face + 2)]);
	glVertex3fv(points[*(face + 2)]);
	glColor3fv(pointColor[*(face + 3)]);
	glVertex3fv(points[*(face + 3)]);
	glEnd();

}

void Cube() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 5);
	gluLookAt(r * sin(theta * 3.14159 / 180) * sin(phi * 3.14159 / 180), r * cos(theta * 3.14159 / 180), r * sin(theta * 3.14159 / 180) * cos(phi * 3.14159 / 180), 0, 0, 0, 0, 1, 0);
	int k;
	for (k = 0; k < 6; k++) {
		glColor3fv(faceColor[k]);
		drawFace(faces[k]);
	}
	glFlush();
}
void myKeyboard(unsigned char key, int x, int y) {//键盘控制函数，控制视角变化

	switch (key)
	{
	case'A':
	case'a':
		if (phi - 1 > -180)
			phi = phi - 1;
		break;
	case'D':
	case'd':
		if (phi + 1 < 180)
			phi = phi + 1;
		break;
	case'S':
	case's':
		if (theta + 1 < 180)
			theta = theta + 1;
		break;
	case'W':
	case'w':
		if (theta - 1 > 0)
			theta = theta - 1;
		break;
	case'G':
	case'g':
		for (int i = 0; i < 8; i++) {
			points[i][0] = points[i][0] + 0.1;
		}
		break;
	case'L':
	case'l':
		for (int i = 0; i < 8; i++) {
			points[i][0] = points[i][0] - 0.1;
		}
		break;
	}
	glutPostRedisplay();
}
void main(int argc, char** argv) {
	glutInit(&argc, argv); //初始化GLUT库;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("cube"); // 创建窗口，标题为“cube" ;
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 5);

	gluLookAt(r * sin(theta * 3.14159 / 180) * sin(phi * 3.14159 / 180), r * cos(theta * 3.14159 / 180), r * sin(theta * 3.14159 / 180) * cos(phi * 3.14159 / 180), 0, 0, 0, 0, 1, 0);
	glutDisplayFunc(Cube); //用于绘制当前窗口;
	glutKeyboardFunc(myKeyboard); //指定键盘回调函数
	glutMainLoop();

}