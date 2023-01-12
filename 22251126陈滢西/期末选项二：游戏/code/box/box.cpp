// map.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <math.h>
#include <GL/glu.h>
#include <gl/glaux.h>
#include<GL/glut.h>
#include <gl/gl.h>
#include <stdio.h>
#include<string>
#include"SOIL.h"
#include <io.h>
#include <stdio.h>
#include<map>
#include<valarray>

#define PI 3.1415926
#define AF 60*(3.14/180) 
#define AF1 60*(3.14/180) 
#define GL_CLAMP_TO_EDGE 0x812F
#define AF2 75*(3.14/180)
#define AF3 110*(3.14/180)

#define TREE 6
#define STEM 1
#define LEAF 2
#define LEAF_MAT 3
#define TREE_MAT 4
#define STEMANDLEAVES 5

HWND hWnd;
HDC hDC;
HGLRC hRC = NULL;
HINSTANCE hInstance;
RECT rect;
using namespace std;
int sw = 800;
int sh = 600;
//视角控制
float look_x = 0,
look_y =0,
look_z = 0,
look_x_temp = 0,
look_z_temp = 0;
float screenrate_x, screenrate_y;//鼠标屏幕坐标相对于中心点移动的比例
float r = 10;
//相机位置
float cpos_x = 1,
cpos_y = 25,
cpos_z = 50;
bool teaflag = true;
int centerpoint_x = sw / 2;
int centerpoint_y = sh / 2;
bool flag;
unsigned char* image;
int width, height;
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat LightPosition[] = { 0.0f, 0.0f, -0.1 * 20.0f, 1.0f };

GLuint filter;
GLuint	texture[50];
int i = 1;
float heading;
float xpos;
float zpos = 7.0f;
const double pi = 3.1415926536;
const float piover180 = 0.0174532925f;
const float p = 0.1f;
const float q = 0.4f;

GLfloat aspect;
GLfloat yup;
GLfloat xup;
GLfloat zup;
GLfloat xrot;
GLfloat yrot;
GLfloat lookupdown = -5.0f;
GLfloat xspeed;
GLfloat yspeed;

bool fullscreen = 1;		//			
bool goodlight;				//			
bool light;					//		
bool blend;					//			
bool lp;					//			
bool bp;					//			

//树

int Rebuild = 1;
int Level = 6;
double g_r = 8, g_theta = PI / 2.0, g_phai = PI / 2.0;
float treePos[3] = { 0, -2, 4 };

bool bool_select_area = false;
void touch(int x, int y);


int viewport[4];

bool dooropen = false;
bool clicktip1 = false;
bool stop = false;
bool startcount = false;
int count1 = 0;
bool open1 = false;

/*翻翻乐*/
bool fl1=false;
bool fl2=false;
bool fl3=false;
bool fr1=false;
bool fr2=false;
bool fr3=false;
bool right1 = false;
bool right2 = false;
bool right3 = false;
int fcount = 0;

bool firstplay = true;//第一关，正常墙
bool secondplay = false;//第二关
bool thirdplay = false;//第三关
bool play3_5 = false;//第三关
bool finalplay = false;//最后漫游

bool mirrodisplay = false;
bool displayjade = true;

bool showsmalldoor = false;
bool flagv1 = true;
bool flagv2 = true;
bool flagv3 = true;
bool flagv4 = true;
/*static M3DVector3f corners[] = {
	-25, 25, 25,
	25, 25, 25,
	25, -25, 25,
	-25, -25, 25,
	-25, 25, -25,
	25, 25, -25,
	25, -25, -25,
	-25, -25, -25
};*/
typedef struct _FPoint3 {
	int x;
	int y;
	int z;
	int r;
}FPoint3;
#pragma comment( lib, "opengl32.lib")	
#pragma comment( lib, "glu32.lib")
#pragma comment( lib, "glaux.lib")
#include"resource.h"
#include<Windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

void skybox();
void wall();
void floor();
void leaf1(float x0, float y0, float z0, float x1, float y1, float z1, float angle1, float angle2, int n);//分形竹子
void leaf2(float x0, float y0, float z0, float x1, float y1, float z1, float angle1, float angle2, int n);
void FractalTree(int level);
inline float randf();
void FractalTree(int level);
void CreateTreeLists(void);
void SetupMaterials(void);
void carboard();
void sword();//草雉剑
void mirror();//八咫镜
void Jade();//八尺琼曲玉
void nengju();//能剧面具
bool findtreasure1 = false;
bool findtreasure2 = false;
bool findtreasure3 = false;
void table();
void lantern(int x,int z);
void bed();
void carboard2();
void carboard3();
enum GLFontType
{
	GL_FONT_SOLID,
	GL_FONT_LINE,
};






