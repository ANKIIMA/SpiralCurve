#include<GL/glut.h>
#include<math.h>
#include<stdio.h>
#define PI 3.14
#define SIZE 500//名称最大数量
#define N 3//拾取矩形尺寸
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

int width = 0;//线宽
int mode = 0;//用于选择线型
int xmove = 0, ymove = 0;//移动绘制
int hscreen = 0;//保存reshape中屏幕高度
GLboolean is1 = GL_FALSE, is2 = GL_FALSE;//是否选中并保持鼠标按下
int dx = 0, dy = 0;//保存鼠标在图形中的位置
int colorRED_tri = 0;//当前颜色
int colorGREEN_tri = 0;
int colorRED_a = 0;
int colorGREEN_a = 0;

void display_Curve();//曲线绘制
void display_2D();//三角形和四边形
GLint xt1 = 400, yt1 = 300;//三角形顶点
GLint xt2 = 500, yt2 = 300;
GLint xt3 = 450, yt3 = 450;
GLint xa1 = 100, ya1 = 100;//矩形顶点
GLint xa2 = 300, ya2 = 300;

void display_3D();//圆台绘制
GLfloat r1 = 3, r2 = 6;//圆台上下半径

void init1();//曲线绘制初始化
void init2();//2D绘制初始化
void init3();//3D绘制初始化

void reshape2(int h, int w);//2D重绘函数

void mykey_Curve(unsigned char key, int x, int y);//曲线的键盘交互

void mymouse_2D(int btn, int state, int x, int y);//2D的鼠标交互
void draw_objects(GLenum mode);//2D物体绘制
void processHits(GLint hits, GLuint buffer[]);//处理命中
void mymotion(GLint x, GLint y);//处理鼠标移动
void mymenu(int value);//菜单
void mysubmenu1(int value);//三角形菜单
void mysubmenu2(int value);//四边形菜单

void mykey_3D(unsigned char key, int x, int y);

int main(int argc,char ** argv)
{
    glutInit(&argc, argv);
//曲线绘制窗口
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Curve");
    init1();
    //注册函数
    glutDisplayFunc(display_Curve);
    glutKeyboardFunc(mykey_Curve);//按Q增加线宽，按E改变线型

//三角形和四边形绘制窗口
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(SCREEN_WIDTH, 0);
    glutCreateWindow("Triangles and Quadrilateral");
    init2();
    //注册函数
    glutDisplayFunc(display_2D);
    glutMouseFunc(mymouse_2D);
    glutMotionFunc(mymotion);
    glutReshapeFunc(reshape2);
    //注册菜单
    int submenu1 = glutCreateMenu(mysubmenu1);
    glutAddMenuEntry("红色", 1);
    glutAddMenuEntry("绿色", 2);

    int submenu2 = glutCreateMenu(mysubmenu2);
    glutAddMenuEntry("红色", 1);
    glutAddMenuEntry("绿色", 2);

    int mainmenu = glutCreateMenu(mymenu);
    glutAddMenuEntry("全白", 1);
    glutAddSubMenu("三角形", submenu1);
    glutAddSubMenu("四边形", submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

//圆台绘制窗口
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(2 * SCREEN_WIDTH, 0);
    glutCreateWindow("Round Platform");
    init3();
    //注册函数
    glutDisplayFunc(display_3D);
    glutKeyboardFunc(mykey_3D);
    

    glutMainLoop();

    return 0;

}

void display_Curve()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float x, y, z, t;
    float a = 2, b = 3, c = 18;
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (t = 0; t < 2 * PI; t+=0.01)
    {
        x = a * t * cos(c * t) + b;
        y = a * t * sin(c * t) + b;
        z = c * t;
        glVertex3f(x, y, z);
    }
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    for (t = 0; t < 2 * PI; t += 0.01)
    {
        x = (a * sin(c * t) + b) * cos(t);
        y = (a * sin(c * t) + b) * sin(t);
        z = a * cos(c * t);
        glVertex3f(x, y, z);
    }
    glEnd();


    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(12, 0, 0);
    glEnd();

    glBegin(GL_LINES); 
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 12);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 12, 0);
    glEnd();

    glFlush();
}

void mykey_Curve(unsigned char key, int x, int y)
{
    
    if (key == 'q' || key == 'Q')
    {
        width += 1;
        glLineWidth(width);
        display_Curve();
    }
    if (key == 'e' || key == 'E')
    {
        glEnable(GL_LINE_STIPPLE);
        int pattern[6] = { 0x0101,0x27FF,0x33FF, 0x3F3F,0x24FF,0x55FF};
        mode = mode + 1;
        if (mode == 6)mode = 0;
        glLineStipple(1, pattern[mode]);
        display_Curve();
    }
}

void init1()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);
    gluLookAt(1, 1, 1, 3, 3, 3, -1, -1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-12, 12, -12, 12, -12, 12);
}

void display_2D()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw_objects(GL_RENDER);
    glutSwapBuffers();
}

void mymouse_2D(int btn, int state, int x, int y)
{
    GLuint nameBuffer[SIZE];//名称缓存
    GLint hits;
    GLint viewport[4];
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        xmove = x;
        ymove = hscreen - y;
        glGetIntegerv(GL_VIEWPORT, viewport); 
        glSelectBuffer(SIZE, nameBuffer); 

        glRenderMode(GL_SELECT); 
        //进入选择模式
        glInitNames(); 
        glPushName(0);
        glMatrixMode(GL_PROJECTION);
        //进入投影变换模式
        glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(x, viewport[3] - y, N, N, viewport);
        gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT); 
        draw_objects(GL_SELECT);
        glPopMatrix();
        //完成选中 处理消息
        hits = glRenderMode(GL_RENDER);
        if(hits>0)processHits(hits, nameBuffer);
        //重绘
        glutPostRedisplay();
    }

    if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        is1 = GL_FALSE;
        is2 = GL_FALSE;
    }
}

