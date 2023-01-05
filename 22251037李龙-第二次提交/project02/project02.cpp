#include <GL/glut.h>
#include <math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static float year = 0, month = 0, day = 0;
double aix_x = 0.0, aix_y = 1, aix_z = 0;
GLuint texName,texName2,texName3;//纹理名称
GLUquadricObj* qobj,*eobj,*mobj;//二次曲面声明类型

void IPosition(void);// 光源位置
void init(void);// 初始化，启动光源、材质、消隐
void sun(void);//绘制太阳
void earth(void);//绘制地球、月球
void display(void);
void Rotate(void);   //更新旋转参数
void myidle(void);   //闲置调用函数
void reshape(int w, int h);
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("project02");
	init();
	glutDisplayFunc(display);   //调用显示回调函数
	glutReshapeFunc(reshape);   // 设置观察体范围
	glutIdleFunc(myidle);       // 闲置 回调函数
	glutMainLoop();
	return 0;
}
//设置光源位置

//设置光源位置
void IPosition()
{
	float light_position[] = { 0.0,0.0,0.75,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	float light_position1[] = { 0.75,0.0,0.0,1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	float light_position2[] = { -0.75,0.0,1.0,1.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
}


//启动光源、消隐、材料
void init(void)
{
	int w1, h1, n1,w2,h2,n2,w3,h3,n3;
	IPosition(); //光源位置
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);//使用颜色材质
	glClearDepth(1.0f);


	unsigned char* TextureImage,* TextureImage2,*TextureImage3;//加载位图
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //多边形的显示方式,模式将适用于物体的所有面采用填充形式
	glGenTextures(1, &texName);//glGenTextures (GLsizei n, GLuint *textures);在数组textures中返回n个当期未使用的值,表示纹理对象的名称 
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//纹理过滤函数，必须写两遍
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureImage = stbi_load("./太阳.bmp",&w1,&h1,&n1,0);
		//根据指定的参数，生成一个2D纹理（Texture）。
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w1, h1, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &texName2);
	glBindTexture(GL_TEXTURE_2D, texName2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//纹理过滤函数，必须写两遍
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureImage2 = stbi_load("./地球.bmp", &w2, &h2, &n2, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &texName3);
	glBindTexture(GL_TEXTURE_2D, texName3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//纹理过滤函数，必须写两遍
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureImage3 = stbi_load("./月球.jpg", &w3, &h3, &n3, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w3, h3, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage3);
	glBindTexture(GL_TEXTURE_2D, 0);

}
void myidle(void)
{
	glutPostRedisplay();
}
void display(void)
{
	IPosition();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Rotate();
	sun();
	earth();
	glutSwapBuffers();
	glFlush();
}
//绘制太阳

void material_sun(void)
{
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //材质镜面反射颜色参数
	GLfloat mat_shininess[] = { 50.0 };                   // 镜面反射指数参数
	GLfloat mat_diffuse[] = { 1.0,0.0,0.0,1.0 };           //材质散射颜色	 
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 1.0,0.0,0.0,1.0 };       //太阳颜色为红色     
	GLfloat mat_emmision[] = { 1.0,1.0,1.0,1.0 };   //自发光为白光
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //材质镜面反射颜色
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //镜面反射指数
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //材质的散射颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //材质的环境颜色 
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision);     //材质的环境颜色 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  整个场景的环境光的RGBA强度
}
//地球材质
void material_earth(void)
{
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //材质镜面反射颜色参数
	GLfloat mat_shininess[] = { 50.0 };                   // 镜面反射指数参数
	GLfloat mat_diffuse[] = { 1.0,1.0,1.0,1.0 };           //材质散射颜色	 
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1,0.1,0.1,1.0 };       //地球颜色为蓝色   
	GLfloat mat_emmision[] = { 0.2, 0.2,0.2, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //材质镜面反射颜色
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //镜面反射指数
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //材质的散射颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //材质的环境颜色 
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision);     //材质的环境颜色 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  整个场景的环境光的RGBA强度
}
//月球材质
void material_moon(void)
{
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //材质镜面反射颜色参数
	GLfloat mat_shininess[] = { 50.0 };                   // 镜面反射指数参数
	GLfloat mat_diffuse[] = { 0.0,0.0,1.0,1.0 };           //材质散射颜色	 
	GLfloat white_light[] = { 1.0, 0.0,0.0, 1.0 };

	GLfloat lmodel_ambient[] = { 0.5,0.5,0.5,1.0 };       //月亮颜色为灰色     


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //材质镜面反射颜色
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //镜面反射指数
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //材质的散射颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //材质的环境颜色 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  整个场景的环境光的RGBA强度
}

void sun()
{
	qobj = gluNewQuadric();//申请空间
	glPushMatrix();
	material_sun();
	glBindTexture(GL_TEXTURE_2D, texName);//允许建立一个绑定到目标纹理的有名称的纹理。
	glEnable(GL_TEXTURE_2D);//启用二维纹理
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);   //太阳自转	
	glInitNames();
	glPushName(0);
	glLoadName(1);
	gluSphere(qobj, 0.75, 60, 60); //二次曲面qobj
	glDisable(GL_TEXTURE_2D);//禁用二维纹理
	glPopMatrix();
}

//绘制地球和月球
void earth()
{
	eobj = gluNewQuadric();
	glPushMatrix();
	material_earth();
	glBindTexture(GL_TEXTURE_2D, texName2);//允许建立一个绑定到目标纹理的有名称的纹理。
	glEnable(GL_TEXTURE_2D);//启用二维纹理
	gluQuadricTexture(eobj, GL_TRUE);//纹理函数
	glRotatef((GLfloat)day, aix_x, aix_y, aix_z);//公转
	glTranslatef(1.5, 0.0, 0.0);//平移
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);//自转
	gluSphere(eobj, 0.15, 20, 16);
	glDisable(GL_TEXTURE_2D);

	//绘制月球
	mobj = gluNewQuadric();
	material_moon();
	glBindTexture(GL_TEXTURE_2D, texName3);//允许建立一个绑定到目标纹理的有名称的纹理。
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(mobj, GL_TRUE);//纹理函数
	glRotatef((GLfloat)day, 0, 1, 0);//公转
	glTranslatef(0.3, 0, 0);
	gluSphere(mobj, 0.08, 20, 16);
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);//自转
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void Rotate(void)          //设置各行星的公转周期
{
	year = year + 0.02;
	if (year > 360.0)
	{
		year = year - 360.0;
	}
	day = day + 0.02;
	if (day > 360.0)
	{
		day = day - 360.0;
	}
	month = month + 0.02;
	if (month > 360)
		month -= 360;
	glutPostRedisplay();

}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