//载入位图
void LoadGLTextures()									//调用Bitmap并转换成纹理
{
	
	int Status = FALSE;									//状态确定

	AUX_RGBImageRec *TextureImage[1];					//为纹理创建存储空间

	memset(TextureImage, 0, sizeof(void *) * 1);         //将指针设为NULL

			  //读取位图，检查错误。如果位图不存在则退出


	
		Status = TRUE;
		glGenTextures(1, &texture[0]); 
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		image = SOIL_load_image("skybox/back.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Status = TRUE;
		glGenTextures(1, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		image = SOIL_load_image("skybox/bottom.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Status = TRUE;
		glGenTextures(1, &texture[2]);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		image = SOIL_load_image("skybox/front.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Status = TRUE;
		glGenTextures(1, &texture[3]);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		image = SOIL_load_image("skybox/left.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Status = TRUE;
		glGenTextures(1, &texture[4]);

		glBindTexture(GL_TEXTURE_2D, texture[4]);
		image = SOIL_load_image("skybox/right.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		Status = TRUE;
		glGenTextures(1, &texture[5]);
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		image = SOIL_load_image("skybox/top.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Status = TRUE;
		glGenTextures(1, &texture[6]);
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		image = SOIL_load_image("wallandfloor/door1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		Status = TRUE;
		glGenTextures(1, &texture[7]);
		glBindTexture(GL_TEXTURE_2D, texture[7]);
		image = SOIL_load_image("wallandfloor/stone.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		Status = TRUE;
		glGenTextures(1, &texture[8]);
		glBindTexture(GL_TEXTURE_2D, texture[8]);
		image = SOIL_load_image("wallandfloor/nwall2.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Status = TRUE;
		glGenTextures(1, &texture[9]);
		glBindTexture(GL_TEXTURE_2D, texture[9]);
		image = SOIL_load_image("wallandfloor/wall.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		Status = TRUE;
		glGenTextures(1, &texture[10]);
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		image = SOIL_load_image("wallandfloor/nwalltiao.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		Status = TRUE;
		glGenTextures(1, &texture[11]);
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		image = SOIL_load_image("wallandfloor/text.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenTextures(1, &texture[12]);
		glBindTexture(GL_TEXTURE_2D, texture[12]);
		image = SOIL_load_image("wallandfloor/wall3.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenTextures(1, &texture[13]);
		glBindTexture(GL_TEXTURE_2D, texture[13]);
		image = SOIL_load_image("wallandfloor/door.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &texture[14]);
		glBindTexture(GL_TEXTURE_2D, texture[14]);
		image = SOIL_load_image("wallandfloor/withdraw.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &texture[15]);
		glBindTexture(GL_TEXTURE_2D, texture[15]);
		image = SOIL_load_image("wallandfloor/wood3.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Status = TRUE;
		glGenTextures(1, &texture[16]);
		glBindTexture(GL_TEXTURE_2D, texture[16]);
		image = SOIL_load_image("wallandfloor/paint1.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		Status = TRUE;
		glGenTextures(1, &texture[17]);
		glBindTexture(GL_TEXTURE_2D, texture[17]);
		image = SOIL_load_image("wallandfloor/one.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[18]);
		glBindTexture(GL_TEXTURE_2D, texture[18]);
		image = SOIL_load_image("wallandfloor/two.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[19]);
		glBindTexture(GL_TEXTURE_2D, texture[19]);
		image = SOIL_load_image("wallandfloor/three.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[20]);
		glBindTexture(GL_TEXTURE_2D, texture[20]);
		image = SOIL_load_image("wallandfloor/four.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[21]);
		glBindTexture(GL_TEXTURE_2D, texture[21]);
		image = SOIL_load_image("wallandfloor/open.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[22]);
		glBindTexture(GL_TEXTURE_2D, texture[22]);
		image = SOIL_load_image("wallandfloor/jingzi.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[23]);
		glBindTexture(GL_TEXTURE_2D, texture[23]);
		image = SOIL_load_image("wallandfloor/caozhijian.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[24]);
		glBindTexture(GL_TEXTURE_2D, texture[24]);
		image = SOIL_load_image("wallandfloor/gouyu.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[25]);
		glBindTexture(GL_TEXTURE_2D, texture[25]);
		image = SOIL_load_image("wallandfloor/nengju1.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[26]);
		glBindTexture(GL_TEXTURE_2D, texture[26]);
		image = SOIL_load_image("wallandfloor/nengju2.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[27]);
		glBindTexture(GL_TEXTURE_2D, texture[27]);
		image = SOIL_load_image("wallandfloor/nengju3.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[28]);
		glBindTexture(GL_TEXTURE_2D, texture[28]);
		image = SOIL_load_image("wallandfloor/nengju4.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[29]);
		glBindTexture(GL_TEXTURE_2D, texture[29]);
		image = SOIL_load_image("wallandfloor/tip2.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[30]);
		glBindTexture(GL_TEXTURE_2D, texture[30]);
		image = SOIL_load_image("wallandfloor/woodshen.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[31]);
		glBindTexture(GL_TEXTURE_2D, texture[31]);
		image = SOIL_load_image("wallandfloor/dengzi.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[32]);
		glBindTexture(GL_TEXTURE_2D, texture[32]);
		image = SOIL_load_image("wallandfloor/feng.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;
		
		Status = TRUE;
		glGenTextures(1, &texture[33]);
		glBindTexture(GL_TEXTURE_2D, texture[33]);
		image = SOIL_load_image("wallandfloor/latern.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[34]);
		glBindTexture(GL_TEXTURE_2D, texture[34]);
		image = SOIL_load_image("wallandfloor/bedb.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[35]);
		glBindTexture(GL_TEXTURE_2D, texture[35]);
		image = SOIL_load_image("wallandfloor/bed.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[36]);
		glBindTexture(GL_TEXTURE_2D, texture[36]);
		image = SOIL_load_image("wallandfloor/carboard1.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[37]);
		glBindTexture(GL_TEXTURE_2D, texture[37]);
		image = SOIL_load_image("wallandfloor/carboard2.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[38]);
		glBindTexture(GL_TEXTURE_2D, texture[38]);
		image = SOIL_load_image("wallandfloor/carboard3.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[39]);
		glBindTexture(GL_TEXTURE_2D, texture[39]);
		image = SOIL_load_image("wallandfloor/mie.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[40]);
		glBindTexture(GL_TEXTURE_2D, texture[40]);
		image = SOIL_load_image("wallandfloor/tip4.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;


		Status = TRUE;
		glGenTextures(1, &texture[41]);
		glBindTexture(GL_TEXTURE_2D, texture[41]);
		image = SOIL_load_image("wallandfloor/tip3.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[42]);
		glBindTexture(GL_TEXTURE_2D, texture[42]);
		image = SOIL_load_image("wallandfloor/tip5.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[43]);
		glBindTexture(GL_TEXTURE_2D, texture[43]);
		image = SOIL_load_image("wallandfloor/c4.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[44]);
		glBindTexture(GL_TEXTURE_2D, texture[44]);
		image = SOIL_load_image("wallandfloor/c5.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[45]);
		glBindTexture(GL_TEXTURE_2D, texture[45]);
		image = SOIL_load_image("wallandfloor/f1.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[46]);
		glBindTexture(GL_TEXTURE_2D, texture[46]);
		image = SOIL_load_image("wallandfloor/f2.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[47]);
		glBindTexture(GL_TEXTURE_2D, texture[47]);
		image = SOIL_load_image("wallandfloor/f3.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;

		Status = TRUE;
		glGenTextures(1, &texture[48]);
		glBindTexture(GL_TEXTURE_2D, texture[48]);
		image = SOIL_load_image("wallandfloor/tipf.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Status = TRUE;
if (TextureImage[0])								//纹理是否存在
	{
		if (TextureImage[0]->data)						//纹理图象是否存在
		{
			free(TextureImage[0]->data);				//释放纹理图象所占用内存
		}
		free(TextureImage[0]);							//释放图象结构
	}
	
	}
	



//天空盒

	void setlight2()
	{
		//设置LIGHT2
		GLfloat light2_position[] = { 0,0,0,1 };
		GLfloat light2_direction[] = { 0,0,0 };
		light2_position[0] = cpos_x;//根据相机位置设置LIGHT1位置
		light2_position[1] = cpos_y;
		light2_position[2] = cpos_z;
		light2_direction[0] = +cpos_x +look_x;//根据相机朝向设置LIGHT1朝向
		light2_direction[1] = +cpos_y + look_y;
		light2_direction[2] = +cpos_z - look_z;
		glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
	}
//初始化，设置光源，打开纹理开关
void SceneInit(int w, int h)
{
	LoadGLTextures();			//条到处理调用位图并用此位图建立纹理的子程序
	glEnable(GL_TEXTURE_2D);	//纹理被创建，并允许2D纹理贴图

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.5);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//设置光源参数
	/*glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		//设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		//设置漫反射光
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	//光源的位置*/
	//glEnable(GL_LIGHT1);								//允许光源一

	glColor4f(1.0f, 1.0f, 1.0f, 0.5);					//全亮度，50%的透明
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	
	
}

//设置视角
void SceneResizeViewport(GLsizei w, GLsizei h)
{
	if (h == 0)
	{
		h = 1;
	}
	aspect = (GLfloat)w / (GLfloat)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	
	gluPerspective(30.f, aspect,1.0f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cpos_x, cpos_y, cpos_z, cpos_x + look_x, cpos_y + look_y, cpos_z - look_z, 0, 1, 0);//注意因为相机是面朝z负方向，所以cpos_z - lookz
	//GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };  //镜面反射参数
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };  //漫反射参数
	GLfloat mat_shininess[] = { 0.0 };         //高光指数
	glShadeModel(GL_SMOOTH);           //多变性填充模式
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glDisable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glShadeModel(GL_SMOOTH);
	SetupMaterials();
	CreateTreeLists();

	/*GLfloat tree_ambuse[] = { 0.4, 0.25, 0.1, 1.0 };
	GLfloat tree_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat tree_shininess[] = { 10 };

	GLfloat leaf_ambuse[] = { 0.0, 0.8, 0.0, 1.0 };
	GLfloat leaf_specular[] = { 0.0, 0.8, 0.0, 1.0 };
	GLfloat leaf_shininess[] = { 10 };

	glNewList(TREE_MAT, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tree_ambuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tree_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, tree_shininess);
	glEndList();

	glNewList(LEAF_MAT, GL_COMPILE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, leaf_ambuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leaf_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, leaf_shininess);
	glEndList();

	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glShadeModel(GL_SMOOTH);
	SetupMaterials();
	CreateTreeLists();*/
	/*
	M3DMatrix44f mat_proj, mat_modelview;
	// 获取模型视图矩阵
	glGetFloatv(GL_MODELVIEW_MATRIX, mat_modelview);

	glBegin(GL_QUADS);
	glVertex3fv(corners[0]);
	glVertex3fv(corners[1]);
	glVertex3fv(corners[5]);
	glVertex3fv(corners[4]);

	glVertex3fv(corners[4]);
	glVertex3fv(corners[7]);
	glVertex3fv(corners[3]);
	glVertex3fv(corners[0]);

	glVertex3fv(corners[3]);
	glVertex3fv(corners[2]);
	glVertex3fv(corners[6]);
	glVertex3fv(corners[7]);

	glVertex3fv(corners[7]);
	glVertex3fv(corners[6]);
	glVertex3fv(corners[5]);
	glVertex3fv(corners[4]);

	glVertex3fv(corners[5]);
	glVertex3fv(corners[1]);
	glVertex3fv(corners[2]);
	glVertex3fv(corners[6]);

	glVertex3fv(corners[1]);
	glVertex3fv(corners[2]);
	glVertex3fv(corners[3]);
	glVertex3fv(corners[0]);
	glEnd();

	glPopMatrix();
	glPopAttrib();

	// 配置

	selection.set_config(corners, 8, left_bottom, right_top, mat_modelview, mat_proj, viewport);

	/************************************************************************/
	/* 构造一个新的环境                                                                     */
	/************************************************************************/
/*
	if (bool_select_area) {

		selection.draw_area();
		selection.highlight_selected_pts();

	}*/
}

void DrawCuboid(double x, double y, double z)
{
	//     glPushMatrix();
	double cx = x / 2.0;
	double cy = y / 2.0;
	double cz = z / 2.0;
	glBegin(GL_QUADS);
	// 左面
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, -cz);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-cx, cy, -cz);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-cx, cy, cz);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, -cy, cz);
	// 右面
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(cx, -cy, -cz);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(cx, cy, -cz);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(cx, cy, cz);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(cx, -cy, cz);
	// 上面
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, cy, -cz);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(cx, cy, -cz);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(cx, cy, cz);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, cy, cz);
	// 下面
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, -cz);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(cx, -cy, -cz);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(cx, -cy, cz);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, -cy, cz);
	// 前面
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, -cz);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(cx, -cy, -cz);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(cx, cy, -cz);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, cy, -cz);
	// 后面
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-cx, -cy, cz);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(cx, -cy, cz);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(cx, cy, cz);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-cx, cy, cz);
	glEnd();

	
}
// 跟扩展库的绘制结果相同
//绘图画板
void SceneShow(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清屏和清除深度缓冲区
	glMatrixMode(GL_PROJECTION);						//选择投影句镇
	glLoadIdentity();
	
	gluPerspective(30.f, aspect, 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	//基于半透明的混合方式
	//聚光灯（手电筒）设置
	glDisable(GL_LIGHT1);

	glEnable(GL_COLOR_MATERIAL);
	GLfloat yellow_light[] = { 1,0.87,0.315,1.0 };//LIGHT2参数 黄色手电筒
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };  //镜面反射参数
	GLfloat light2_angle[] = { 10.0 };//LIGHT2扩散角度
	GLfloat lightKc = 1, lightKl = 0.0, lightKq = 0.0, lightExp = 2;//有关点光源扩散强度衰减的参数。
	glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, &lightKc);
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, &lightKl);
	glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, &lightKq);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, light2_angle);
	glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &lightExp);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//设置LIGHT2
	GLfloat light2_position[] = { 0,0,0,1 };
	GLfloat light2_direction[] = { 0,0,0 };
	light2_position[0] = cpos_x;//根据相机位置设置LIGHT1位置
	light2_position[1] = cpos_y;
	light2_position[2] = cpos_z;
	light2_direction[0] = 0;//根据相机朝向设置LIGHT1朝向
	light2_direction[1] =  0;
	light2_direction[2] = -0.02;
	glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light2_direction);
	glEnable(GL_LIGHT1);
	glLoadIdentity();		

	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -0.25f;
	GLfloat sceneroty = 360.0f - yrot;
	
	
	glLoadIdentity();
	gluLookAt(cpos_x, cpos_y, cpos_z, cpos_x + look_x, cpos_y + look_y, cpos_z - look_z, 0, 1, 0);//注意因为相机是面朝z负方向，所以cpos_z - lookz
	glTranslatef(xtrans , ytrans , ztrans );	//移动
																								  //开始绘图
	/*glRotatef(lookupdown, 1.0f, 0, 0);					//X旋转
	glRotatef(sceneroty, 0, 1.0f, 0);					//y旋转*/
	
	glColor3f(1.0, 1.0, 1.0);
	//天空盒
	
	skybox();
	floor();
	
	wall();
	carboard();
	if (secondplay)//第2关开始
	{
		sword();
		glEnable(GL_LIGHTING);
	}
	

	//分形竹子
	//glTranslated(-600, 0, 0);
	glLineWidth(1);
	glColor3ub(59, 126, 18);
	glBegin(GL_LINES);
	leaf1(0-600, -200, 0, -600, 100, 0, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(100-600, -200, 0, 100-600, 150, 0, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(100-600, -200, 100, 100-600, 50, 100, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-100-600, -200, 0, -100-600, 50, 0, 0, AF2, 5);
	glEnd();
	glFlush();

	glBegin(GL_LINES);
	leaf1(100-600, -200, -100, 100-600, 100, -100, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-100-600, -200, -100, -100-600, 50, -100, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(50-600, -200, 110, 50-600, 50, 110, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-100-600, -200, 100, -100-600, 50, 100, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-100 - 600, -200, 300, -100 - 600, 50, 300, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-400, -200, 250, -400, 50, 250, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-450, -200, 260, -450, 50, 260, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-460, -200, 270, -460, 50, 270, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-700, -200, 500, -700, 50, 500, 0, AF2, 5);
	glEnd();
	glFlush();

	glBegin(GL_LINES);
	leaf2(-700, -200, 300, -700, 50, 300, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-600, -200, 265, -600, 50, 265, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-465, -200, 270, -465, 50, 270, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-700, -200, 450, -700, 50, 450, 0, AF2, 5);
	glEnd();
	glFlush();

	glBegin(GL_LINES);
	leaf2(-700, -200, 200, -700, 100, 200, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-600, -200, 215, -600, 150, 215, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-465, -200, 210, -465, 150, 210, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-700, -200, 420, -700, 150, 420, 0, AF2, 5);
	glEnd();
	glFlush();

	leaf1(-465, -200, 400, -465, 150, 400, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-700, -200, 520, -700, 150, 520, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-700, -200, 500, -700, 50, 500, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-600, -200, 665, -600, 50, 665, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-465, -200, 770, -465, 50, 770, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-700, -200, 450, -700, 50, 450, 0, AF2, 5);
	glEnd();
	glFlush();

	glBegin(GL_LINES);
	leaf1(100 - 600, -200, 700, 100 - 600, 100, 700, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-100 - 600, -200, -720, -100 - 600, 50, -720, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(50 - 600, -200, 670, 50 - 600, 50, 670, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-100 - 600, -200, 550, -100 - 600, 50, 550, 0, AF2, 5);
	glEnd();
	glFlush();


	glBegin(GL_LINES);
	leaf2(-100 - 600, -200, 580, -100 - 600, 50, 580, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-400, -200, 470, -400, 50, 470, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf2(-450, -200, 330, -450, 50, 330, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-460, -200, 310, -460, 50,310, 0, AF2, 5);
	glEnd();
	glFlush();
	glBegin(GL_LINES);
	leaf1(-700, -200, 220, -700, 50, 220, 0, AF2, 5);
	glEnd();
	glFlush();
	glColor3f(1.0f, 1.0f, 1.0f);
	table();
	lantern(-500, 300);
	lantern(-400, 100);
	lantern(-400, 500);
	lantern(-560, 356);
	glColor3f(1.0f, 1.0f, 1.0f);
	if (!finalplay)
	{
		if (clicktip1)
		{
			glLoadIdentity();
			glTranslatef(xtrans , ytrans, ztrans );	//移动

			glBindTexture(GL_TEXTURE_2D, texture[11]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);  glVertex3d(-20 + 260, 35 - 10, 300);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(20 + 260, 35 - 10, 300);
			glTexCoord2f(1.0, 1.0f);  glVertex3d(20 + 260, -5 - 10, 300);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(-20 + 260, -5 - 10, 300);
			glEnd();
		}
		else
		{


			glBindTexture(GL_TEXTURE_2D, texture[11]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);  glVertex3d(350, 22, 470);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(350, 22, 520);
			glTexCoord2f(1.0f, 1.0f);  glVertex3d(300, 22, 520);
			glTexCoord2f(0.0f, 1.0f);    glVertex3d(300, 22, 470);
			glEnd();
		}
	}

	if (secondplay) {
		glLoadIdentity();
		gluLookAt(cpos_x, cpos_y, cpos_z, cpos_x + look_x, cpos_y + look_y, cpos_z - look_z, 0, 1, 0);//注意因为相机是面朝z负方向，所以cpos_z - lookz//重置当前MODELVIEW矩阵
		glTranslatef(xtrans + xup, ytrans + yup, ztrans + zup);	//移动
		glTranslatef(-600.0, 0.0, 100.0);	//移动
		glScalef(50.0f, 50.0f, 50.0f);
		glColor3ub(66, 35, 36);
		if (Rebuild)
		{
			glNewList(TREE, GL_COMPILE);
			FractalTree(0);
			glEndList();
			Rebuild = 0;
		}
		glCallList(TREE);

		glLoadIdentity();
		gluLookAt(cpos_x, cpos_y, cpos_z, cpos_x + look_x, cpos_y + look_y, cpos_z - look_z, 0, 1, 0);//注意因为相机是面朝z负方向，所以cpos_z - 
		glTranslatef(xtrans + xup, ytrans + yup, ztrans + zup);	//移动
		glTranslatef(-600.0, 0.0, 300.0);	//移动
		glScalef(50.0f, 50.0f, 50.0f);
		glColor3ub(66, 35, 36);
		if (Rebuild)
		{
			glNewList(TREE, GL_COMPILE);
			FractalTree(0);
			glEndList();
			Rebuild = 0;
		}
		glCallList(TREE);

		glLoadIdentity();
		gluLookAt(cpos_x, cpos_y, cpos_z, cpos_x + look_x, cpos_y + look_y, cpos_z - look_z, 0, 1, 0);//注意因为相机是面朝z负方向，所以cpos_z - 
		glTranslatef(xtrans + xup, ytrans + yup, ztrans + zup);	//移动
		glTranslatef(-600.0, 0.0, 500.0);	//移动
		glScalef(50.0f, 50.0f, 50.0f);
		glColor3ub(66, 35, 36);
		if (Rebuild)
		{
			glNewList(TREE, GL_COMPILE);
			FractalTree(0);
			glEndList();
			Rebuild = 0;
		}
		glCallList(TREE);
	}
	glFlush();
	glLoadIdentity();
	
}

//OpenGL开关
void EnableOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	hDC = GetDC(hWnd);

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(hDC, &pfd);

	SetPixelFormat(hDC, iFormat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
}
void DisableOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}
bool cannotgo(int xpos, int zpos)
{
	if (xpos >= 200 && xpos <= 400 && zpos <= 15 && zpos >= -15)//1
	{
		return true;
	}
	if (xpos >= 375 && zpos >= 0 && zpos <= 730)//8
	{
		return true;
	}
	if (zpos >= 450)//9
	{
		return true;
	}
	if (xpos>=400-15 &&zpos>=-730&&zpos<=0)//2
	{
		return true;
	}
	if (zpos<=-730+15 )//7 5
	{
		return true;
	}
	if (xpos >= - 100 && zpos >= -730 &&zpos<=-150&& xpos <= 3)//3
	{
		return true;
	}
	if (xpos <=-400 +15 && zpos >= -730 && zpos <= 0 )//6
	{
		return true;
	}
	if (xpos <= -600 && zpos >=0 && zpos <= 730)//6
	{
		return true;
	}
	if (!open1)
	{
		if (xpos >= -20 && xpos <= 150 && zpos <= 3 && zpos >= -15&&!dooropen)
		{
			return true;
		}
	}
	if (!showsmalldoor)
	{
		if (xpos >= -20 && xpos <= 3 && zpos <= 0 && zpos >= -100 && !dooropen)
		{
			return true;
		}
	}
	if(finalplay)
	{
		if (xpos >= 128 && xpos <= 262 && zpos <= -550 && zpos >= -730 )
		{
			return true;
		}
		else if (xpos >= 330 && xpos <= 400 && zpos <= -290 && zpos >= -490)
		{
			return true;
		}
		else if (xpos >= 330 && xpos <= 400 && zpos <= -170 && zpos >= -270)
		{
			return true;
		}
	}
	if (xpos >= -250 && xpos <= -150 && zpos <= -185 && zpos >= -300)
	{
		return true;
	}
	return false;
}
bool ChangeResolution(int w, int h, int bitdepth)
{
	DEVMODE devMode;
	int modeSwitch;
	int closeMode = 0;

	EnumDisplaySettings(NULL, closeMode, &devMode);

	devMode.dmBitsPerPel = bitdepth;
	devMode.dmPelsWidth = w;
	devMode.dmPelsHeight = h;
	devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

	if (modeSwitch == DISP_CHANGE_SUCCESSFUL)
	{
		return true;
	}
	else
	{
		ChangeDisplaySettings(NULL, 0);
		return false;
	}
}
float step = 0.5;
float offsetx=0;
float offsety=0;
float r_castlenght=0;
bool state = true;
int buttons = 0;
//键盘控制
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int width = sw, height =sh;
	switch (message)
	{
	case WM_CREATE:
		GetWindowRect(hWnd, &rect);
		sw = rect.right - rect.left;
		sh = rect.bottom - rect.top;
		SceneResizeViewport(sw, sh);
		return 0;

	case WM_SIZE:
		if (!fullscreen)
		{
			GetWindowRect(hWnd, &rect);
			sw = rect.right - rect.left;
			sh = rect.bottom - rect.top;
			if (sw>0 && sh>0)
				SceneResizeViewport(sw, sh);
		}
		else
		{
			SceneResizeViewport(GetSystemMetrics(SM_CXSCREEN),
				GetSystemMetrics(SM_CYSCREEN));
		}
		return 0;

	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		PostQuitMessage(0);
		return 0;

	case WM_DESTROY:
		return 0;
	case WM_MOUSEMOVE:  //捕捉鼠标移动消息WM_MOUSEMOVE
		int xPos, yPos;
		//得到当前鼠标的位置
		//LOWORD 和 HIWORD 是宏，表示取一个长整形数的低16位与高16位
		xPos = LOWORD(lParam);   //获取鼠标的X坐标
		yPos = HIWORD(lParam);   //获取鼠标的Y坐标
		 offsetx = xPos - centerpoint_x;
		 offsety = yPos - centerpoint_y;
		screenrate_x = offsetx / centerpoint_x * PI;//用于摄像机水平移动
		screenrate_y = offsety / centerpoint_y * PI / 2;//用于摄像机上下移动
		//水平方向
		look_x_temp = r * sin(screenrate_x);
		look_z_temp = r * cos(screenrate_x);//最后使用时要和相机坐标相加/减
		//竖直方向
		look_y = r * sin(-screenrate_y);
		 r_castlenght = abs(r * cos(screenrate_y));//投影在xz面的长度
		//根据长度计算真正的look_x,look_z
		look_x = r_castlenght * look_x_temp / r;
		look_z = r_castlenght * look_z_temp / r;
		 buttons = (int)wParam;
		if (buttons == MK_LBUTTON) {

			touch(xPos, yPos);

			
		}
		else if (buttons == MK_RBUTTON) {

			if (xpos >= 200 && xpos <= 400 && zpos >= 300 && zpos <= 730)//点击提示
			{
				if (clicktip1)
				{
					clicktip1 = false;
					stop = false;
					startcount = true;
				}
				else
				{
					clicktip1 = true;
					stop = true;
					startcount = false;
					count1 = 0;
				}
			}
			else if (xpos >= 300 && xpos <= 400 && zpos <= -100 && zpos >= -300)//点击右一面具
			{
				if (thirdplay) {


					if (fr1)
					{
						fr1 = false;
						fcount--;
					}
					else
					{
						if (!right1) {
							if (fcount <= 1) {
								fr1 = true;
								fcount++;
								if (fl2 == true)
								{
									right1 = true;
									fcount -= 2;
									if (right1&&right2&&right3) {
										if (flagv2)
										{
											PlaySound(LPCSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
											flagv2 = false;
										}
										play3_5 = true;
									}
								}
							}
						}
					}
				}

			}
			else if (xpos >= 300 && xpos <= 400 && zpos <= -320 && zpos >= -500)//点击右二面具
			{
				if (firstplay) {
					secondplay = true;
					if (flagv3)
					{
						PlaySound(LPCSTR(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
						flagv3 = false;
					}
				}
				else if (thirdplay) {
					if (fr2)
					{
						fr2 = false;
						fcount--;
					}
					else
					{
						if (!right2) {
							if (fcount <= 1) {
								fr2 = true;
								fcount++;
								if (fl1 == true)
								{
									right2 = true;
									fcount -= 2;
									if (right1&&right2&&right3) {
										if (flagv2)
										{
											PlaySound(LPCSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
											flagv2 = false;
										}
										play3_5 = true;
									}
								}
							}
						}
					}
				}
			}
			
			else if (xpos >= 300 && xpos <= 400 && zpos <= -520 && zpos >= -700)//点击右三面具
			{
				if (thirdplay) {


					if (fr3)
					{
						fr3 = false;
						fcount--;
					}
					else
					{
						if (!right3) {
							if (fcount <= 1) {
								fr3 = true;
								fcount++;
								if (fl3 == true)
								{
									right3 = true;
									fcount -= 2;
									if (right1&&right2&&right3) {
										if (flagv2)
										{
											PlaySound(LPCSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
											flagv2 = false;
										}
										play3_5 = true;
									}
								}
							}
						}
					}

				}
			}
			else if (xpos >= 0 && xpos <= 350 && zpos <= -100 && zpos >= -300)//点击左一面具
			{
				if (thirdplay) {
					if (fl1)
					{
						fl1 = false;
						fcount--;
					}
					else
					{
						if (!right2) {
							if (fcount <= 1) {

								fl1 = true;
								fcount++;
								if (fr2 == true)
								{
									right2 = true;
									fcount -= 2;
									if (right1&&right2&&right3) {
										if (flagv2)
										{
											PlaySound(LPCSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
											flagv2 = false;
										}
										play3_5 = true;
									}
								}
							}
						}

					}
				}

			}
			else if (xpos >= 0 && xpos <= 350 && zpos <= -320 && zpos >= -500)//点击左二面具
			{
				if (thirdplay) {
					if (fl2)
					{
						fl2 = false;
						fcount--;
					}
					else
					{

						if (fcount <= 1) {
							if (!right1) {


								fl2 = true;
								fcount++;
								if (fr1 == true)
								{
									right1 = true;
									fcount -= 2;
									if (right1&&right2&&right3) {
										if (flagv2)
										{
											PlaySound(LPCSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
											flagv2 = false;
										}
										play3_5 = true;
									}
								}
							}
						}
					}
				}
			}

			else if (xpos >= 0 && xpos <= 350 && zpos <= -520 && zpos >= -700)//点击左三面具
			{
				if (thirdplay) {
					if (fl3)
					{
						fl3 = false;
						fcount--;
					}
					else
					{
						if (!right3) {
							if (fcount <= 1) {
								fl3 = true;
								fcount++;
								if (fr3 == true)
								{

									right3 = true;
									fcount -= 2;
									if (right1&&right2&&right3) {
										if (flagv2)
										{
											PlaySound(LPCSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
											flagv2 = false;
										}
										play3_5 = true;
									}
								}
							}
						}
					}

				}
			}
			else if (xpos >= -750 && xpos <= -400 && zpos <= 220 && zpos >= 50) {//点击到天云丛剑
			
			thirdplay = true;
			firstplay = false;
			secondplay = false;
			}
			else if (xpos >= 120 && xpos <= 240  && zpos <= -400) {

			mirrodisplay = true;//镜子消失
			displayjade = false;//显示勾玉
			showsmalldoor = true;
			}
			else if (zpos >= -100 && zpos <= 0 && xpos <= -150 && xpos >= -250)
			{
			displayjade = true;
			finalplay = true;
			play3_5 = false;
			thirdplay = false;
			if (flagv4)
			{
				PlaySound(LPCSTR(IDR_WAVE4), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
				glDisable(GL_LIGHTING);
				flagv4 = false;
			}

			}
		}
		
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
				
		case 'A':
			xpos += -look_z_temp * step;
			zpos -= look_x_temp * step;
			if (cannotgo(xpos, zpos) || stop)
			{
				if (!stop)
				{
					if (startcount)
					{
						count1++;
					}
				}
				xpos -= -look_z_temp * step;
				zpos += look_x_temp * step;
			}
			break;
		case 'D':  
			xpos += look_z_temp * step;
			zpos -= -look_x_temp * step;
			if (cannotgo(xpos, zpos) || stop)
			{
				if (!stop)
				{
					if (startcount)
					{
						count1++;
					}
				}
				xpos -= look_z_temp * step;
				zpos += -look_x_temp * step;
			}
			break;
			break;
		case 'W':  
			xpos += look_x_temp * step;
			zpos -= look_z_temp * step;
			if (cannotgo(xpos, zpos) || stop)
			{
				if (!stop)
				{
					if (startcount)
					{
						count1++;
						if (count1 > 16)
						{
							open1 = true;
						}
					}

				}
				xpos -= look_x_temp * step;
				zpos += look_z_temp * step;
			}
			break;
		case 'S':  
			xpos -= look_x_temp * step;
			zpos += look_z_temp * step;
			if (cannotgo(xpos, zpos) || stop)
			{
				if (!stop)
				{
					if (startcount)
					{
						count1++;
					}
				}
				xpos += look_x_temp * step;
				zpos -= look_z_temp * step;
			}
			break;
		
		}
	
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

//不按鼠标移动事件
/*void onMouseMovePassive(int screen_x, int screen_y) {
	float offsetx = screen_x - centerpoint_x;
	float offsety = screen_y - centerpoint_y;
	screenrate_x = offsetx / centerpoint_x * PI;//用于摄像机水平移动
	screenrate_y = offsety / centerpoint_y * PI / 2;//用于摄像机上下移动
	//水平方向
	look_x_temp = r * sin(screenrate_x);
	look_z_temp = r * cos(screenrate_x);//最后使用时要和相机坐标相加/减
	//竖直方向
	look_y = r * sin(-screenrate_y);
	float r_castlenght = abs(r * cos(screenrate_y));//投影在xz面的长度
	//根据长度计算真正的look_x,look_z
	look_x = r_castlenght * look_x_temp / r;
	look_z = r_castlenght * look_z_temp / r;
	int width = sw, height = sh;
	
	//myDisplay();
	glutPostRedisplay();
}*/



//WIN32全屏
int APIENTRY WinMain(HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	bool bQuit = false;

	if (MessageBox(NULL, "是否选择全屏显示模式", "提示", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = 0;	//窗口模式
	}

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Name";
	RegisterClass(&wc);

	if (fullscreen)
	{
		ChangeResolution(1024, 768, 24);
		hWnd = CreateWindow(
			"Name",
			"陈滢西",
			WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			NULL, NULL,
			hInstance,
			NULL);
	}
	else
	{
		hWnd = CreateWindow(
			"Name",
			"陈滢西",
			WS_TILEDWINDOW | WS_VISIBLE,
			GetSystemMetrics(SM_CXSCREEN) / 2 - sw / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - sh / 2,
			sw,
			sh,
			NULL, NULL,
			hInstance,
			NULL);
		ChangeDisplaySettings(NULL, 0);
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	EnableOpenGL();
	SceneInit(sw, sh);

	if (!fullscreen)
	{
		GetWindowRect(hWnd, &rect);
		sw = rect.right - rect.left;
		sh = rect.bottom - rect.top;
		if (sw > 0 && sh > 0)
			SceneResizeViewport(sw, sh);
	}
	else
	{
		SceneResizeViewport(GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN));
	}

	while (!bQuit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			if (msg.message == WM_QUIT)
				bQuit = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		else
		{

			SceneShow();
			SwapBuffers(hDC);
		}
	}

	DisableOpenGL();

	ShowWindow(hWnd, SW_HIDE);
	DestroyWindow(hWnd);

	ChangeDisplaySettings(NULL, 0);

	return msg.wParam;

	return 0;
}
bool touchdoor(FPoint3 nearPoint, FPoint3 farPoint)
{
	int x1 = nearPoint.x;
	int y1= nearPoint.y;
	int z1= nearPoint.z;
	int x2 = farPoint.x;
	int y2= farPoint.y;
	int z2 = farPoint.z;
	int x0 = 0;/*平面上一点*/
	int y0 = 0;
	int z0 = 0;

	int A = 0, B = 0, C = 1;/*法向量*/
	int D = -(A*x0 + B * y0 + C * z0);
	if ((A*(x2 - x1) + B * (y2 - y1) + C * (z2 - z1)) == 0)
	{
		return false;
	}
	float t = (-D - C * z1 - B * y1 - A * x1)*1.0 / (A*(x2 - x1) + B * (y2 - y1) + C * (z2 - z1));
	int x = (x2 - x1)*t + x1;
	int y = (y2 - y1)*t + y1;
	int z = (z2 - z1)*t + z1;
	if ((x >= 0 && x <= 400) && (y >= 0 && y <= 100))
	{
		if (open1) {
			return true;
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		return false;
	}
}
bool touchtip1(FPoint3 nearPoint, FPoint3 farPoint)
{
	int x1 = nearPoint.x;
	int y1 = nearPoint.y;
	int z1 = nearPoint.z;
	int x2 = farPoint.x;
	int y2 = farPoint.y;
	int z2 = farPoint.z;
	int x0 = 320;/*平面上一点*/
	int y0 = 22;
	int z0 = 500;

	int A = 0, B = 1, C = 0;/*法向量*/
	int D = -(A*x0 + B * y0 + C * z0);
	if ((A*(x2 - x1) + B * (y2 - y1) + C * (z2 - z1)) == 0)
	{
		return false;
	}
	float t = (-D - C * z1 - B * y1 - A * x1)*1.0 / (A*(x2 - x1) + B * (y2 - y1) + C * (z2 - z1));
	int x = (x2 - x1)*t + x1;
	int y = (y2 - y1)*t + y1;
	int z = (z2 - z1)*t + z1;
	if ((x+xpos >= 300 && x+xpos<=400) && (z+zpos >= 380 && z+zpos <= 580))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void touch(int x, int y)
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];



	glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	int winX = x;
	int winY = sh - y;
	GLdouble posX, posY, posZ;
	//获取像素对应的前裁剪面的点坐标
	bool bResult = gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &posX, &posY, &posZ);
	FPoint3 nearPoint;
	nearPoint.x = posX; nearPoint.y = posY; nearPoint.z = posZ;
	//获取像素对应的后裁剪面的点坐标
	bResult = gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &posX, &posY, &posZ);
	FPoint3 farPoint;
	farPoint.x = posX; farPoint.y = posY; farPoint.z = posZ;
	if ((0 - nearPoint.x)*(farPoint.y - nearPoint.y) == (0 - nearPoint.y)*(farPoint.x - nearPoint.x) && (0 - nearPoint.z)*(farPoint.y - nearPoint.y) == (0 - nearPoint.y)*(farPoint.z - nearPoint.z))
		teaflag = false;
	if (touchdoor(nearPoint, farPoint))
	{
		if (dooropen == false)
		{
			dooropen = true;
		}
		else
		{
			dooropen = false;
		}
	}
	
	}
void skybox()
{
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1000.0f, -1000.0f, 1000.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(1000.0f, -1000.0f, 1000.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1000.0f, -1000.0f, 1000.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1000.0f, 1000.0f, 1000.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1000.0f, 1000.0f, -1000.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1000.0f, 1000.0f, -1000.0f);
	glEnd();
	/*栏杆*/
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-400.0,0.0,-120.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-400.0, 30.0, -120.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-400.0, 30.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-400.0, 0.0, 0.0);
	glEnd();
	/*观景室↑*/
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -730.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 100.0, -730.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-400.0, 100.0, -730.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-400.0, 0.0, -730.0);
	glEnd();
	
	
}
void carboard()
{
	glBindTexture(GL_TEXTURE_2D, texture[14]);
	glBegin(GL_QUADS);//下
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(300.0f, 0.0f, 380.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(300.0f, 0.0f, 580.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(300.0, 20.0f, 580.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(300.0f, 20.0f, 380.0f);
	glEnd();
	glBegin(GL_QUADS);//上
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(300.0f, 70.0f, 380.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(300.0f, 70.0f, 580.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(300.0, 90.0f, 580.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(300.0f, 90.0f, 380.0f);
	glEnd();
	glColor3ub(230, 214, 178);
	glBegin(GL_TRIANGLE_FAN);//1
	glVertex3f(350.0, 20.0f, 480.0f);
	glVertex3f(300.0f, 20.0f, 580.0f);
	glVertex3f(400.0f, 20.0f, 580.0f);
	glVertex3f(400.0, 20.0f, 380.0f);
	glVertex3f(300.0f, 20.0f, 380.0f);
	glVertex3f(300.0f, 20.0f, 580.0f);
	glEnd();
	glColor3ub(230, 214, 178);
	glBegin(GL_TRIANGLE_FAN);//1
	glVertex3f(350.0, 70.0f, 485.0f);
	glVertex3f(300.0f, 70.0f, 575.0f);
	glVertex3f(380.0f, 70.0f, 575.0f);
	glVertex3f(380.0, 70.0f, 385.0f);
	glVertex3f(300.0f, 70.0f, 385.0f);
	glVertex3f(300.0f, 70.0f, 575.0f);
	glEnd();
	
	
	/*glBindTexture(GL_TEXTURE_2D, texture[15]);//1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(300.0f, 20.0f, 580.0f);
	glTexCoord2f(4.0f, 0.0f); glVertex3f(400.0f, 20.0f, 580.0f);
	glTexCoord2f(1.0f, 4.0f);   glVertex3f(400.0, 20.0f, 380.0f);
	glTexCoord2f(0.0f, 4.0f);   glVertex3f(300.0f, 20.0f, 380.0f);
	glEnd()*/
	glBindTexture(GL_TEXTURE_2D, texture[15]);//1
	glBegin(GL_QUADS);//2
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(300.0f, 0.0f, 380.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(300.0f, 90.0f, 380.0f);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(380.0, 90.0f, 380.0f);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(380.0f, 0.0f, 380.0f);
	glEnd();
	glBegin(GL_QUADS);//3
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(300.0f, 0.0f, 580.0f);
	glTexCoord2f(4.0f, 0.0f); glVertex3f(300.0f, 90.0f, 580.0f);
	glTexCoord2f(1.0f, 4.0f);   glVertex3f(370.0, 90.0f, 580.0f);
	glTexCoord2f(0.0f, 4.0f);   glVertex3f(370.0f, 0.0f, 580.0f);
	glEnd();
; 
	
	
}
void floor()//室内地板
{
	//glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-20.0f, 0.0f, 730.0f);
	glTexCoord2f(300.0f, 0.0f); glVertex3f(800.0, 0.0f, 730.0f);
	glTexCoord2f(180.0f, 300.0f);  glVertex3f(800.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 300.0f);  glVertex3f(-20.0f, 0.0f, 0.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(300.0f, 0.0f); glVertex3f(400.0, 0.0f, 0.0f);
	glTexCoord2f(180.0f, 300.0f);  glVertex3f(400.0f, 0.0f, -730.0f);
	glTexCoord2f(0.0f, 300.0f);  glVertex3f(0.0f, 0.0f, -730.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-400.0f, 0.0f, 0.0f);
	glTexCoord2f(300.0f, 0.0f); glVertex3f(0.0, 0.0f, 0.0f);
	glTexCoord2f(180.0f, 300.0f);  glVertex3f(0.0f, 0.0f, -730.0f);
	glTexCoord2f(0.0f, 300.0f);  glVertex3f(-400.0f, 0.0f, -730.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-20.0f, -10.0f, 730.0f);
	glTexCoord2f(300.0f, 0.0f); glVertex3f(-20.0, -10.0f, 0.0f);
	glTexCoord2f(180.0f, 300.0f);  glVertex3f(-800.0f, -10.0f, -0.0f);
	glTexCoord2f(0.0f, 300.0f);  glVertex3f(-800.0f, -10.0f, 730.0f);
	glEnd();

		if (startcount)
		{
			if (count1 > 16)
			{
				if (flagv1) {
					PlaySound(LPCSTR(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
					flagv1 = false;
				}
				if (!thirdplay) {
					if (finalplay)
					{
						open1 = true;
						glBindTexture(GL_TEXTURE_2D, texture[48]);
						glBegin(GL_QUADS);
						glTexCoord2f(0.0f, 0.0f);   glVertex3d(400, 80, 125);
						glTexCoord2f(1.0f, 0.0f); glVertex3d(400, 80, 175);
						glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, 175);
						glTexCoord2f(0.0f, 1.0f);    glVertex3d(400, 30, 125);
						glEnd();
					}
					else {
						open1 = true;
						glBindTexture(GL_TEXTURE_2D, texture[21]);
						glBegin(GL_QUADS);
						glTexCoord2f(0.0f, 0.0f);   glVertex3d(400, 80, 125);
						glTexCoord2f(1.0f, 0.0f); glVertex3d(400, 80, 175);
						glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, 175);
						glTexCoord2f(0.0f, 1.0f);    glVertex3d(400, 30, 125);
						glEnd();
					}
					
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, texture[40]);
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);   glVertex3d(400, 80, 125);
					glTexCoord2f(1.0f, 0.0f); glVertex3d(400, 80, 175);
					glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, 175);
					glTexCoord2f(0.0f, 1.0f);    glVertex3d(400, 30, 125);
					glEnd();
				}
			}
			else
			{

				glBindTexture(GL_TEXTURE_2D, texture[16 + count1 / 4]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);   glVertex3d(400, 80, 125);
				glTexCoord2f(1.0f, 0.0f); glVertex3d(400, 80, 175);
				glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, 175);
				glTexCoord2f(0.0f, 1.0f);    glVertex3d(400, 30, 125);
				glEnd();
			}
		}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture[9]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 30, 125);
				glTexCoord2f(1.0f, 0.0f); glVertex3d(400, 80, 125);
				glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 80, 175);
				glTexCoord2f(0.0f, 1.0f);    glVertex3d(400, 30, 175);
				glEnd();

			}
		}
	
void wall()//墙壁
{/*门*/glBindTexture(GL_TEXTURE_2D, texture[13]);
if (!dooropen) {

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(200.0, 100.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 100.0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(200.0, 0.0, 0.0);
	glEnd();
}

glBegin(GL_QUADS);
glTexCoord2f(0.0f, 0.0f);  glVertex3f(400.0, 100.0, 0.0);
glTexCoord2f(1.0f, 0.0f); glVertex3f(200.0, 100.0, 0.0);
glTexCoord2f(1.0f, 1.0f); glVertex3f(200.0, 0.0, 0.0);
glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 0.0, 0.0);
glEnd();
	
	if (displayjade) {
		glBindTexture(GL_TEXTURE_2D, texture[9]);//换鞋区1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex3d(-400, 0, 0);
		glTexCoord2f(300.0f, 0.0f); glVertex3d(0, 0, 0);
		glTexCoord2f(180.0f, 300.0f);  glVertex3d(0, 200 / 2, 0);
		glTexCoord2f(0.0f, 300.0f);  glVertex3d(-400, 200 / 2, 0);
		glEnd();
	}
	else {
	
		mirror();

		glBindTexture(GL_TEXTURE_2D, texture[9]);//换鞋区1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-220.0, 00.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-220.0, 40.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-200.0, 40.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-200.0, 00.0, 0.0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[9]);//换鞋区1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-220.0, 60.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-220.0, 100.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-200.0, 100.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-200.0, 60.0, 0.0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[9]);//换鞋区1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-400.0, 100.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-220.0, 100.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-220.0, 0.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-400.0, 0.0, 0.0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[9]);//换鞋区1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-200.0, 100.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 100.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-200.0, 0.0, 0.0);
		glEnd();
	}

	
	
	
	if (thirdplay) {
		nengju();
		if (play3_5) {//显示八尺镜
			if (!mirrodisplay)
			{
				Jade();
			}
			else {
				glBindTexture(GL_TEXTURE_2D, texture[42]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(120.0, 80.0, -730.0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(170.0, 80.0, -730.0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(170.0, 30.0, -730.0);
				glTexCoord2f(0.0f, 1.0f);  glVertex3f(120.0, 30.0, -730.0);
				glEnd();
			}
		}
		else {
			glBindTexture(GL_TEXTURE_2D, texture[41]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(120.0, 80.0, -730.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(170.0, 80.0, -730.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(170.0, 30.0, -730.0);
			glTexCoord2f(0.0f, 1.0f);  glVertex3f(120.0, 30.0, -730.0);
			glEnd();
		}
			glBegin(GL_TRIANGLE_FAN);//墙3
			glVertex3d(600, 100 / 2 - 10, 0);
			glVertex3d(800, 0, 0);
			glVertex3d(800, 200 / 2 - 10, 0);
			glVertex3d(400, 200 / 2 - 10, 0);
			glVertex3d(400, 0, 0);
			glVertex3d(800, 0, 0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, texture[9]);//墙11
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 0, -730);
			glTexCoord2f(300.0f, 0.0f); glVertex3d(0, 200 / 2 - 10, -730);
			glTexCoord2f(180.0f, 300.0f); glVertex3d(120, 200 / 2 - 10, -730);
			glTexCoord2f(0.0f, 300.0f);  glVertex3d(120, 0, -730);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, texture[9]);//墙12
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);  glVertex3d(170, 0, -730);
			glTexCoord2f(300.0f, 0.0f); glVertex3d(170, 200 / 2 - 10, -730);
			glTexCoord2f(180.0f, 300.0f); glVertex3d(400, 200 / 2 - 10, -730);
			glTexCoord2f(0.0f, 300.0f);  glVertex3d(400, 0, -730);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, texture[9]);//下
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(120.0, 0.0, -730.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(120.0, 30.0, -730.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(170.0, 30.0, -730.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(170.0, 0.0, -730.0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, texture[9]);//上
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(120.0, 80.0, -730.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(120.0, 90.0, -730.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(170.0, 90.0, -730.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(170.0, 80.0, -730.0);
			glEnd();


		
	}
	if (firstplay) {
		glBegin(GL_TRIANGLE_FAN);//墙4(铅笔标注的)!!!!!!!!!!!
		glVertex3d(400, 100 / 2 - 10, -150);
		glVertex3d(400, 0, 0);
		glVertex3d(400, 200 / 2 - 10, 0);
		glVertex3d(400, 200 / 2 - 10, -400);
		glVertex3d(400, 0, -400);
		glVertex3d(400, 0, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[9]);//墙5
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 0, -450);
		glTexCoord2f(300.0f, 0.0f); glVertex3d(400, 200 / 2 - 10, -450);
		glTexCoord2f(180.0f, 300.0f); glVertex3d(400, 200 / 2 - 10, -730);
		glTexCoord2f(0.0f, 300.0f);  glVertex3d(400, 0, -730);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[29]);//提示
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 80, -450); 
		glTexCoord2f(1.0f, 0.0f);  glVertex3d(400, 80, -400);
		glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, -400);
		glTexCoord2f(0.0f, 1.0f);  glVertex3d(400, 30, -450);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[9]);//提示下
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 0, -450);
		glTexCoord2f(1.0f, 0.0f);  glVertex3d(400, 0, -400);
		glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, -400);
		glTexCoord2f(0.0f, 1.0f);  glVertex3d(400, 30, -450);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[9]);//提示上
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 80, -450);
		glTexCoord2f(1.0f, 0.0f);  glVertex3d(400, 80, -400);
		glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 100, -400);
		glTexCoord2f(0.0f, 1.0f);  glVertex3d(400, 100, -450);
		glEnd();
		if (showsmalldoor) {
			glBegin(GL_TRIANGLE_FAN);//墙2
			glVertex3d(0, 100 / 2 - 20, -375);
			glVertex3d(0, 0, -730);
			glVertex3d(-0, 200 / 2 - 10, -730);
			glVertex3d(0, 200 / 2 - 10, -30);
			glVertex3d(0, 0, -30);
			glVertex3d(0, 0, -730);
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLE_FAN);//墙2
			glVertex3d(0, 100 / 2 - 20, -375);
			glVertex3d(0, 0, -730);
			glVertex3d(-0, 200 / 2 - 10, -730);
			glVertex3d(0, 200 / 2 - 10, 0);
			glVertex3d(0, 0, 0);
			glVertex3d(0, 0, -730);
			glEnd();
		}
		
		glBindTexture(GL_TEXTURE_2D, texture[10]);//墙11
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex3d(130, 0, -730);
		glTexCoord2f(4.0, 0.0f); glVertex3d(130, 100, -730);
		glTexCoord2f(4.0f, 4.0f); glVertex3d(0, 100, -730);
		glTexCoord2f(0.0f, 4.0f); glVertex3d(0, 0, -730);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[10]);//墙12
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 0, -730);
		glTexCoord2f(4.0, 0.0f); glVertex3d(400, 100, -730);
		glTexCoord2f(4.0f, 4.0f); glVertex3d(260, 100, -730);
		glTexCoord2f(0.0f, 4.0f); glVertex3d(260, 0, -730);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, texture[9]);//墙12 11下
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex3d(260, 0, -730);
		glTexCoord2f(2.0, 0.0f); glVertex3d(260, 20, -730);
		glTexCoord2f(2.0f, 1.0f); glVertex3d(130, 20, -730);
		glTexCoord2f(0.0f, 1.0f); glVertex3d(130, 0, -730);
		glEnd();

	}
	if(!finalplay){
	if (showsmalldoor)
	{
		glBegin(GL_TRIANGLE_FAN);//墙4(铅笔标注的)!!!!!!!!!!!
		glVertex3d(400, 100 / 2 - 10, -150);
		glVertex3d(400, 0, 0);
		glVertex3d(400, 200 / 2 - 10, 0);
		glVertex3d(400, 200 / 2 - 10, -400);
		glVertex3d(400, 0, -400);
		glVertex3d(400, 0, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[9]);//墙5
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 0, -450);
		glTexCoord2f(300.0f, 0.0f); glVertex3d(400, 200 / 2 - 10, -450);
		glTexCoord2f(180.0f, 300.0f); glVertex3d(400, 200 / 2 - 10, -730);
		glTexCoord2f(0.0f, 300.0f);  glVertex3d(400, 0, -730);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[29]);//提示
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 80, -450);
		glTexCoord2f(1.0f, 0.0f);  glVertex3d(400, 80, -400);
		glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, -400);
		glTexCoord2f(0.0f, 1.0f);  glVertex3d(400, 30, -450);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[9]);//提示下
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 0, -450);
		glTexCoord2f(1.0f, 0.0f);  glVertex3d(400, 0, -400);
		glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 30, -400);
		glTexCoord2f(0.0f, 1.0f);  glVertex3d(400, 30, -450);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, texture[9]);//提示上
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 80, -450);
		glTexCoord2f(1.0f, 0.0f);  glVertex3d(400, 80, -400);
		glTexCoord2f(1.0f, 1.0f);  glVertex3d(400, 100, -400);
		glTexCoord2f(0.0f, 1.0f);  glVertex3d(400, 100, -450);
		glEnd();
	}
	}
	glBegin(GL_TRIANGLE_FAN);//墙6.3
	glVertex3d(400, 100/2-10, 400);
	glVertex3d(400, 0, 730);
	glVertex3d(400, 200/2-10, 730);
	glVertex3d(400, 200/2-10, 380);
	glVertex3d(400, 0, 380);
	glVertex3d(400, 0, 730);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);//墙6.2
	glVertex3d(400, 100 / 2 - 10, 175);
	glVertex3d(400, 0, 310);
	glVertex3d(400, 200 / 2 - 10, 310);
	glVertex3d(400, 200 / 2 - 10, 175);
	glVertex3d(400, 0, 175);
	glVertex3d(400, 0, 310);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);//墙6.1
	glVertex3d(400, 100 / 2 - 10, 0);
	glVertex3d(400, 0, 125);
	glVertex3d(400, 200 / 2 - 10, 125);
	glVertex3d(400, 200 / 2 - 10, 0);
	glVertex3d(400, 0, 0);
	glVertex3d(400, 0, 125);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);//墙6.4
	glVertex3d(400, 20, 0);
	glVertex3d(400, 0, 125);
	glVertex3d(400, 30, 125);
	glVertex3d(400, 30, 175);
	glVertex3d(400, 0, 175);
	glVertex3d(400, 0, 125);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);//墙6.5
	glVertex3d(400, 90, 0);
	glVertex3d(400, 80, 125);
	glVertex3d(400, 90, 125);
	glVertex3d(400, 90, 175);
	glVertex3d(400, 80, 175);
	glVertex3d(400, 80, 125);
	glEnd();


	glBegin(GL_TRIANGLE_FAN);//墙7
	glVertex3d(600, 100/2, 730);
	glVertex3d(800, 0, 730);
	glVertex3d(800, 200/2-10, 730);
	glVertex3d(400, 200/2-10, 730);
	glVertex3d(400, 0, 730);
	glVertex3d(800, 0, 730);
	glEnd();



	glBegin(GL_TRIANGLE_FAN);//墙9
	glVertex3d(360, 20, 730);
	glVertex3d(400, 0, 730);
	glVertex3d(400, 20, 730);
	glVertex3d(350, 20, 730);
	glVertex3d(350, 0, 730);
	glVertex3d(400, 0, 730);
	glEnd();
	

	glBegin(GL_TRIANGLE_FAN);//墙10
	glVertex3d(360, 90, 730);
	glVertex3d(400, 90, 730);
	glVertex3d(400, 100-10, 730);
	glVertex3d(350, 100-10, 730);
	glVertex3d(350, 90, 730);
	glVertex3d(400, 90, 730);
	glEnd();

	

	

	

	glBegin(GL_TRIANGLE_FAN);//顶2
	glVertex3d(10, 100, -500);
	glVertex3d(400, 100, 0);
	glVertex3d(400, 100, -730);
	glVertex3d(0, 100, -730);
	glVertex3d(0, 100, 0);
	glVertex3d(400, 100, 0);
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);//顶1 ↓
	glVertex3d(0, 100, 650);
	glVertex3d(400, 100, 730);
	glVertex3d(400, 100, 600);
	glVertex3d(-20, 100, 600);
	glVertex3d(-20, 100, 730);
	glVertex3d(400, 100, 730);
	glEnd();

	

	
	glBegin(GL_TRIANGLE_FAN);//顶1 ↑
	glVertex3d(100, 100, 50);
	glVertex3d(400, 100, 100);
	glVertex3d(400, 100, 0);
	glVertex3d(-20, 100, 0);
	glVertex3d(-20, 100, 100);
	glVertex3d(400, 100, 100);
	glEnd();

	
	
	glBegin(GL_TRIANGLE_FAN);//顶1 ←
	glVertex3d(0, 100, 730);
	glVertex3d(-20, 100, 500);
	glVertex3d(150, 100, 600);
	glVertex3d(150, 100, 100);
	glVertex3d(-20, 100, 100);
	glVertex3d(-20, 100, 730);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[10]);//横梁1下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 90, 730);
	glTexCoord2f(300.0f, 0.0f); glVertex3d(400, 100 , 730);
	glTexCoord2f(180.0f, 300.0f); glVertex3d(-20,100, 730);
	glTexCoord2f(0.0f, 300.0f);  glVertex3d(-20, 90, 730);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[10]);//横梁1you
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 90, 730);
	glTexCoord2f(300.0f, 0.0f); glVertex3d(400, 100, 730);
	glTexCoord2f(180.0f, 300.0f); glVertex3d(400, 100, -730);
	glTexCoord2f(0.0f, 300.0f);  glVertex3d(400, 90, -730);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[10]);//横梁2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -730);
	glTexCoord2f(300.0f, 0.0f); glVertex3d(0, 100, -730);
	glTexCoord2f(180.0f, 300.0f); glVertex3d(0, 100, -20);
	glTexCoord2f(0.0f, 300.0f);  glVertex3d(0, 90, -20);
	glEnd();/**/
	
	
	glBindTexture(GL_TEXTURE_2D, texture[16]);//浮世绘
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(400, 90, 380);
	glTexCoord2f(1.0, 0.0f);  glVertex3d(400, 90, 310); 
	glTexCoord2f(1.0f, 1.0f); glVertex3d(400, 0, 310);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(400, 0, 380); 
	glEnd();
	
	if (finalplay)
	{
				glBindTexture(GL_TEXTURE_2D, texture[10]);//墙11
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(130, 0, -730);
				glTexCoord2f(4.0, 0.0f); glVertex3d(130, 100, -730);
				glTexCoord2f(4.0f, 4.0f); glVertex3d(0, 100, -730);
				glTexCoord2f(0.0f, 4.0f); glVertex3d(0, 0, -730);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[10]);//墙12
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 0, -730);
				glTexCoord2f(4.0, 0.0f); glVertex3d(400, 100, -730);
				glTexCoord2f(4.0f, 4.0f); glVertex3d(260, 100, -730);
				glTexCoord2f(0.0f, 4.0f); glVertex3d(260, 0, -730);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[9]);//墙12 11下
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(260, 0, -730);
				glTexCoord2f(2.0, 0.0f); glVertex3d(260, 20, -730);
				glTexCoord2f(2.0f, 1.0f); glVertex3d(130, 20, -730);
				glTexCoord2f(0.0f, 1.0f); glVertex3d(130, 0, -730);
				glEnd();

				carboard2();
				carboard3();
				bed();


				/*左*/
				glBindTexture(GL_TEXTURE_2D, texture[9]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -730);
				glTexCoord2f(4.0, 0.0f); glVertex3d(0, 90, -440);
				glTexCoord2f(4.0f, 4.0f); glVertex3d(0, 0, -440);
				glTexCoord2f(0.0f, 4.0f); glVertex3d(0, 0, -730);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[9]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -380);
				glTexCoord2f(4.0, 0.0f); glVertex3d(0, 90, -320);
				glTexCoord2f(4.0f, 4.0f); glVertex3d(0, 0, -320);
				glTexCoord2f(0.0f, 4.0f); glVertex3d(0, 0, -380);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[9]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -240);
				glTexCoord2f(4.0, 0.0f); glVertex3d(0, 90, -180);
				glTexCoord2f(4.0f, 4.0f); glVertex3d(0, 0, -180);
				glTexCoord2f(0.0f, 4.0f); glVertex3d(0, 0, -240);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[9]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -120);
				glTexCoord2f(4.0, 0.0f); glVertex3d(0, 90, -100);
				glTexCoord2f(4.0f, 4.0f); glVertex3d(0, 0, -100);
				glTexCoord2f(0.0f, 4.0f); glVertex3d(0, 0, -120);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[45]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -440);
				glTexCoord2f(1.0, 0.0f); glVertex3d(0, 90, -380);
				glTexCoord2f(1.0f, 1.0f); glVertex3d(0, 0, -380);
				glTexCoord2f(0.0f, 1.0f); glVertex3d(0, 0, -440);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[46]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -320);
				glTexCoord2f(1.0, 0.0f); glVertex3d(0, 90, -240);
				glTexCoord2f(1.0f, 1.0f); glVertex3d(0, 0, -240);
				glTexCoord2f(0.0f, 1.0f); glVertex3d(0, 0, -320);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[47]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(0, 90, -180);
				glTexCoord2f(1.0, 0.0f); glVertex3d(0, 90, -120);
				glTexCoord2f(1.0f, 1.0f); glVertex3d(0, 0, -120);
				glTexCoord2f(0.0f, 1.0f); glVertex3d(0, 0, -180);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture[9]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);  glVertex3d(400, 90, -730);
				glTexCoord2f(1.0, 0.0f); glVertex3d(400, 90, 0);
				glTexCoord2f(1.0f, 1.0f); glVertex3d(400, 0, 0);
				glTexCoord2f(0.0f, 1.0f); glVertex3d(400, 0, -730);
				glEnd();
	}

	
}
//分形竹子
void leaf1(float x0, float y0, float z0, float x1, float y1, float z1, float angle1, float angle2, int n)
{
	float xx1, yy1, zz1;
	float  ll, ang1, ang2;
	int xs, ys, zs, xe, ye, ze;
	if (n == 0)
	{

		xs = (int)(x0 + 0.5);
		ys = (int)(y0 + 0.5);
		zs = (int)(z0 + 0.5);
		xe = (int)(x1 + 0.5);
		ye = (int)(y1 + 0.5);
		ze = (int)(z1 + 0.5);


		glVertex3d(xs, ys, zs);
		glVertex3d(xe, ye, ze);

		//Sleep(100);
	}
	else {
		ll = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0) + (z1 - z0)*(z1 - z0)) / 3;
		xs = (int)(x0 + 0.5);
		ys = (int)(y0 + 0.5);
		zs = (int)(z0 + 0.5);
		xe = (int)(x1 + 0.5);
		ye = (int)(y1 + 0.5);
		ze = (int)(z1 + 0.5);
		if (n <= 2)
		{
			glLineWidth(1);
		}
		else
		{
			glLineWidth(1);
		}
		leaf1(xs, ys, zs, xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, angle1, angle2, 0);
		xx1 = xe + ll * cos(angle1)*cos(angle2);
		yy1 = ye + ll * sin(angle2);
		zz1 = ze + ll * cos(angle2)*sin(angle1);
		ang1 = angle1;
		ang2 = angle2 + AF1;
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);

		xx1 = xs + (xe - xs) * 2 / 3 + ll * cos(ang1)*cos(angle2);
		yy1 = ys + (ye - ys) * 2 / 3 + ll * sin(angle2);
		zz1 = zs + (ze - zs) * 2 / 3 + ll * cos(angle2)*sin(ang1);
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);

		ang1 = angle1 + AF2;
		xx1 = xe + ll * cos(ang1)*cos(angle2);
		yy1 = ye + ll * sin(angle2);
		zz1 = ze + ll * cos(angle2)*sin(ang1);
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);

		xx1 = xs + (xe - xs) * 2 / 3 + ll * cos(angle1)*cos(angle2);
		yy1 = ys + (ye - ys) * 2 / 3 + ll * sin(angle2);
		zz1 = zs + (ze - zs) * 2 / 3 + ll * cos(angle2)*sin(angle1);
		ang1 = angle1;
		ang2 = angle2 + AF1;
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);
		ang1 = angle1 + AF2 * 2;
		xx1 = xe + ll * cos(angle1)*cos(angle2);
		yy1 = ye + ll * sin(angle2);
		zz1 = ze + ll * cos(angle2)*sin(angle1);;
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);

		//leaf2(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);
		ang1 = angle1 + AF2 * 3;
		xx1 = xe + ll * cos(ang1)*cos(angle2);
		yy1 = ye + ll * sin(angle2);
		zz1 = ze + ll * cos(angle2)*sin(ang1);;
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);
	}
}
void leaf2(float x0, float y0, float z0, float x1, float y1, float z1, float angle1, float angle2, int n)
{
	float xx1, yy1, zz1;
	float  ll, ang1, ang2;
	int xs, ys, zs, xe, ye, ze;
	if (n == 0)
	{

		xs = (int)(x0 + 0.5);
		ys = (int)(y0 + 0.5);
		zs = (int)(z0 + 0.5);
		xe = (int)(x1 + 0.5);
		ye = (int)(y1 + 0.5);
		ze = (int)(z1 + 0.5);


		glVertex3d(xs, ys, zs);
		glVertex3d(xe, ye, ze);

		//Sleep(100);
	}
	else {
		ll = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0) + (z1 - z0)*(z1 - z0)) / 3;
		xs = (int)(x0 + 0.5);
		ys = (int)(y0 + 0.5);
		zs = (int)(z0 + 0.5);
		xe = (int)(x1 + 0.5);
		ye = (int)(y1 + 0.5);
		ze = (int)(z1 + 0.5);
		if (n <= 2)
		{
			glLineWidth(3);
		}
		else
		{
			glLineWidth(1);
		}
		leaf1(xs, ys, zs, xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, angle1, angle2, 0);
		xx1 = xe + ll * cos(angle1)*cos(angle2);
		yy1 = ye + ll * sin(angle2);
		zz1 = ze + ll * cos(angle2)*sin(angle1);
		ang1 = angle1;
		ang2 = angle2 + AF1 / 2;
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);
		xx1 = xe + ll * cos(angle1)*cos(angle2);
		yy1 = ye + ll * sin(angle2);
		zz1 = ze + ll * cos(angle2)*sin(angle1);
		ang1 = angle1;
		ang2 = angle2 + AF1;
		leaf1(xs + (xe - xs) * 2 / 3, ys + (ye - ys) * 2 / 3, zs + (ze - zs) * 2 / 3, xx1, yy1, zz1, ang1, ang2, n - 1);

	}
}

