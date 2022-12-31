#include <GL/glut.h>
#include <GL/SOIL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma pack(1)
#define DISCRETE_GRAPHICS 1
#if DISCRETE_GRAPHICS
__declspec(dllexport) int NvOptimusEnablement = 0x00000001; //使用独显
#endif

#define Pi 3.1415926
#define RADIUS       10.0       //观察点位置所在球面半径
#define ROTATE_SPEED 0.01       //观察点移动速度
#define MERCURY_ORBIT_RADIUS 0.6
#define VENUS_ORBIT_RADIUS   1.0
#define EARTH_ORBIT_RADIUS   1.5
#define MARS_ORBIT_RADIUS    2.0
#define JUPITER_ORBIT_RADIUS 2.7
#define SATURN_ORBIT_RADIUS  3.4
#define URANUS_ORBIT_RADIUS  4.0
#define NEPTUNE_ORBIT_RADIUS 4.76

#define SUN_RADIUS 0.5
#define MERCURY_RADIUS 0.05

static void Resize(int w, int h);
static void Display(void);                                      //显示回调函数
static void Idle(void);                                         //空闲回调函数
static void ProcessNormalKeys(unsigned char key, int x, int y); //ASCII按键响应函数
static void ProcessSpecialKeys(int skey, int x, int y);         //非ASCII按键响应函数
static void MouseMove(int x, int y);                            //鼠标移动回调函数
static void MouseButton(int button, int state, int x, int y);   //鼠标按键回调函数

static void ChangeViewPosition(GLdouble theta, GLdouble phi);
static void CreateGLUTMenus(void);
//display text function
static void SetOrthographicProjection(void);
static void RestorePerspectiveProjection(void);
static void RenderBitmapFontString(GLfloat x, GLfloat y, GLfloat z, void* font, char* string); //show bitmap font
static void RenderStrokeFontString(GLfloat x, GLfloat y, GLfloat z, void* font, char* string); //show stroke font
//texture
void LoadPicture(char* file_name, GLuint* texture);

static int window_width, window_height;
static int main_menu;
static int mouse_x = -1, mouse_y = -1;
//observer's parameters
static GLdouble radius = RADIUS; //The radius of the sphere where the observer is located
static GLdouble theta_angle = 0.0, phi_angle = Pi / 2.0;
static GLdouble delta_theta = 0.0, delta_phi = 0.0;
static GLdouble x = RADIUS, y = 0.0, z = 0.0; //position of the viewer
static GLdouble cx = 0.0, cy = 0.0, cz = 0.0; //viewer's direction
static GLdouble hx = 0.0, hy = 0.0, hz = 1.0; //viewer's head direction
//light
const GLfloat light0_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //光源中环境光的颜色，黑色。红，绿，蓝，第四个分量1.0表示不透明。
const GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //光源中漫射光的颜色，白色
const GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //定向光源，指定了光源的方向
//materials
const GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };      //材质的环境颜色
const GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };      //材质的漫射光颜色
const GLfloat high_shininess[] = { 10.0f };                    //材质的镜面反射指数

int main(int argc, char* argv[])
{
    // init GLUT and create window
    glutInit(&argc, argv);
    // GLuint texture;
    // LoadPicture("img_test.bmp", &texture);
    glutInitWindowSize(1000, 1000);   //set window size
    glutInitWindowPosition(1000, -1); //set window position
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("solar system"); //根据前面设置的创建窗口，参数被作为窗口的标题。
    glShadeModel(GL_SMOOTH);
    //light
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    //materials
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);//启用0号光源
    glEnable(GL_NORMALIZE);// 启用自动单位化法向量
    glEnable(GL_COLOR_MATERIAL);//启用材质颜色
    glEnable(GL_LIGHTING);//启用光照

    // register callbacks
    glutReshapeFunc(Resize);  //注册当前窗口的再整形回调函数
    glutDisplayFunc(Display); //注册当前窗口的显示回调函数
    glutIdleFunc(Idle);       //注册空闲回调函数
    glutKeyboardFunc(ProcessNormalKeys); //注册ASCII按键响应函数
    glutSpecialFunc(ProcessSpecialKeys); //注册非ASCII按键响应函数
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //启用反走样
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);  // Antialias the lines
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    ChangeViewPosition(theta_angle, phi_angle);
    CreateGLUTMenus();

    glutMainLoop();//主循环处理
    return EXIT_SUCCESS;
}

void Idle(void)
{
    glutPostRedisplay();
}

