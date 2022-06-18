// GlutScissorDemo.cpp : 定义控制台应用程序的入口点。

//
#include "stdafx.h"
#include <gl/glut.h>
#include <math.h>
//圆周率宏
#define GL_PI 3.1415f

//获取屏幕的宽度
GLint SCREEN_WIDTH = 0;
GLint SCREEN_HEIGHT = 0;

//设置程序的窗口大小
GLint windowWidth = 400;
GLint windowHeight = 300;

//绕x轴旋转角度
GLfloat xRotAngle = 0.0f;
//绕y轴旋转角度
GLfloat yRotAngle = 0.0f;

//受支持的点大小范围
GLfloat sizes[2];
//受支持的点大小增量
GLfloat step;

//显示回调函数
void renderScreen(void)
{
    GLfloat x, y, z, angle;
    int i;
    // Clear blue window
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    //把整个窗口清理为当前清理颜色：蓝色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //将当前Matrix状态入栈
    glPushMatrix();
    //坐标系绕x轴旋转xRotAngle
    glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
    //坐标系绕y轴旋转yRotAngle
    glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    //进行平滑处理　
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    //绘制坐标系
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-80.0f, 0.0f, 0.0f);
    glVertex3f(80.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -80.0f, 0.0f);
    glVertex3f(0.0f, 80.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -80.0f);
    glVertex3f(0.0f, 0.0f, 80.0f);
    glEnd();

    glPushMatrix();
    glTranslatef(80.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutWireCone(3, 6, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 80.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutWireCone(3, 6, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 80.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glutWireCone(3, 6, 10, 10);
    glPopMatrix();

    //使能裁剪区
    glEnable(GL_SCISSOR_TEST);
    //将窗口中间的四分之一面积清空为红色
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    glScissor(windowWidth / 4, windowHeight / 4, windowWidth / 2, windowHeight / 2);
    glClear(GL_COLOR_BUFFER_BIT);

    //将窗口中间的十六分之一面积清空为绿色
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glScissor(windowWidth * 3 / 8, windowHeight * 3 / 8, windowWidth / 4, windowHeight / 4);
    glClear(GL_COLOR_BUFFER_BIT);

    //禁止裁剪区
    glDisable(GL_SCISSOR_TEST);

    //恢复压入栈的Matrix
    glPopMatrix();
    //交换两个缓冲区的指针
    glutSwapBuffers();
}
//设置Redering State
void setupRederingState(void)
{
    //设置清理颜色为黑色
    glClearColor(0.0f, 0.0, 0.0, 1.0f);
    //设置绘画颜色为绿色
    glColor3f(0.0f, 1.0f, 0.0f);
    //使能深度测试
    glEnable(GL_DEPTH_TEST);
    //获取受支持的点大小范围
    glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
    //获取受支持的点大小增量
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
    printf("point size range:%f-%f\n", sizes[0], sizes[1]);
    printf("point step:%f\n", step);
}
//窗口大小变化回调函数
void changSize(GLint w, GLint h)
{
    //横宽比率
    GLfloat ratio;
    //设置坐标系为x(-100.0f,100.0f)、y(-100.0f,100.0f)、z(-100.0f,100.0f)
    GLfloat coordinatesize = 100.0f;
    //窗口宽高为零直接返回
    if ((w == 0) || (h == 0))
        return;
    //设置视口和窗口大小一致
    glViewport(0, 0, w, h);
    //对投影矩阵应用随后的矩阵操作
    glMatrixMode(GL_PROJECTION);
    //重置当前指定的矩阵为单位矩阵　
    glLoadIdentity();
    ratio = (GLfloat)w / (GLfloat)h;
    //正交投影
    if (w < h)
        glOrtho(-coordinatesize, coordinatesize, -coordinatesize / ratio, coordinatesize / ratio, -coordinatesize, coordinatesize);
    else
        glOrtho(-coordinatesize * ratio, coordinatesize * ratio, -coordinatesize, coordinatesize, -coordinatesize, coordinatesize);
    //对模型视图矩阵堆栈应用随后的矩阵操作
    glMatrixMode(GL_MODELVIEW);
    //重置当前指定的矩阵为单位矩阵　
    glLoadIdentity();
}

//按键输入处理回调函数
void specialKey(int key, int x, int y)
{

    if (key == GLUT_KEY_UP)
    {
        xRotAngle -= 5.0f;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        xRotAngle += 5.0f;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        yRotAngle -= 5.0f;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        yRotAngle += 5.0f;
    }
    //重新绘制
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    int nModeMenu;
    int nEdgeMenu;
    int nDepthMask;
    int nMainMenu;
    //初始化glut
    glutInit(&argc, argv);
    //使用双缓冲区模式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    //获取系统的宽像素
    SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
    //获取系统的高像素
    SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
    //创建窗口，窗口名字为OpenGL Scissor Demo
    glutCreateWindow("OpenGL Scissor Demo");
    //设置窗口大小
    glutReshapeWindow(windowWidth, windowHeight);
    //窗口居中显示
    glutPositionWindow((SCREEN_WIDTH - windowWidth) / 2, (SCREEN_HEIGHT - windowHeight) / 2);
    //窗口大小变化时的处理函数
    glutReshapeFunc(changSize);
    //设置显示回调函数
    glutDisplayFunc(renderScreen);
    //设置按键输入处理回调函数
    glutSpecialFunc(specialKey);
    //设置全局渲染参数
    setupRederingState();
    glutMainLoop();
    return 0;
}