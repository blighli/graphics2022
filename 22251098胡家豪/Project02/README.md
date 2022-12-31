********
## 任务：
### 必选特性：
#### 1、光照
##### 太阳为光源
#### 2、纹理
##### 使用图片进行纹理映射
### 可选特性
#### 3、使用顶点着色器和片段着色器
##### 自己实现光照效果
#### 4、鼠标选择
##### 点击不同球体显示不同名称
********
#### 使用框架/库 glfw-3.3.5、glew-2.2.0-win32、glad、glm-0.9.9
#### 编译器vs2019，项目运行采用x64
##### 参考：https://learnopengl-cn.github.io/
********
### 使用说明：
#### 1.功能键“Esc”做窗口退出键。
#### 2.功能键“WASD”做窗口上下左右移动（即，物体相反移动）键，W上、S下、A左、D右。
#### 3.功能键“JK”做窗口远近移动（即，物体缩放）键，J近（物体放大）、K远（物体缩小）。
#### 4.功能键“C”做还原键（即，重置旋转角步长rotate_angle_step为默认角度值），不同旋转角度按周期比计算。
#### 5.功能键“↑↓”做旋转速度变换键，“↑”加速正向（逆时针）旋转（即加大旋转步长），“↓”减速正向旋转（可减至逆运行，即顺时针旋转）。
#### 6.鼠标移动做俯仰角(Pitch)和偏航角(Yaw)变换，即实现上下看、左右看。
#### 7.滚鼠标轮的功能同键“JK”，前滚J、后滚K。
********
### 任务图例：
### <img src="ppt.png" width="280"/><br/>
### 运行截图：
<img src="./运行截图/1.png" width="280"/> <img src="./运行截图/2.png" width="280"/> <img src="./运行截图/3.png" width="280"/><br/>
<img src="./运行截图/4.png" width="280"/> <img src="./运行截图/5.png" width="280"/> <img src="./运行截图/6.png" width="280"/><br/>
********
### 实现说明：
#### 1.项目实现了以太阳为光源的光照。
#### 2.项目实现了太阳、地球、月球绘制即纹理贴图，贴图如下：太阳（左）、地球（中）、月球（右）：
#### <img src="sun.jpg" width="280"/> <img src="earth.jpg" width="280"/> <img src="moon.jpg" width="280"/><br/>
#### 3.项目实现了不同球体的公转、自转（太阳自转，地球公转、自转，月球公转、自转），转速按周期比计算，旋转角、轴及其步长说明如下：
``` c++
    glm::vec3 sun_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);//太阳绕y轴自转
    glm::vec3 earth_rotate_axis = glm::vec3(cos(glm::radians(66.34)), sin(glm::radians(66.34)), 0.0f);//地球自转轴与黄道面(地球的公转轨道平面)成66.34度夹角
    glm::vec3 moon_rotate_axis = glm::vec3(cos(glm::radians(1.5424)), sin(glm::radians(1.5424)), 0.0f);//月球的轨道平面(白道面)与黄道面保持5.145 396°的夹角，月球自转轴则与黄道面的法线成1.5424°的夹角
    rotated_angle += rotate_angle_step;
    //rotate_angle_step = 20.0f;默认值为0.5f
    //假设地球每次转20度，太阳每次转(rotated_angle*earth_rotate地球自转周期/sun_rotate太阳自转周期)度
```
#### 4.项目实现由于按实际值计算差太大，不便于视口观看，对如球体直径、球间距、旋转周期做了调整，具体值如下：
``` c++
    //实际值差太大
    float sun_d = 6.0f;//float sun_d = 1392000.0f;//太阳直径，km
    float sun_rotate = 25.0f * 24.0f;//float sun_rotate = 25.05f;//太阳自转周期，天，h
    float sun_earth_len = 18.0f;//float sun_earth_len = 149597870.0f;//日地距离，km
    float earth_d = 2.0f;//float earth_d = 12742.0f;//地球直径，km
    float earth_rotate = 1.0f * 24.0f;//地球自转周期，天，h
    float earth_sun_rotate = 365.0f * 24.0f;//float earth_sun_rotate = 365.256363f;//地球公转，天，h
    float moon_earth_len = 6.0f;//float moon_earth_len = 384000.0f;//月地距离，km
    float moon_d = 0.5f;//float moon_d = 3476.28f;//月球直径，km
    float moon_rotate = 27.0f * 24.0f;//float moon_rotate = 27.32f;//月球自转周期，天，h
    float moon_earth_rotate = 27.0f * 24.0f;//float moon_earth_rotate = 27.32f;//月球公转，天，h
```
#### 5.项目实现使用了顶点着色器和片段着色器实现光照效果。
#### 6.其他未说明见“使用说明”。
********
