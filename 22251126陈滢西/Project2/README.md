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
* 向着色器传递当前渲染的物体是否是太阳的信息
* 如果是太阳就只进行纹理贴图（这样看起来就是发光的效果），如果是月球和地球就进行正常的phong光照模型渲染。

具体实现即如下代码。
 
```cs
GLint lightPosLoc = glGetUniformLocation(renderingProgram, "light.position");
glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);
GLuint isSun= glGetUniformLocation(renderingProgram, "isSun");
glUniform1f(isSun, 1.0f);
```

 ## 纹理，使用图片进行纹理映射
step1:根据球体的公式生成球体的顶点，获得球体的顶点坐标、法线、纹理，并三个顶点为一组（三角形）地返回，见Sphere类
step2:使用soil库读入图像，见Utils类
step3:生成纹理
step4:应用纹理

 ## 使用顶点着色器和片段着色器，自己实现光照效果
 
 光照效果的实现使用的是Phong的反射模型，在Phong的反射模型中包含三个反射部分：环境反射、漫反射、镜面反射。
 
  ![image](https://user-images.githubusercontent.com/44937001/209655350-f651d690-7ea4-4701-ba63-4bcaaccd902c.png)
  
  因为本次作业内容为天体的光照，所以不考虑镜面反射，只实现环境光与漫反射。
  
 天体着色器代码见vertShader.vert与fragShader.frag
 
 ## 鼠标选择，点击不同球体显示不同名称
 
# 茶壶倒茶动画效果 ❤️：
[外链图片转存失败,源站可能有防盗链机制,建议将图片保存下来直接上传(img-UbZ2rIjz-1671460868189)(https://github.com/seimeicyx/graphics2022/blob/main/22251126%E9%99%88%E6%BB%A2%E8%A5%BF/Project1/result/still.gif)]
