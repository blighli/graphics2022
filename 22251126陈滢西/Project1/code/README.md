# 最终实现：基于glut的茶壶倒茶动画
 * 茶杯倒茶的动画
 * 不同视角与距离观察模型
 * 播放bgm
 * 点击查看发布在bilibili上的[程序视频](
https://www.bilibili.com/video/BV1gA411R78D/?vd_source=881f21b11ef8e2832f37c7c84736a66c)❤️

**相机控制效果❤️：**
![image](https://github.com/seimeicyx/graphics2022/blob/main/22251126%E9%99%88%E6%BB%A2%E8%A5%BF/Project1/result/move.gif)

---
# 解决方案

 ## 水波正弦函数模拟
 
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

 * 颜色渐变以保证水倒下的渐变效果
 * 茶壶
 *	* 光照
 *	 * 材质
 *	计算与调整位置参数以保证效果
 *	bgm播放
 *	glut+vs环境搭建



善用分割线
黑体强调
放gif
放可爱的表情
放bilibili链接
**茶壶倒茶动画效果：**
![image](https://github.com/seimeicyx/graphics2022/blob/main/22251126%E9%99%88%E6%BB%A2%E8%A5%BF/Project1/result/still.gif)
