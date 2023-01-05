#include <GL/glut.h>
#include <math.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static float year = 0, month = 0, day = 0;
double aix_x = 0.0, aix_y = 1, aix_z = 0;
GLuint texName,texName2,texName3;//��������
GLUquadricObj* qobj,*eobj,*mobj;//����������������

void IPosition(void);// ��Դλ��
void init(void);// ��ʼ����������Դ�����ʡ�����
void sun(void);//����̫��
void earth(void);//���Ƶ�������
void display(void);
void Rotate(void);   //������ת����
void myidle(void);   //���õ��ú���
void reshape(int w, int h);
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("project02");
	init();
	glutDisplayFunc(display);   //������ʾ�ص�����
	glutReshapeFunc(reshape);   // ���ù۲��巶Χ
	glutIdleFunc(myidle);       // ���� �ص�����
	glutMainLoop();
	return 0;
}
//���ù�Դλ��

//���ù�Դλ��
void IPosition()
{
	float light_position[] = { 0.0,0.0,0.75,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	float light_position1[] = { 0.75,0.0,0.0,1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	float light_position2[] = { -0.75,0.0,1.0,1.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
}


//������Դ������������
void init(void)
{
	int w1, h1, n1,w2,h2,n2,w3,h3,n3;
	IPosition(); //��Դλ��
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);//ʹ����ɫ����
	glClearDepth(1.0f);


	unsigned char* TextureImage,* TextureImage2,*TextureImage3;//����λͼ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //����ε���ʾ��ʽ,ģʽ���������������������������ʽ
	glGenTextures(1, &texName);//glGenTextures (GLsizei n, GLuint *textures);������textures�з���n������δʹ�õ�ֵ,��ʾ������������ 
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//������˺���������д����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureImage = stbi_load("./̫��.bmp",&w1,&h1,&n1,0);
		//����ָ���Ĳ���������һ��2D����Texture����
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w1, h1, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &texName2);
	glBindTexture(GL_TEXTURE_2D, texName2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//������˺���������д����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureImage2 = stbi_load("./����.bmp", &w2, &h2, &n2, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &texName3);
	glBindTexture(GL_TEXTURE_2D, texName3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//������˺���������д����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureImage3 = stbi_load("./����.jpg", &w3, &h3, &n3, 0);
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
//����̫��

void material_sun(void)
{
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //���ʾ��淴����ɫ����
	GLfloat mat_shininess[] = { 50.0 };                   // ���淴��ָ������
	GLfloat mat_diffuse[] = { 1.0,0.0,0.0,1.0 };           //����ɢ����ɫ	 
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 1.0,0.0,0.0,1.0 };       //̫����ɫΪ��ɫ     
	GLfloat mat_emmision[] = { 1.0,1.0,1.0,1.0 };   //�Է���Ϊ�׹�
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //���ʾ��淴����ɫ
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //���淴��ָ��
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //���ʵ�ɢ����ɫ
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //���ʵĻ�����ɫ 
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision);     //���ʵĻ�����ɫ 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  ���������Ļ������RGBAǿ��
}
//�������
void material_earth(void)
{
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //���ʾ��淴����ɫ����
	GLfloat mat_shininess[] = { 50.0 };                   // ���淴��ָ������
	GLfloat mat_diffuse[] = { 1.0,1.0,1.0,1.0 };           //����ɢ����ɫ	 
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1,0.1,0.1,1.0 };       //������ɫΪ��ɫ   
	GLfloat mat_emmision[] = { 0.2, 0.2,0.2, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //���ʾ��淴����ɫ
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //���淴��ָ��
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //���ʵ�ɢ����ɫ
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //���ʵĻ�����ɫ 
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision);     //���ʵĻ�����ɫ 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  ���������Ļ������RGBAǿ��
}
//�������
void material_moon(void)
{
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };      //���ʾ��淴����ɫ����
	GLfloat mat_shininess[] = { 50.0 };                   // ���淴��ָ������
	GLfloat mat_diffuse[] = { 0.0,0.0,1.0,1.0 };           //����ɢ����ɫ	 
	GLfloat white_light[] = { 1.0, 0.0,0.0, 1.0 };

	GLfloat lmodel_ambient[] = { 0.5,0.5,0.5,1.0 };       //������ɫΪ��ɫ     


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   //���ʾ��淴����ɫ
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); //���淴��ָ��
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //���ʵ�ɢ����ɫ
	glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);     //���ʵĻ�����ɫ 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //  ���������Ļ������RGBAǿ��
}

void sun()
{
	qobj = gluNewQuadric();//����ռ�
	glPushMatrix();
	material_sun();
	glBindTexture(GL_TEXTURE_2D, texName);//������һ���󶨵�Ŀ������������Ƶ�����
	glEnable(GL_TEXTURE_2D);//���ö�ά����
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);   //̫����ת	
	glInitNames();
	glPushName(0);
	glLoadName(1);
	gluSphere(qobj, 0.75, 60, 60); //��������qobj
	glDisable(GL_TEXTURE_2D);//���ö�ά����
	glPopMatrix();
}

//���Ƶ��������
void earth()
{
	eobj = gluNewQuadric();
	glPushMatrix();
	material_earth();
	glBindTexture(GL_TEXTURE_2D, texName2);//������һ���󶨵�Ŀ������������Ƶ�����
	glEnable(GL_TEXTURE_2D);//���ö�ά����
	gluQuadricTexture(eobj, GL_TRUE);//������
	glRotatef((GLfloat)day, aix_x, aix_y, aix_z);//��ת
	glTranslatef(1.5, 0.0, 0.0);//ƽ��
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);//��ת
	gluSphere(eobj, 0.15, 20, 16);
	glDisable(GL_TEXTURE_2D);

	//��������
	mobj = gluNewQuadric();
	material_moon();
	glBindTexture(GL_TEXTURE_2D, texName3);//������һ���󶨵�Ŀ������������Ƶ�����
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(mobj, GL_TRUE);//������
	glRotatef((GLfloat)day, 0, 1, 0);//��ת
	glTranslatef(0.3, 0, 0);
	gluSphere(mobj, 0.08, 20, 16);
	glRotatef((GLfloat)month, 0.0, 1.0, 0.0);//��ת
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void Rotate(void)          //���ø����ǵĹ�ת����
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

