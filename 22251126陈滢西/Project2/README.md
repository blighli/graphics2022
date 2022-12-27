# 最终实现内容如下
 *  **必选特性：✔️**
 * *  光照，太阳为光源✔️
 * *  纹理，使用图片进行纹理映射✔️
 *  **可选特性✔️**
 * *  使用顶点着色器和片段着色器，自己实现光照效果✔️
 *  * 鼠标选择，点击不同球体显示不同名称✔️
 
点击查看发布在bilibili上的[程序视频](
https://www.bilibili.com/video/BV1gA411R78D/?vd_source=881f21b11ef8e2832f37c7c84736a66c)❤️

---
# 解决方案

 ## 光照，太阳为光源
 
 * 以x,y平面对一条正弦函数进行绘制
 * 正弦函数随着时间进行位移，以实现动态水波的效果
 * 以y轴正方向为旋转轴，向四面八方绘制水波正弦函数
 
```cs
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

		}glLoadIdentity();
```

 ## 纹理，使用图片进行纹理映射
茶壶倒下水柱的颜色与当前帧的序号有关
```cs
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
```
 ## 使用顶点着色器和片段着色器，自己实现光照效果
 
 * 设置光照与材质
```cs
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
```
 * 计算与调整各种位置参数以保证效果
 
 ## 鼠标选择，点击不同球体显示不同名称
 
 **按键控制**
 
 * 'q','e'：控制相机左旋右旋
 * 'w','a','s','d'：控制相机前进后退左右移动
 * '1','2'：控制相机上下移动
 
 **实现思路**
 
 按键触发后，对gluLookAt函数的前六个参数进行更改。具体见代码中的**orientMe**，**move_Front_Back**，**move_Left_Right**，**move_High_Low**函数。
 
 ## bgm播放

* 将音频文件转化为wab格式，并放入vs解决方案目录
* 导入复制文件ID
* 使用以下代码完成
 ```cs
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
PlaySound(LPWSTR(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
 ```
 ## glut+vs环境搭建

* 下载glut的dll,lib,.h文件
* 将压缩包内的glut.h放到...\Microsoft Visual Studio\VC98\Include\GL目录下
* 将glut32.lib放到...\Microsoft Visual Studio\VC98\Lib目录下
* 将glut32.dll放到X:\windows\systom32目录下（win98用户放到X:\windows\systom目录下）

---
# 茶壶倒茶动画效果 ❤️：
[外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-UbZ2rIjz-1671460868189)(https://github.com/seimeicyx/graphics2022/blob/main/22251126%E9%99%88%E6%BB%A2%E8%A5%BF/Project1/result/still.gif)]
