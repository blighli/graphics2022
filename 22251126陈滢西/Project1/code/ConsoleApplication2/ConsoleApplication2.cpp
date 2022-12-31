
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include"resource1.h"
#include<windows.h>
#include <thread>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")

const GLfloat factor = 0.1f;
const GLfloat Pi = 3.1415926536f;
GLfloat num = 0.0f;
const int n = 400;


static float angle = 0.0;//angle绕y轴的旋转角
static float x = 0.0f, y = 0.5f, z = -1.5f;//相机位置
static float lx = 0.0f, ly = 0.0f, lz = -0.0f;//视线方向，初始设为沿着Z轴负方向

//定义观察方式
void changeSize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);    //投影变换
	glLoadIdentity();
	//设置视口为整个窗口大小
	glViewport(0, 0, w, h);
	//设置可视空间
	gluPerspective(45, 1.0f * w / h, 1, 1000); //角度45， 窗口纵横比， 眼睛所及距离（近和远）

	glMatrixMode(GL_MODELVIEW);     //模型变换
	glLoadIdentity();
	//相机（人眼）位置， 眼睛看的点（相机位置+视线方向）， 观察者本身方向（角度，比如正立）
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

//旋转相机，绕y轴旋转
void orientMe(float ang)
{
	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	//把所看的点（即视线方向上的点）理解为在一个圆上旋转，那设置的点的坐标应该是旋转的，通过圆半径计算坐标
	//！！！画图好理解！！！
}
//前后移动相机
void move_Front_Back(int direction)
{
	x = x + direction * (lx) * 0.05;
	z = z + direction * (lz) * 0.05;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	//同时移动相机和所看的点坐标，只用修改x和z，视线参数不用修改，
}

//左右移动相机，一定要画图理解
void move_Left_Right(int direction)
{
	x = x + direction * (lz) * 0.05;
	z = z - direction * (lx) * 0.05;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void move_High_Low(int direction)
{
	y = y + direction * 0.05;
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

//键盘响应
void inputKey(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q':
		angle -= 0.01f;
		orientMe(angle);
		break;
	case 'e':               //q,e键调用相机旋转
		angle += 0.01f;
		orientMe(angle);
		break;
	case 'w':
		move_Front_Back(1);
		break;
	case 's':
		move_Front_Back(-1);
		break;
	case 'a':
		move_Left_Right(1);
		break;
	case 'd':
		move_Left_Right(-1);
		break;              //wasd调整相机前后左右移动
	case '1':
		move_High_Low(1);
		break;
	case '2':
		move_High_Low(-1);
		break;              //1,2键上下移动相机
	default:
		break;
	}
}

//设置光源位置
void SetLight()
{


	float light_position[] = { 3,3,0,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
void material(void)
{
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //材质镜面反射颜色参数
	GLfloat mat_shininess[] = { 50.0 };                   // 镜面反射指数参数
	GLfloat mat_diffuse[] = { 0.0,0.0,1.0,1.0 };           //材质散射颜色	 
	GLfloat white_light[] = { 1.0, 0.0,0.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.2f, 0.5f, 0.6f };       //颜色为蓝色     
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //材质镜面反射颜色
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //镜面反射指数
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //材质的散射颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //材质的环境颜色 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  整个场景的环境光的RGBA强度
}
void init(void)
{
	SetLight(); //光源颜色
	glShadeModel(GL_SMOOTH);//平滑阴影
	glEnable(GL_LIGHTING);//启用光源
	glEnable(GL_LIGHT0);////开启GL_LIGHT0光源
	//glEnable(GL_DEPTH_TEST);//！！！！！！！！！！！！！不然茶壶会浸没到水里
	glClearDepth(1.0f);//深度缓存

	//glMatrixMode(GL_PROJECTION);/**/
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glMatrixMode(GL_MODELVIEW);

	/*	unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

		//帧缓存
		unsigned int hdrFBO;
		glGenFramebuffers(1, &hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "帧缓存未初始化完毕！" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}
static float _fin = 0.0;
void myDisplay(void) {
	material();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	gluLookAt(0.0, 0.5, 0.4, 0, 0, 0, 0, 1.0, 0.0);
	//for (float _num = 0.0; _num < 1; _num += 0.1)
	{
		//glPushMatrix();
		glLineWidth(3);
		GLfloat _x;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		//float lx = sin(_num*0.1);
		//float lz = -cos(_num * 0.1);
		//gluLookAt(0.0, 0.5, -1.0, 0, 0, 0, 0, 1.0, 0.0);
		gluLookAt(x, y, z, lx, ly, lz, 0.0f, 1.0f, 0.0f);
		//gluLookAt(0.0, 0.5, -1.5, 0, 0, 0, 0, 1.0, 0.0);
		//gluLookAt(0.0, 0.5 , 0.4, 0 + lx, 0, lz, 0, 1.0, 0.0);
		//设置线条的颜色
		glColor3f(0.2f, 0.5f, 0.6f);
		for (int i = 0; i <= 200; i++)
		{

			glRotatef(2, 0, 1, 0);
			glBegin(GL_LINES);
			for (_x = -1.0f / factor; _x < 1.0f / factor; _x += 0.01f) {
				glVertex3f(_x * factor, sin(_x + _fin) * factor, 0);

			}
			glEnd();
			glFlush();
			//双缓冲技术，会隐性的使用一次glFlush();


		}glLoadIdentity();
		//gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
		//gluLookAt(0.0, 0.5, -1.5, 0, 0, 0, 0, 1.0, 0.0);
		gluLookAt(x, y, z, lx, ly, lz, 0.0f, 1.0f, 0.0f);
		//gluLookAt(0.0, 0.0 , 0.4, 0 + _num * 0.05, 0,0, 0, 1.0, 0.0);
		glRotatef(-20, 0, 1.0, 1.0);

		glColor3ub(119, 53, 32);
		glTranslated(0.01, 0.0, 1.5);
		glutSolidTeapot(0.2);
		glutSwapBuffers();
		glLineWidth(3);
		glLoadIdentity();
		gluLookAt(x, y, z, lx, ly, lz, 0.0f, 1.0f, 0.0f);
		//gluLookAt(0.0, 0.5, -1.5, 0, 0, 0, 0, 1.0, 0.0);
		for (int i = 0; i < 20; i++)
		{
			if (i % 6 == 0)
			{
				glColor3f(1 - 0.03 * i, 1 - 0.02 * i, 1 - 0.01 * i);
			}
			else { glColor3f(1.0, 1.0, 1.0); }
			//glTranslated(0.0001, 0, 0);
			glBegin(GL_LINES);
			glVertex3d(0.0, 0, 1.9);
			glVertex3d(0.0,0, 0);
			glEnd();
			glutSwapBuffers();

		}
		_fin = _fin+0.01;
		if (_fin > 600)
		{
			_fin = 0.0;
		}
	}
	

}



int main(int argc, char* argv[]) {
	PlaySound(LPWSTR(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("茶");
	init();
	glutKeyboardFunc(inputKey);     //键盘响应事件
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myDisplay);      //闲置时回调函数
	glutReshapeFunc(&changeSize);    //调整窗口大小回调函数
	glutMainLoop();
	return 0;
}