void mymotion(GLint x, GLint y)
{
    if (is1 == GL_TRUE)
    {
        xmove = x;
        ymove = hscreen-y;

        xt1 = xmove - dx;
        yt1 = ymove - dy;

        xt2 = xt1 + 100;
        yt2 = yt1;

        xt3 = 50 + xt1;
        yt3 = 150 + yt1;

        glutPostRedisplay();
    }

    if (is2 == GL_TRUE)
    {
        xmove = x;
        ymove = hscreen - y;

        xa1 = xmove - dx;
        ya1 = ymove - dy;

        xa2 = xa1 + 200;
        ya2 = ya1 + 200;

        glutPostRedisplay();
    }
}

void draw_objects(GLenum mode)
{
    if (mode == GL_SELECT) glLoadName(1);
    glColor3f(1.0, 1.0, 1.0);
    if (colorRED_tri == 1)
        glColor3f(1.0, 0.0, 0.0);
    if (colorGREEN_tri == 1)
        glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xt1, yt1);//xt1=400,yt1=300
    glVertex2f(xt2, yt2);//xt2=500,yt2=300
    glVertex2f(xt3, yt3);//xt3=450,yt3=450
    glEnd();

    if (mode == GL_SELECT) glLoadName(2);
    glColor3f(1.0, 1.0, 1.0);
    if (colorRED_a == 1)
        glColor3f(1.0, 0.0, 0.0);
    if (colorGREEN_a == 1)
        glColor3f(0.0, 1.0, 0.0);
    glRectf(xa1,ya1,xa2,ya2);
}

void processHits(GLint hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint names;
    printf("hits = %d\n", hits);


    for (i = 0; i < hits; i++)
    {
        names = buffer[3 + 4 * i];
        if (names == 1)
        {
            printf("三角形\n");
            is1 = GL_TRUE;
            dx = xmove - xt1;
            dy = ymove - yt1;
        }

        else if (names == 2)
        {
            printf("四边形\n");
            is2 = GL_TRUE;
            dx = xmove - xa1;
            dy = ymove - ya1;
        }
    }
    
}

void reshape2(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluOrtho2D(0, w, 0, h);
    
    hscreen = h;
}

void mysubmenu1(int value)
{
    if (value == 1)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        colorRED_tri = 1;
        glutPostRedisplay();
    }

    if (value == 2)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        colorGREEN_tri = 1;
        glutPostRedisplay();
    }
}

void mysubmenu2(int value)
{

    if (value == 1)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        colorRED_a = 1;
        glutPostRedisplay();
    }

    if (value == 2)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        colorGREEN_a = 1;
        glutPostRedisplay();
    }
}

void mymenu(int value)
{
    if (value == 1)
    {
        glColor3f(1.0, 1.0, 1.0);
        colorGREEN_a = 0;
        colorGREEN_tri = 0;
        colorRED_a = 0;
        colorRED_tri = 0;
        glutPostRedisplay();
    }
}

void init2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}

void display_3D()
{
    glClear(GL_COLOR_BUFFER_BIT);
    GLint theta = 0;
    float x, y, z;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0;i<30 ;i++, theta += 12)//侧面
    {
        glColor3f(1.0,1.0,1.0);
        x = r1 * sin(theta);
        y = r1 * cos(theta);
        z = 8;
        glVertex3f(x,y,z);

        x = r2 * sin(theta);
        y = r2 * cos(theta);
        z = 0;
        glVertex3f(x,y,z);
    }
    glEnd();

    int black = 1;
    glBegin(GL_TRIANGLE_FAN);//顶部
    glVertex3f(0.0, 0.0, 8.0);
    for (int i = 0; i < 30; i++, theta += 12,black = !black)
    {
        if (black)
        {
            glColor3f(0.8, 0.8, 0.8);
        }
        else
        {
            glColor3f(0.2, 0.2, 0.2);
        }
        x = r1 * sin(theta);
        y = r1 * cos(theta);
        z = 8;
        glVertex3f(x, y, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);//底部
    glVertex3f(0.0, 0.0, 0.0);
    for (int i = 0; i < 30; i++, theta += 12, black = !black)
    {
        if (black)
        {
            glColor3f(0.8, 0.8, 0.8);
        }
        else
        {
            glColor3f(0.2, 0.2, 0.2);
        }
        x = r2 * sin(theta);
        y = r2 * cos(theta);
        z = 0;
        glVertex3f(x, y, z);
    }
    glEnd();


    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(12, 0, 0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 12);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 12, 0);
    glEnd();

    glutSwapBuffers();
}

void mykey_3D(unsigned char key, int x, int y)
{
    if (key == 'q' || key == 'Q')
    {
        r1 += 0.5;
    }

    if (key == 'e' || key == 'E')
    {
        r2 += 0.5;
    }
    if (key == 'a' || key == 'A')
    {
        r1 -= 0.5;
    }
    if (key == 'd' || key == 'D')
    {
        r2 -= 0.5;
    }

    glutPostRedisplay();
}

void init3()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    gluLookAt(1, 1, 1, 5, 5, 5, -1, -1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-12, 12, -12, 12, -12, 12);
}