void sword()//草雉剑
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[23]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-600.0, 100.0, 180.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-700.0, 100.0, 100.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-700.0, 110.0, 100.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-600.0, 110.0, 180.0);
	glEnd();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void mirror()//八尺琼曲玉
{
	glBindTexture(GL_TEXTURE_2D, texture[24]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-220.0, 40.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-220.0, 60.0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-200.0, 60.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-200.0, 40.0, 0.0);
	glEnd();
}
void Jade()//八咫镜
{
	glBindTexture(GL_TEXTURE_2D, texture[22]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(120.0, 30.0, -730.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(120.0, 80.0, -730.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(170.0, 80.0, -730.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(170.0, 30.0, -730.0);
	glEnd();
}
void nengju()//能剧面具
{
	
	glBindTexture(GL_TEXTURE_2D, texture[9]);//1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 90.0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 90.0, -200.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 0.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//12下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 0.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 60.0, -220.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 60.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//12上
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 80.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 80.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 100.0, -220.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 100.0, -200.0);
	glEnd();
	if (right1) {
		glBindTexture(GL_TEXTURE_2D, texture[39]);//12
	}
	else {
		if (fr1) {
			glBindTexture(GL_TEXTURE_2D, texture[26]);//12
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[15]);
		}
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 80.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 80.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 60.0, -220.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 60.0, -200.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[9]);//2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, -220.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 90.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 90.0, -400.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 0.0, -400.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//23下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, -400.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 0.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 60.0, -420.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 60.0, -400.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//23上
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 80.0, -400.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 80.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 100.0, -420.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 100.0, -400.0);
	glEnd();
	if (right2) {
		glBindTexture(GL_TEXTURE_2D, texture[39]);
	}
	else {
		if (fr2) {
			glBindTexture(GL_TEXTURE_2D, texture[25]);//23
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[15]);
		}
	}
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 80.0, -400.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 80.0, -420.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 60.0, -420.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 60.0, -400.0);
		glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texture[9]);//3
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, -420.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 90.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 90.0, -600.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 0.0, -600.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//34下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, -600.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 0.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 60.0, -620.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 60.0, -600.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//34上
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 80.0, -600.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 80.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 100.0, -620.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 100.0, -600.0);
	glEnd();
	if (right3) {
		glBindTexture(GL_TEXTURE_2D, texture[39]);
	}
	else {
		if (fr3) {
			glBindTexture(GL_TEXTURE_2D, texture[27]);//34
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[15]);
		}
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 80.0, -600.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 80.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 60.0, -620.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 60.0, -600.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(400.0, 0.0, -620.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(400.0, 90.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 90.0, -730.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(400.0, 0.0, -730.0);
	glEnd();
	//左边
	if (!showsmalldoor) {
		glBindTexture(GL_TEXTURE_2D, texture[9]);//1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 90.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 90.0, -200.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, -200.0);
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[9]);//1
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -100.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 90.0, -100.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 90.0, -200.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, -200.0);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, texture[9]);//12下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 0.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 60.0, -220.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 60.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//12上
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 80.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 80.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 100.0, -220.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 100.0, -200.0);
	glEnd();
	if (right2) { glBindTexture(GL_TEXTURE_2D, texture[39]); }
	else {
		if (fl1) {
			glBindTexture(GL_TEXTURE_2D, texture[25]);//12
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[15]);
		}
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 80.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 80.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 60.0, -220.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 60.0, -200.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[9]);//2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -220.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 90.0, -220.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 90.0, -400.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, -400.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//23下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -400.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 0.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 60.0, -420.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 60.0, -400.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//23上
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 80.0, -400.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 80.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 100.0, -420.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 100.0, -400.0);
	glEnd();
	if (right1) {
		glBindTexture(GL_TEXTURE_2D, texture[39]);
	}
	else {


		if (fl2) {
			glBindTexture(GL_TEXTURE_2D, texture[26]);//23
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[15]);
		}
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 80.0, -400.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 80.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 60.0, -420.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 60.0, -400.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//3
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -420.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 90.0, -420.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 90.0, -600.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, -600.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//34下
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -600.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 0.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 60.0, -620.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 60.0, -600.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//34上
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 80.0, -600.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 80.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 100.0, -620.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 100.0, -600.0);
	glEnd();
	if (right3) {
		glBindTexture(GL_TEXTURE_2D, texture[39]);
	}
	else {
		if (fl3) {
			glBindTexture(GL_TEXTURE_2D, texture[27]);//34
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture[15]);
		}
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 80.0, -600.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 80.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 60.0, -620.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 60.0, -600.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, -620.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 90.0, -620.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 90.0, -730.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, -730.0);
	glEnd();
}
void table()
{
	//下
	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-240.0, 0.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-240.0, 20.0, -200.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0, 20.0, -200.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0, 0.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-165.0, 0.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-165.0, 20.0, -200.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-160.0, 20.0, -200.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-160.0, 0.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0, 5.0, -200.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0, 20.0, -200.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-165.0, 20.0, -200.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-165.0, 5.0, -200.0);
	glEnd();
	//左
	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0, 0.0, -205.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0, 20.0, -205.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0, 20.0, -200.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0, 0.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0, 5.0, -285.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0, 20.0, -285.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0, 20.0, -205.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0, 5.0, -205.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0, 0.0, -290.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0, 20.0, -290.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0, 20.0, -285.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0, 0.0, -285.0);
	glEnd();

	//右

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0+85, 0.0, -205.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0+85, 20.0, -205.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0+85, 20.0, -200.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0+85, 0.0, -200.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0+85, 5.0, -285.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0+85, 20.0, -285.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0+85, 20.0, -205.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0+85, 5.0, -205.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0+85, 0.0, -290.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0+85, 20.0, -290.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0+85, 20.0, -285.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0+85, 0.0, -285.0);
	glEnd();
	//上
	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-240.0, 0.0, -200.0-85);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-240.0, 20.0, -200.0-85);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-245.0, 20.0, -200.0-85);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-245.0, 0.0, -200.0-85);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-165.0, 0.0, -200.0-85);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-165.0, 20.0, -200.0-85);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-160.0, 20.0, -200.0-85);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-160.0, 0.0, -200.0-85);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[30]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-245.0, 5.0, -200.0-85);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-245.0, 20.0, -200.0-85);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-165.0, 20.0, -200.0-85);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-165.0, 5.0, -200.0-85);
	glEnd();

	//桌面
	glBindTexture(GL_TEXTURE_2D, texture[32]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-255.0, 20.0, -190.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-255.0, 20.0, -295.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-150.0, 20.0, -295.0);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-150.0, 20.0, -190.0);
	glEnd();

	//椅子xia
	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 0.0, -155.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 10.0, -155.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-195.0, 10.0, -155.0);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-195.0, 0.0, -155.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 0.0, -155.0-10);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 10.0, -155.0-10);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-195.0, 10.0, -155.0-10);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-195.0, 0.0, -155.0-10);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 10.0, -155.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 0.0, -155.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-205.0, 0.0, -165.0);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-205.0, 10.0, -165.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0+10, 10.0, -155.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0+10, 0.0, -155.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-205.0+10, 0.0, -165.0);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-205.0+10, 10.0, -165.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0 , 10.0, -165.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0 , 10.0, -155.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-205.0 + 10, 10.0, -155.0);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-205.0 + 10, 10.0, -165.0);
	glEnd();

	//椅子上
	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 0.0, -155.0-165);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 10.0, -155.0-165);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-195.0, 10.0, -155.0-165);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-195.0, 0.0, -155.0-165);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 0.0, -155.0 - 10-165);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 10.0, -155.0 - 10-165);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-195.0, 10.0, -155.0 - 10-165);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-195.0, 0.0, -155.0 - 10-165);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 10.0, -155.0-165);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 0.0, -155.0-165);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-205.0, 0.0, -165.0-165);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-205.0, 10.0, -165.0-165);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0 + 10, 10.0, -155.0-165);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0 + 10, 0.0, -155.0-165);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-205.0 + 10, 0.0, -165.0-165);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-205.0 + 10, 10.0, -165.0-165);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[31]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-205.0, 10.0, -165.0-165);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-205.0, 10.0, -155.0-165);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-205.0 + 10, 10.0, -155.0-165);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-205.0 + 10, 10.0, -165.0-165);
	glEnd();
}
void lantern(int x,int z)
{
	glBindTexture(GL_TEXTURE_2D, texture[33]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0+x, 13.0-10, 5.0+z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(5.0+x, 13.0-10, 5.0+z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(5.0+x, 0.0-10, 5.0+z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-5.0+x, 0.0-10, 5.0+z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[33]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0+x, 13.0-10, -5.0+z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(5.0+x, 13.0-10, -5.0+z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(5.0+x, 0.0-10, -5.0+z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-5.0+x, 0.0-10, -5.0+z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[33]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0+x, 13.0-10, -5.0+z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-5.0+x, 13.0-10, 5.0+z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-5.0+x, 0.-10, 5.0+z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-5.0+x, 0.0-10, -5.0+z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[33]);//4
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0+x, 13.0-10, -5.0+z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(5.0+x, 13.0-10, 5.0+z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(5.0+x, 0.0-10, 5.0+z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(5.0+x, 0.0-10, -5.0+z);
	glEnd();
}
void bed()
{
	glBindTexture(GL_TEXTURE_2D, texture[34]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0,25.0,-550.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(260.0, 25.0, -550.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(260.0, 0.0, -550.0);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(130.0, 0.0, -550.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[34]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0, 25.0, -730.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(130.0, 25.0, -550.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(130.0, 0.0, -550.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(130.0, 0.0, -730.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[34]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0+130, 25.0, -730.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(130.0+130, 25.0, -550.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(130.0+130, 0.0, -550.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(130.0+130, 0.0, -730.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[35]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(130.0 , 25.0, -730.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(260.0 ,25.0, -730.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(260.0 , 25.0, -550.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(130.0, 25.0, -550.0);
	glEnd();
}
void carboard2()
{
	glBindTexture(GL_TEXTURE_2D, texture[36]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -490.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(330.0, 25.0, -290.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(330.0, 0.0, -290.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 0.0, -490.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[43]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -290.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(400.0, 25.0, -290.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 0.0, -290.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 0.0, -290.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[43]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -290.0-200);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(400.0, 25.0, -290.0-200);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 0.0, -290.0-200);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 0.0, -290.0-200);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[43]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -290.0 - 200);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(400.0, 25.0, -290.0 - 200);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(400.0, 25.0, -290.0 );
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 25.0, -290.0 );
	glEnd();
}
void carboard3()
{
	glBindTexture(GL_TEXTURE_2D, texture[37]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -270.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(330.0, 25.0, -170.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(330.0, 0.0, -170.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 0.0, -270.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[38]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -170.0);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(400.0, 25.0, -170.0);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(400.0, 0.0, -170.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 0.0, -170.0);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, texture[38]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -170.0-100);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(400.0, 25.0, -170.0-100);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(400.0, 0.0, -170.0-100);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 0.0, -170.0-100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[38]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(330.0, 25.0, -170.0 - 100);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(400.0, 25.0, -170.0 - 100);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(400.0, 25.0, -170.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(330.0, 25.0, -170.0 );
	glEnd();
}


inline float randf()
{
	return (rand() / (float)RAND_MAX);
}

void FractalTree(int level)
{
	long savedseed;

	if (level == Level)
	{
		glPushMatrix();
		glRotatef(60 + randf() * 120, 0, 1, 0);
		glCallList(STEMANDLEAVES);
		glPopMatrix();
	}
	else
	{
		glCallList(STEM);

		glPushMatrix();
		glTranslatef(0, 1, 0);
		glScalef(0.7, 0.7, 0.7);

		savedseed = rand();
		glPushMatrix();
		glRotatef(0 + randf() * 90, 0, 1, 0);
		glRotatef(30 + randf() * 30, 0, 0, 1);
		FractalTree(level + 1);
		glPopMatrix();

		srand(savedseed);
		savedseed = rand();
		glPushMatrix();
		glRotatef(180 + randf() * 90, 0, 1, 0);
		glRotatef(30 + randf() * 30, 0, 0, 1);
		FractalTree(level + 1);
		glPopMatrix();

		srand(savedseed);
		savedseed = rand();
		glPushMatrix();
		glRotatef(90 + randf() * 90, 0, 1, 0);
		glRotatef(30 + randf() * 30, 0, 0, 1);
		FractalTree(level + 1);
		glPopMatrix();

		glPopMatrix();
	}
}

void CreateTreeLists(void)
{
	GLUquadricObj *cylquad = gluNewQuadric();
	int i;

	glNewList(STEM, GL_COMPILE);
	glCallList(TREE_MAT);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluCylinder(cylquad, 0.1, 0.08, 1, 10, 2);
	glPopMatrix();
	glEndList();

	glNewList(LEAF, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.1, 0, 0.25);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5, 0.5, 0.2);
	glVertex3f(0, 1.0, 0);
	glNormal3f(0.2, 0, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1.0, 0);
	glVertex3f(-0.5, 0.5, 0.2);
	glEnd();
	glEndList();

	glNewList(STEMANDLEAVES, GL_COMPILE);
	glPushMatrix();
	glPushAttrib(GL_LIGHTING_BIT);
	//glColor3f(1.0, 0.0, 0.0);
	//glColor3ub(66, 35, 36);
	glCallList(STEM);
	//glColor3ub(66, 35, 36);
	glCallList(LEAF_MAT);
	for (i = 0; i < 3; i++)
	{
		glTranslatef(0, 0.333, 0);
		glRotatef(90, 0, 1, 0);
		glPushMatrix();
		glRotatef(0, 0, 1, 0);
		glRotatef(50, 1, 0, 0);
		glColor3ub(233, 200, 227);

		glCallList(LEAF);
		glPopMatrix();
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glRotatef(60, 1, 0, 0);
		glColor3ub(66, 35, 36);
		glCallList(LEAF);
		glPopMatrix();
	}
	glPopAttrib();
	glPopMatrix();
	glEndList();

	gluDeleteQuadric(cylquad);
}

void SetupMaterials(void)
{
	GLfloat tree_ambuse[] = { 0.4, 0.25, 0.1, 1.0 };
	GLfloat tree_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat tree_shininess[] = { 10 };

	GLfloat leaf_ambuse[] = { 0.0, 0.8, 0.0, 1.0 };
	GLfloat leaf_specular[] = { 0.0, 0.8, 0.0, 1.0 };
	GLfloat leaf_shininess[] = { 10 };

	glNewList(TREE_MAT, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tree_ambuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tree_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, tree_shininess);
	glEndList();

	glNewList(LEAF_MAT, GL_COMPILE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, leaf_ambuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leaf_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, leaf_shininess);
	glEndList();
}