static void Resize(int width, int height)
{
    GLdouble ratio;
    height = (!height) ? 0.1 : height;
    window_width = width;
    window_height = height;
    ratio = 1.0 * width / height;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //设置视口为整个窗口大小
    glViewport(0, 0, width, height);
    //设置可视空间
    gluPerspective(60, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x + cx, y + cy, z + cz,
        cx, cy, cz,
        hx, hy, hz);
}

void DrawCircle(GLdouble R)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 7200; i++)
    {
        glVertex2f(R * cos(2 * Pi * i / 7200), R * sin(2 * Pi * i / 7200));
    }
    glEnd();
}

inline double GetDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

void AutoChangeTextAngle(void)
{
    glRotated(90, 0, 0, 1);
    glRotated(90, 1, 0, 0);
    glRotated((theta_angle + delta_theta) * 180.0l / Pi, 0, 1, 0);
    glRotated((phi_angle + delta_phi) * 180.0l / Pi, -1, 0, 0);
}
//display
static void Display(void)//显示回调函数 图片绘制在XOY平面上
{
    static double angle = 0;
    const int slices = 60;
    const int stacks = 60;
    GLdouble star_x, star_y, star_z;
#if DISCRETE_GRAPHICS
    angle += 0.005;
#else
    angle += 0.001;
#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区
    //Sun
    glColor3d(1, 0.5, 0.04);
    glPushMatrix();
    glutSolidSphere(SUN_RADIUS, slices, stacks);
    AutoChangeTextAngle();
    glPopMatrix();
    //Mercury
    star_x = MERCURY_ORBIT_RADIUS * cos(1 / 0.24 * angle);
    star_y = MERCURY_ORBIT_RADIUS * sin(1 / 0.24 * angle);
    star_z = 0.0;
    glColor3f(0.61f, 0.33f, 0.16f);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(MERCURY_RADIUS, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.25f, 0.1f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Mercury");
    }
    glPopMatrix();
    DrawCircle(MERCURY_ORBIT_RADIUS);
    //Venus
    star_x = VENUS_ORBIT_RADIUS * cos(1 / 0.62 * angle);
    star_y = VENUS_ORBIT_RADIUS * sin(1 / 0.62 * angle);
    star_z = 0.0;
    glColor3f(1.0f, 0.65f, 0.31f);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.065, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.2f, 0.1f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Venus");
    }
    glPopMatrix();
    DrawCircle(VENUS_ORBIT_RADIUS);
    //Earth
    star_x = EARTH_ORBIT_RADIUS * cos(1.0 * angle);
    star_y = EARTH_ORBIT_RADIUS * sin(1.0 * angle);
    star_z = 0.0;
    glColor3f(0.18f, 0.3f, 0.41f);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.07, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.15f, 0.1f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Earth");
    }
    glPopMatrix();
    DrawCircle(EARTH_ORBIT_RADIUS);
    //Mars
    star_x = MARS_ORBIT_RADIUS * cos(1 / 1.9 * angle);
    star_y = MARS_ORBIT_RADIUS * sin(1 / 1.9 * angle);
    star_z = 0.0;
    glColor3f(0.54, 0.058, 0.027);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.053, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.15f, 0.1f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Mars");
    }
    glPopMatrix();
    DrawCircle(MARS_ORBIT_RADIUS);
    //Jupiter
    star_x = JUPITER_ORBIT_RADIUS * cos(1 / 11.9 * angle);
    star_y = JUPITER_ORBIT_RADIUS * sin(1 / 11.9 * angle);
    star_z = 0.0;
    glColor3f(0.55, 0.42, 0.35);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.2, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.2f, 0.3f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Jupiter");
    }
    glPopMatrix();
    DrawCircle(JUPITER_ORBIT_RADIUS);
    //Saturn
    star_x = SATURN_ORBIT_RADIUS * cos(1 / 29.5 * angle);
    star_y = SATURN_ORBIT_RADIUS * sin(1 / 29.5 * angle);
    star_z = 0.0;
    glColor3f(0.7, 0.46, 0.22);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.1, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.2f, 0.2f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Saturn");
    }
    glPopMatrix();
    DrawCircle(SATURN_ORBIT_RADIUS);
    //Uranus
    star_x = URANUS_ORBIT_RADIUS * cos(1 / 83.0 * angle);
    star_y = URANUS_ORBIT_RADIUS * sin(1 / 83.0 * angle);
    star_z = 0.0;
    glColor3f(0.39, 0.68, 0.87);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.07, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.2f, 0.17f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Uranus");
    }
    glPopMatrix();
    DrawCircle(URANUS_ORBIT_RADIUS);
    //Neptune
    star_x = NEPTUNE_ORBIT_RADIUS * cos(1 / 164.8 * angle);
    star_y = NEPTUNE_ORBIT_RADIUS * sin(1 / 164.8 * angle);
    star_z = 0.0;
    glColor3f(0.29, 0.53, 0.9);
    glPushMatrix();
    glTranslated(star_x, star_y, star_z);
    glutSolidSphere(0.07, slices, stacks);
    if (GetDistance(star_x, star_y, star_z, x, y, z) < GetDistance(0, 0, 0, x, y, z))
    {
        AutoChangeTextAngle();
        RenderStrokeFontString(-0.25f, 0.17f, 0.0f, GLUT_STROKE_ROMAN, (char*)"Neptune");
    }
    glPopMatrix();
    DrawCircle(NEPTUNE_ORBIT_RADIUS);
    //compute frames per second
    static int time = 0, frame = 0, last_time = 0;
    static double fps = 0.0l;
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - last_time >= 1000)
    {
        fps = frame * 1000.0l / (time - last_time);
        last_time = time;
        frame = 0;
    }
    //display a string with bitmap fonts
    SetOrthographicProjection();
    glPushMatrix();
    glLoadIdentity();
    glColor3d(1, 0.5, 0.04);

    char str[64];
    int wwidth = glutGet(GLUT_WINDOW_WIDTH);
    RenderBitmapFontString(wwidth / 2 - 50, 20, 0, GLUT_BITMAP_HELVETICA_18, (char*)"Solar System");
    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(str, "FPS: %.2lf", fps);
    RenderBitmapFontString(5, 20, 0, GLUT_BITMAP_HELVETICA_18, str);
    sprintf(str, "Observation Point: x=%.2lf, y=%.2lf, z=%.2lf", x + cx, y + cy, z + cz);
    RenderBitmapFontString(5, 40, 0, GLUT_BITMAP_HELVETICA_18, str);
    sprintf(str, "theta=%.2lf, phi=%.2lf, radius=%.2lf", (theta_angle + delta_theta) * 180.0 / Pi, (phi_angle + delta_phi) * 180.0 / Pi, radius);
    RenderBitmapFontString(5, 60, 0, GLUT_BITMAP_HELVETICA_18, str);
    glPopMatrix();
    RestorePerspectiveProjection();
    //交换缓冲区
    glutSwapBuffers();
}
//view
void ChangeViewDistance(unsigned char option)
{
    switch (option)
    {
    case '+':
        radius -= 0.1;
        radius = (radius < 0.1) ? 0.1 : radius;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    case '-':
        radius += 0.1;
        radius = (radius > 15) ? 15 : radius;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    default:
        break;
    }
}

