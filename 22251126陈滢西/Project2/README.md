球体生成
https://blog.csdn.net/aoxuestudy/article/details/122423637?spm=1001.2101.3001.6650.9&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-10-122423637-blog-85012774.pc_relevant_3mothn_strategy_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-10-122423637-blog-85012774.pc_relevant_3mothn_strategy_recovery&utm_relevant_index=18
# 最终实现内容如下
 *  **必选特性：✔️**
 * *  光照，太阳为光源✔️
 * *  纹理，使用图片进行纹理映射✔️
 *  **可选特性✔️**
 * *  使用顶点着色器和片段着色器，自己实现光照效果✔️
 *  * 鼠标选择，点击不同球体显示不同名称✔️
 
点击查看发布在bilibili上的[程序视频](
https://www.bilibili.com/video/BV15K411i78i/)❤️

---
# 解决方案

 ## 光照，太阳为光源
 
由于实现了作业的可选部分**自己使用顶点着色器和片段着色器来实现光照效果**，因此这里以太阳为光源的实现逻辑如下
* 将光源的位置设置成太阳的位置。
* 设置环境反射系数
* 设置漫反射系数
具体实现即如下代码。
 
```cs
GLint lightPosLoc = glGetUniformLocation(renderingProgram, "light.position");
glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
GLint lightAmbLoc = glGetUniformLocation(renderingProgram, "light.ambient");
glUniform3f(lightAmbLoc, 0.1f, 0.1f, 0.1f);
GLint lightDiffuseLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
glUniform3f(lightDiffuseLoc, 0.7f, 0.7f, 0.7f);
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
 
 光照效果的实现使用的是Phong的反射模型，在Phong的反射模型中包含三个反射部分：环境反射、漫反射、镜面反射。
 
  ![image](https://user-images.githubusercontent.com/44937001/209655350-f651d690-7ea4-4701-ba63-4bcaaccd902c.png)
  
  因为本次作业内容为天体的光照，所以不考虑镜面反射，只实现环境光与漫反射。
  
 着色器代码如下：
 
 **顶点着色器**
 
```cs
#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2 ) in vec3 normal;
out vec2 tc;
out vec3 normalIn;
out vec3 fragPosIn;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 model;             //模型矩阵

layout (binding=0) uniform sampler2D s;

void main(void)
{	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	tc = tex_coord;
    fragPosIn = vec3(model * vec4(position, 1.0f));
    normalIn = mat3(transpose(inverse(model))) * normal;

}

```

**片元着色器**

```cs
#version 430

in vec2 tc;
in vec3 fragPosIn;
in vec3 normalIn;
out vec4 color;
struct Material
{
    sampler2D diffuse;      
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D s;

void main(void)
{	
    //环境光
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, tc));
    //漫反射光
    vec3 norm = normalize(normalIn);
    vec3 lightDir = normalize(light.position - fragPosIn);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, tc)));
    color = vec4(diffuse+ambient,1.0f);
}

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
