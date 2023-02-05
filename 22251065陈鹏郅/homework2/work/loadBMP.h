#pragma once

#define BITMAP_ID 0x4D42
#define PI 3.1415926

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>

//---------- �������ݽṹ
typedef struct {
	int width;				//������
	int height;				//����߶�
	unsigned int texID;		//������� ID
	unsigned char * data;	//ʵ����������
}texture;

//������ת�ٶ�
static float year = 0, month = 0, day = 0, angle = 15;
static bool first = false;


//---------- ����λͼ��Ϊ��������
unsigned char * LoadBmpFile(char * filename, BITMAPINFOHEADER * bmpInfoHeader);

//---------- ���������ļ�
texture * LoadTexFile(char * filename);

//---------- ��ʼ�������������ݺ�����
BOOL LoadAllTextures();
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);

void drawCircle(GLfloat r);