void ChangeViewPosition(GLdouble theta, GLdouble phi)
{
    x = radius * cos(phi) * cos(theta);
    y = radius * cos(phi) * sin(theta);
    z = radius * sin(phi);
    glLoadIdentity();
    gluLookAt(x + cx, y + cy, z + cz,
        cx, cy, cz,
        hx, hy, hz);
}
//key
static void ProcessSpecialKeys(int skey, int x, int y)//非ASCII按键响应函数
{
    switch (skey) {
    case GLUT_KEY_LEFT:
        theta_angle -= ROTATE_SPEED;
        theta_angle = (theta_angle < -Pi) ? -Pi : theta_angle;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    case GLUT_KEY_RIGHT:
        theta_angle += ROTATE_SPEED;
        theta_angle = (theta_angle > Pi) ? Pi : theta_angle;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    case GLUT_KEY_UP:
        phi_angle += ROTATE_SPEED;
        phi_angle = (phi_angle > Pi / 2.0l) ? Pi / 2.0l : phi_angle;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    case GLUT_KEY_DOWN:
        phi_angle -= ROTATE_SPEED;
        phi_angle = (phi_angle < 0.0l) ? 0.0l : phi_angle;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    case GLUT_KEY_HOME:
        theta_angle = 0.0l;
        phi_angle = Pi / 2.0l;
        ChangeViewPosition(theta_angle, phi_angle);
        break;
    default:
        break;
    }
}

static void ProcessNormalKeys(unsigned char key, int x, int y)//ASCII按键响应函数
{
    switch (key)
    {
    case ' ':
    case 'q':
        glutDestroyMenu(main_menu);
        exit(0);
        break;
    case '+':
        ChangeViewDistance('+');
        break;
    case '-':
        ChangeViewDistance('-');
        break;
    default:
        break;
    }
}
//mouse
void MouseMove(int x, int y)
{
    // this will only be true when the left button is down
    if (mouse_x >= 0 && abs(mouse_x - x) >= 5)
    {
        delta_theta = (double)(mouse_x - x) * ROTATE_SPEED;
        if (theta_angle + delta_theta > Pi)
            delta_theta = Pi - theta_angle;
        else if (theta_angle + delta_theta < -Pi)
            delta_theta = -Pi - theta_angle;
    }
    if (mouse_y >= 0 && abs(y - mouse_y) >= 10)
    {
        delta_phi = (double)(y - mouse_y) * ROTATE_SPEED;
        if (phi_angle + delta_phi > Pi / 2.0l)
            delta_phi = Pi / 2.0l - phi_angle;
        else if (phi_angle + delta_phi < 0.0l)
            delta_phi = -phi_angle;
    }
    ChangeViewPosition(theta_angle + delta_theta, phi_angle + delta_phi);
}

void MouseButton(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        // only start motion if the left button is pressed
        if (state == GLUT_UP)
        {
            theta_angle += delta_theta;
            phi_angle += delta_phi;
            delta_theta = 0;
            delta_phi = 0;
            mouse_x = -1;
            mouse_y = -1;
        }
        else
        {
            mouse_x = x;
            mouse_y = y;
        }
        break;
    case 3:
        if (state == GLUT_UP)
        {
            radius += ROTATE_SPEED;
        }
    default:
        break;
    }
}
//menu
void ProcessMenuEvents(int option)
{
    switch (option)
    {
    case 1:
        ProcessSpecialKeys(GLUT_KEY_HOME, 0, 0);
        break;
    case 2:
        ChangeViewDistance('+');
        break;
    case 3:
        ChangeViewDistance('-');
        break;
    case 4:
        ProcessNormalKeys('q', 0, 0);
        break;
    default:
        break;
    }
}

void CreateGLUTMenus(void)
{
    main_menu = glutCreateMenu(ProcessMenuEvents);
    glutAddMenuEntry("Reset        'home'", 1);
    glutAddMenuEntry("Zoom In    '+'", 2);
    glutAddMenuEntry("Zoom Out  '-'", 3);
    glutAddMenuEntry("Exit            'q'", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//font
void SetOrthographicProjection(void)
{

    glMatrixMode(GL_PROJECTION); // switch to projection mode
    glPushMatrix();              // save previous matrix which contains the settings for the perspective projection
    glLoadIdentity();                              // reset matrix
    gluOrtho2D(0, window_width, window_height, 0); // set a 2D orthographic projection
    glMatrixMode(GL_MODELVIEW);                    // switch back to modelview mode
}

void RestorePerspectiveProjection(void)
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();              // restore previous projection matrix
    glMatrixMode(GL_MODELVIEW); // get back to modelview mode
}

void RenderBitmapFontString(GLfloat x, GLfloat y, GLfloat z, void* font, char* string)
{
    glRasterPos3f(x, y, z);
    while (*string != '\0')
    {
        glutBitmapCharacter(font, *string);
        string++;
    }
}

void RenderStrokeFontString(GLfloat x, GLfloat y, GLfloat z, void* font, char* string)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.001f, 0.001f, 0.001f);
    while (*string != '\0')
    {
        glutStrokeCharacter(font, *string);
        string++;
    }
    glPopMatrix();
}
//texture
GLuint texture_name;
struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};
struct BITMAPINFOHEADER {
    unsigned int biSize;
    long biWidth;
    long biHeight;
    unsigned short biPlanes;
    unsigned short biBitcount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};
void LoadPicture(char* file_name, GLuint* texture)
{
    FILE* bmp = NULL;
    struct BITMAPFILEHEADER bmfh;
    struct BITMAPINFOHEADER bmih;
    bmp = fopen(file_name, "rb"); //装入位图
    if (bmp == NULL)
        printf("load picture error!\r\n");
    fseek(bmp, 0, SEEK_END);
    int bmp_size = ftell(bmp);
    fseek(bmp, 0, SEEK_SET);
    unsigned char* p_image = malloc(bmp_size * sizeof(char));
    fread(p_image, bmp_size, 1, bmp);
    fseek(bmp, 0, SEEK_SET);
    fread(&bmfh, sizeof(struct BITMAPFILEHEADER), 1, bmp);
    fread(&bmih, sizeof(struct BITMAPINFOHEADER), 1, bmp);
    glGenTextures(1, texture);              //生成贴图
    glBindTexture(GL_TEXTURE_2D, *texture); //贴图生效
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bmih.biWidth, bmih.biHeight,
        GL_RGB, GL_UNSIGNED_BYTE, p_image); //贴图数据
    printf("%ld %ld\n", bmih.biWidth, bmih.biHeight);
    printf("%d", bmp_size);
    fclose(bmp);
}

