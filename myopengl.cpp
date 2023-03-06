#ifdef WIN32  
#pragma execution_character_set("utf-8")
#endif

#include<glew.h>
#include "myopengl.h"
#include"example12.h"
#include<qdiriterator.h>
#include<QtGui/qevent.h>
#include<cmath>
#include <qdebug.h>
#include <iostream>
#include <QtCore/qtimer.h>
#include<ctime>
#include <string.h>
#include <QFileDialog>
#include<qlist.h>
#define  PI 3.1415926
int name;
myopengl::myopengl(QWidget* parent) :QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);//键盘焦点，鼠标单击触发键盘事件
    //setMouseTracking(true);
    pTimer = new QTimer(this); //创建一个定时器
    //将定时器的计时信号与updateGL()绑定
    connect(pTimer, SIGNAL(timeout()), this, SLOT(update()));
    pTimer->start(10);//以10ms为一个计时周期


    //// 设置OpenGL的版本信息
    //QSurfaceFormat format;
    //format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setVersion(2, 0);
    //setFormat(format);

    //变量初始化
    speed = 1.0f;
    yaw = -90.0f;
    pitch = 0.0f;
    m_rtri = 0.0f;
    hits = 0;
    ispressed = false;
    ScalefX = 0.001;
    ScalefY = 0.001;
    ScalefZ = 0.001;
}
myopengl::~myopengl()
{

}
//设置OpenGL渲染环境，定义显示列表等。该函数只在第一次调用resizeGL()或paintGL()前被自动调用一次。
void myopengl::initializeGL()
{

    //glClearColor(0.0, 0.0, 0.5, 1.0);//改变窗口的背景颜色 
    glClearColor(0.3, 0.3, 0.3, 1);
    int err = glewInit();
    if (GLEW_OK != err) {
        // problem: glewInit failed, something is seriously wrong
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;

    }
    //glClearDepth(1.0);//设置深度缓存 
    glEnable(GL_DEPTH_TEST);//允许深度测试 
    //glShadeModel(GL_FLAT);//设置阴影平滑模式 
    //glDepthRange(0.0, 1.0);//the range of z mapping
    //glDepthFunc(GL_LEQUAL);//设置深度测试类型 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正 



    //qDebug() << "modfilelist:" << modFileList.size() << endl;
}
//设置OpenGL的视口、投影等。每次部件改变大小时都会自动调用该函数。
void myopengl::resizeGL(int w, int h)
{
    qDebug() << "W:" << w <<"+"<<"H:"<<h<< endl;
   
    glViewport(0, 0, w, h);
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(w) / GLfloat(h);
    gluPerspective(45.0, x, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

}

//创建显示列表1：用于没有选中.ID号：1 - gimModFileList.size()
void myopengl::CreateDisplayLists1()
{
    for (int i = 0; i < gimModFileList.size(); i++) {
        //qDebug() <<111111<< endl;
        //qDebug() << gimModFileList.size() << endl;
        if (!glIsList((GLuint)(i + 1)))
        {
            glNewList(i + 1, GL_COMPILE);
            //qDebug() << 111111 << endl;
            ModFile modFile = gimModFileList[i];
            glLoadName(i + 1);
            for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
                if (!simpleEntity.getGraphicElement().isNull()) {
                    if (simpleEntity.getVisible() == ("true")) {
                        if (simpleEntity.getGraphicElement().nodeName() == ("Cuboid")) {
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                                //qDebug() << mat[i] << endl;
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            /*GLuint id1 = glGenLists(1);
                            glNewList(id1, GL_COMPILE);*/
                            //绘制线框
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Cuboid(L, W, H);
                            glPopMatrix();
                            //绘制实体
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            Cuboid(L, W, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Sphere")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float R1 = (n.attribute("R")).toFloat();
                            //GLuint id2 = glGenLists(1);
                            //glNewList(id2, GL_COMPILE);
                            //绘制线框
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Sphere(R1);
                            glPopMatrix();
                            //绘制实体
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            Sphere(R1);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("RotationalEllipsoid")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float LR = (n.attribute("LR")).toFloat();
                            float WR = (n.attribute("WR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id3 = glGenLists(1);
                            //glNewList(id3, GL_COMPILE);
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            RotationalEllipsoid(LR, WR, H);
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            RotationalEllipsoid(LR, WR, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Table")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TL1 = (n.attribute("TL1")).toFloat();
                            float TL2 = (n.attribute("TL2")).toFloat();
                            float LL1 = (n.attribute("LL1")).toFloat();
                            float LL2 = (n.attribute("LL2")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id4 = glGenLists(1);
                            //glNewList(id4, GL_COMPILE);
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Table(TL1, TL2, LL1, LL2, H);
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            Table(TL1, TL2, LL1, LL2, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("OffsetRectangularTable")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TL = (n.attribute("TL")).toFloat();
                            float TW = (n.attribute("TW")).toFloat();
                            float LL = (n.attribute("LL")).toFloat();
                            float LW = (n.attribute("LW")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float XOFF = (n.attribute("XOFF")).toFloat();
                            float YOFF = (n.attribute("YOFF")).toFloat();
                            //GLuint id5 = glGenLists(1);
                            //glNewList(id5, GL_COMPILE);
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Cylinder")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float R1 = (n.attribute("R")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id6 = glGenLists(1);
                            //glNewList(id6, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Cylinder(R1, H);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            Cylinder(R1, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("TruncatedCone")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TR = (n.attribute("TR")).toFloat();
                            float BR = (n.attribute("BR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id7 = glGenLists(1);
                            //glNewList(id7, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            TruncatedCone(TR, BR, H);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            TruncatedCone(TR, BR, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("EccentricTruncatedCone")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TR = (n.attribute("TR")).toFloat();
                            float BR = (n.attribute("BR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float TOPXOFF = (n.attribute("TOPXOFF")).toFloat();
                            float TOPYOFF = (n.attribute("TOPYOFF")).toFloat();
                            //GLuint id8 = glGenLists(1);
                            //glNewList(id8, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Ring")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float DR = (n.attribute("DR")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            //GLuint id9 = glGenLists(1);
                            //glNewList(id9, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Ring(DR, R1, Rad);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            Ring(DR, R1, Rad);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("RectangularRing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float DR = (n.attribute("DR")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            //GLuint id10 = glGenLists(1);
                            //glNewList(id10, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            RectangularRing(DR, R1, L, W);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            RectangularRing(DR, R1, L, W);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("EllipticRing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float DR = (n.attribute("DR")).toFloat();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            //GLuint id11 = glGenLists(1);
                            //glNewList(id11, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            EllipticRing(DR, L, W);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            EllipticRing(DR, L, W);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("CircularGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float OR = (n.attribute("OR")).toFloat();
                            float IR = (n.attribute("IR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            //GLuint id12 = glGenLists(1);
                            //glNewList(id12, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            CircularGasket(OR, IR, H, Rad);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            CircularGasket(OR, IR, H, Rad);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("TableGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TR = (n.attribute("TR")).toFloat();
                            float OR = (n.attribute("OR")).toFloat();
                            float IR = (n.attribute("IR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            //GLuint id13 = glGenLists(1);
                            //glNewList(id13, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            TableGasket(TR, OR, IR, H, Rad);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            TableGasket(TR, OR, IR, H, Rad);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("SquareGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float L1 = (n.attribute("L1")).toFloat();
                            float W1 = (n.attribute("W1")).toFloat();
                            float L2 = (n.attribute("L2")).toFloat();
                            float W2 = (n.attribute("W2")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float CT = (n.attribute("CT")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            float CL = (n.attribute("CL")).toFloat();
                            //GLuint id14 = glGenLists(1);
                            //glNewList(id14, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("PorcelainBushing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float H = (n.attribute("H")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float R2 = (n.attribute("R1")).toFloat();
                            float R3 = (n.attribute("R2")).toFloat();
                            float N = (n.attribute("N")).toFloat();
                            //GLuint id15 = glGenLists(1);
                            //glNewList(id15, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Insulator")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float N = (n.attribute("N")).toFloat();
                            float D = (n.attribute("D")).toFloat();
                            float N1 = (n.attribute("N1")).toFloat();
                            float H1 = (n.attribute("H1")).toFloat();
                            float R1 = (n.attribute("R1")).toFloat();
                            float R2 = (n.attribute("R2")).toFloat();
                            float R3 = (n.attribute("R")).toFloat();
                            //float FL = (n.attribute("FL")).toFloat();
                            //float AL = (n.attribute("AL")).toFloat();
                            //float LN = (n.attribute("LN")).toFloat();
                            //GLuint id17 = glGenLists(1);
                            //glNewList(id17, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Insulator(N, D, N1, H1, R1, R2, R3);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(R, G, B, A);
                            Insulator(N, D, N1, H1, R1, R2, R3);
                            glPopMatrix();
                            //glEndList();
                        }
                    }
                }
            }
            glEndList();
        }
    }

    //if (!glIsList((GLuint)(1))) {
    //    glNewList(1, GL_COMPILE);
    //    glPushMatrix();
    //    // glColor3f(1,0,0);
    //    Cuboid(1.8, 1.8, 1.8);
    //    glPopMatrix();
    //    glEndList();
    //}
    //if (!glIsList((GLuint)(2))) {
    //    glNewList(2, GL_COMPILE);
    //    glPushMatrix();
    //    //glColor3f(1, 0, 0);
    //    glTranslatef(0, 1.2, 0);
    //    Sphere(1.2);
    //    glPopMatrix();
    //    glEndList();
    //}
}
//创建显示列表1：用于选中.ID号：gimModFileList.size()+1 - 
void myopengl::CreateDisplayLists2()
{
    for (int i = 0; i < gimModFileList.size(); i++) {
        //qDebug() <<111111<< endl;
        //qDebug() << ModFileList.size() << endl;
        if (!glIsList((GLuint)(gimModFileList.size() + 1 + i))) {
            glNewList(gimModFileList.size() + 1 + i, GL_COMPILE);
            ModFile modFile = gimModFileList[i];
            //glLoadName(i + 1);
            for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
                if (!simpleEntity.getGraphicElement().isNull()) {
                    if (simpleEntity.getVisible() == ("true")) {
                        if (simpleEntity.getGraphicElement().nodeName() == ("Cuboid")) {
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                                //qDebug() << mat[i] << endl;
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            /*GLuint id1 = glGenLists(1);
                            glNewList(id1, GL_COMPILE);*/
                            //绘制线框
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Cuboid(L, W, H);
                            glPopMatrix();
                            //绘制实体
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            Cuboid(L, W, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Sphere")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float R1 = (n.attribute("R")).toFloat();
                            //GLuint id2 = glGenLists(1);
                            //glNewList(id2, GL_COMPILE);
                            //绘制线框
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Sphere(R1);
                            glPopMatrix();
                            //绘制实体
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            Sphere(R1);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("RotationalEllipsoid")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float LR = (n.attribute("LR")).toFloat();
                            float WR = (n.attribute("WR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id3 = glGenLists(1);
                            //glNewList(id3, GL_COMPILE);
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            RotationalEllipsoid(LR, WR, H);
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            RotationalEllipsoid(LR, WR, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Table")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TL1 = (n.attribute("TL1")).toFloat();
                            float TL2 = (n.attribute("TL2")).toFloat();
                            float LL1 = (n.attribute("LL1")).toFloat();
                            float LL2 = (n.attribute("LL2")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id4 = glGenLists(1);
                            //glNewList(id4, GL_COMPILE);
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Table(TL1, TL2, LL1, LL2, H);
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            Table(TL1, TL2, LL1, LL2, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("OffsetRectangularTable")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TL = (n.attribute("TL")).toFloat();
                            float TW = (n.attribute("TW")).toFloat();
                            float LL = (n.attribute("LL")).toFloat();
                            float LW = (n.attribute("LW")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float XOFF = (n.attribute("XOFF")).toFloat();
                            float YOFF = (n.attribute("YOFF")).toFloat();
                            //GLuint id5 = glGenLists(1);
                            //glNewList(id5, GL_COMPILE);
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Cylinder")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float R1 = (n.attribute("R")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id6 = glGenLists(1);
                            //glNewList(id6, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Cylinder(R1, H);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            Cylinder(R1, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("TruncatedCone")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TR = (n.attribute("TR")).toFloat();
                            float BR = (n.attribute("BR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            //GLuint id7 = glGenLists(1);
                            //glNewList(id7, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            TruncatedCone(TR, BR, H);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            TruncatedCone(TR, BR, H);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("EccentricTruncatedCone")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TR = (n.attribute("TR")).toFloat();
                            float BR = (n.attribute("BR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float TOPXOFF = (n.attribute("TOPXOFF")).toFloat();
                            float TOPYOFF = (n.attribute("TOPYOFF")).toFloat();
                            //GLuint id8 = glGenLists(1);
                            //glNewList(id8, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Ring")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float DR = (n.attribute("DR")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            //GLuint id9 = glGenLists(1);
                            //glNewList(id9, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Ring(DR, R1, Rad);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            Ring(DR, R1, Rad);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("RectangularRing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float DR = (n.attribute("DR")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            //GLuint id10 = glGenLists(1);
                            //glNewList(id10, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            RectangularRing(DR, R1, L, W);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            RectangularRing(DR, R1, L, W);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("EllipticRing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float DR = (n.attribute("DR")).toFloat();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            //GLuint id11 = glGenLists(1);
                            //glNewList(id11, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            EllipticRing(DR, L, W);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            EllipticRing(DR, L, W);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("CircularGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float OR = (n.attribute("OR")).toFloat();
                            float IR = (n.attribute("IR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            //GLuint id12 = glGenLists(1);
                            //glNewList(id12, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            CircularGasket(OR, IR, H, Rad);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            CircularGasket(OR, IR, H, Rad);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("TableGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float TR = (n.attribute("TR")).toFloat();
                            float OR = (n.attribute("OR")).toFloat();
                            float IR = (n.attribute("IR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            //GLuint id13 = glGenLists(1);
                            //glNewList(id13, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            TableGasket(TR, OR, IR, H, Rad);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            TableGasket(TR, OR, IR, H, Rad);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("SquareGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float L1 = (n.attribute("L1")).toFloat();
                            float W1 = (n.attribute("W1")).toFloat();
                            float L2 = (n.attribute("L2")).toFloat();
                            float W2 = (n.attribute("W2")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float CT = (n.attribute("CT")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            float CL = (n.attribute("CL")).toFloat();
                            //GLuint id14 = glGenLists(1);
                            //glNewList(id14, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("PorcelainBushing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float H = (n.attribute("H")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float R2 = (n.attribute("R1")).toFloat();
                            float R3 = (n.attribute("R2")).toFloat();
                            float N = (n.attribute("N")).toFloat();
                            //GLuint id15 = glGenLists(1);
                            //glNewList(id15, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            //glEndList();
                        }
                        else if (simpleEntity.getGraphicElement().nodeName() == ("Insulator")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float N = (n.attribute("N")).toFloat();
                            float D = (n.attribute("D")).toFloat();
                            float N1 = (n.attribute("N1")).toFloat();
                            float H1 = (n.attribute("H1")).toFloat();
                            float R1 = (n.attribute("R1")).toFloat();
                            float R2 = (n.attribute("R2")).toFloat();
                            float R3 = (n.attribute("R")).toFloat();
                            //float FL = (n.attribute("FL")).toFloat();
                            //float AL = (n.attribute("AL")).toFloat();
                            //float LN = (n.attribute("LN")).toFloat();
                            //GLuint id17 = glGenLists(1);
                            //glNewList(id17, GL_COMPILE);
                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Insulator(N, D, N1, H1, R1, R2, R3);
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
                            Insulator(N, D, N1, H1, R1, R2, R3);
                            glPopMatrix();
                        }
                    }
                }
            }
            glEndList();
        }
    }

    //if (!glIsList((GLuint)(1))) {
    //    glNewList(1, GL_COMPILE);
    //    glPushMatrix();
    //    // glColor3f(1,0,0);
    //    Cuboid(1.8, 1.8, 1.8);
    //    glPopMatrix();
    //    glEndList();
    //}
    //if (!glIsList((GLuint)(2))) {
    //    glNewList(2, GL_COMPILE);
    //    glPushMatrix();
    //    //glColor3f(1, 0, 0);
    //    glTranslatef(0, 1.2, 0);
    //    Sphere(1.2);
    //    glPopMatrix();
    //    glEndList();
    //}
}

//渲染OpenGL场景。每当部件需要更新时都会调用该函数。
void myopengl::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();

    //glGetIntegerv(GL_VIEWPORT, viewport); //获得viewport  
    //qDebug() << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << endl;
    int  renderMode;
    glGetIntegerv(GL_RENDER_MODE, &renderMode);

    if (renderMode != GL_SELECT) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat x = GLfloat(viewport[2]) / GLfloat(viewport[3]);
        projection = glm::perspective(45.0f, x, 0.1f, 100.0f);
        glLoadMatrixf(value_ptr(projection));

        /*glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat x = GLfloat(viewport[2]) / GLfloat(viewport[3]);
        gluPerspective(45.0, x, 0.1, 100.0);*/
    }

    //摄像机 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    modelview = glm::lookAt(camerapos, camerapos + camerafront, cameraup);
    glLoadMatrixf(glm::value_ptr(modelview));

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
    //    cameraPos[0] + cameraFront[0], cameraPos[1] + cameraFront[1], cameraPos[2] + cameraFront[2],
    //    cameraUp[0], cameraUp[1], cameraUp[2]);//摄像机位置、目标位置、上向量
    //qDebug() << cameraPos[0] << " " << cameraPos[1] << " " << cameraPos[2] << endl;
    //qDebug() << cameraFront[0] << " " << cameraFront[1] << " " << cameraFront[2] << endl;

   //gluOrtho2D(-10, 10, -10, 10);//设定在OpenGL可视化范围（左，右，下，上）

    //---------------------绘图代码------------------------------------------------------
    glRotated(m_rtri, 0.0f, 1.0f, 0.0f);
    //glEnable(GL_DEPTH_TEST);//允许深度测试 
    glPushMatrix();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0, 2.0);

    glScalef(ScalefX, ScalefY, ScalefZ);
    //Axis();
    // qDebug() << ModFileList.size() << endl;
     //for (int i = 0;i< gimModFileList.size();i++ ){
     //    //qDebug() <<111111<< endl;
     //    //qDebug() << ModFileList.size() << endl;
     //    ModFile modFile = gimModFileList[i];
     //    glLoadName(i+1);
     //    for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
     //        if (!simpleEntity.getGraphicElement().isNull()) {
     //            if (simpleEntity.getGraphicElement().nodeName() == ("Cuboid")) {
     //                QString  vs = simpleEntity.getTransformMatrix();
     //                QStringList splits = vs.split(",");
     //                float mat[16];
     //                for (int i = 0; i < 16; i++)
     //                {
     //                    mat[i] = (splits[i]).toFloat();
     //                    //qDebug() << mat[i] << endl;
     //                }
     //                ////获取旋转角度
     //                float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //                float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //                float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //                //获取坐标
     //                float X = mat[12];
     //                float Y = mat[13];
     //                float Z = mat[14];
     //                //获取颜色
     //                float R = (float)simpleEntity.getR() / 255;
     //                float G = (float)simpleEntity.getG() / 255;
     //                float B = (float)simpleEntity.getB() / 255;
     //                float A = (float)simpleEntity.getA() / 255;
     //                //获取长宽高
     //                QDomElement n = simpleEntity.getGraphicElement().toElement();
     //                float L = (n.attribute("L")).toFloat();
     //                float W = (n.attribute("W")).toFloat();
     //                float H = (n.attribute("H")).toFloat();
     //                
     //                /*GLuint id1 = glGenLists(1);
     //                glNewList(id1, GL_COMPILE);*/
     //                //绘制线框
     //                glPushMatrix();
     //                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //                glTranslatef(X, Z, Y);
     //                glRotatef(rX, 1, 0, 0);
     //                glRotatef(rY, 0, 1, 0);
     //                glRotatef(rZ, 0, 0, 1);
     //                glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //                Cuboid(L, W, H);
     //                glPopMatrix();
     //                //绘制实体
     //                glPushMatrix();
     //                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //                glTranslatef(X, Z, Y);
     //                glRotatef(rX, 1, 0, 0);
     //                glRotatef(rY, 0, 1, 0);
     //                glRotatef(rZ, 0, 0, 1);
     //                glColor4f(R, G, B, A);
     //                Cuboid(L, W, H);
     //                glPopMatrix();
     //                //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("Sphere")) {
     //                QDomElement n = simpleEntity.getGraphicElement().toElement();
     //                QString  vs = simpleEntity.getTransformMatrix();
     //                QStringList splits = vs.split(",");
     //                float mat[16];
     //                for (int i = 0; i < 16; i++)
     //                {
     //                    mat[i] = (splits[i]).toFloat();
     //                }
     //                ////获取旋转角度
     //                float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //                float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //                float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //                //获取坐标
     //                float X = mat[12];
     //                float Y = mat[13];
     //                float Z = mat[14];
     //                //获取颜色
     //                float R = (float)simpleEntity.getR() / 255;
     //                float G = (float)simpleEntity.getG() / 255;
     //                float B = (float)simpleEntity.getB() / 255;
     //                float A = (float)simpleEntity.getA() / 255;
     //                //获取长宽高
     //                float R1 = (n.attribute("R")).toFloat();
     //                //GLuint id2 = glGenLists(1);
     //                //glNewList(id2, GL_COMPILE);
     //                //绘制线框
     //                glPushMatrix();
     //                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //                glTranslatef(X, Z, Y);
     //                glRotatef(rX, 1, 0, 0);
     //                glRotatef(rY, 0, 1, 0);
     //                glRotatef(rZ, 0, 0, 1);
     //                glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //                Sphere(R1);
     //                glPopMatrix();
     //                //绘制实体
     //                glPushMatrix();
     //                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //                glTranslatef(X, Z, Y);
     //                glRotatef(rX, 1, 0, 0);
     //                glRotatef(rY, 0, 1, 0);
     //                glRotatef(rZ, 0, 0, 1);
     //                glColor4f(R, G, B, A);
     //                Sphere(R1);
     //                glPopMatrix();
     //                //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("RotationalEllipsoid")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float LR = (n.attribute("LR")).toFloat();
     //            float WR = (n.attribute("WR")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            //GLuint id3 = glGenLists(1);
     //            //glNewList(id3, GL_COMPILE);
     //            //
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            RotationalEllipsoid(LR, WR, H);
     //            glPopMatrix();
     //            //
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            RotationalEllipsoid(LR, WR, H);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("Table")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float TL1 = (n.attribute("TL1")).toFloat();
     //            float TL2 = (n.attribute("TL2")).toFloat();
     //            float LL1 = (n.attribute("LL1")).toFloat();
     //            float LL2 = (n.attribute("LL2")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            //GLuint id4 = glGenLists(1);
     //            //glNewList(id4, GL_COMPILE);
     //            //
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            Table(TL1, TL2, LL1, LL2, H);
     //            glPopMatrix();
     //            //
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            Table(TL1, TL2, LL1, LL2, H);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("OffsetRectangularTable")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float TL = (n.attribute("TL")).toFloat();
     //            float TW = (n.attribute("TW")).toFloat();
     //            float LL = (n.attribute("LL")).toFloat();
     //            float LW = (n.attribute("LW")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            float XOFF = (n.attribute("XOFF")).toFloat();
     //            float YOFF = (n.attribute("YOFF")).toFloat();
     //            //GLuint id5 = glGenLists(1);
     //            //glNewList(id5, GL_COMPILE);
     //            //
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
     //            glPopMatrix();
     //            //
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("Cylinder")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float R1 = (n.attribute("R")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            //GLuint id6 = glGenLists(1);
     //            //glNewList(id6, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            Cylinder(R1, H);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            Cylinder(R1, H);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("TruncatedCone")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float TR = (n.attribute("TR")).toFloat();
     //            float BR = (n.attribute("BR")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            //GLuint id7 = glGenLists(1);
     //            //glNewList(id7, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            TruncatedCone(TR, BR, H);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            TruncatedCone(TR, BR, H);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("EccentricTruncatedCone")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float TR = (n.attribute("TR")).toFloat();
     //            float BR = (n.attribute("BR")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            float TOPXOFF = (n.attribute("TOPXOFF")).toFloat();
     //            float TOPYOFF = (n.attribute("TOPYOFF")).toFloat();
     //            //GLuint id8 = glGenLists(1);
     //            //glNewList(id8, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("Ring")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float DR = (n.attribute("DR")).toFloat();
     //            float R1 = (n.attribute("R")).toFloat();
     //            float Rad = (n.attribute("Rad")).toFloat();
     //            //GLuint id9 = glGenLists(1);
     //            //glNewList(id9, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            Ring(DR, R1, Rad);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            Ring(DR, R1, Rad);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("RectangularRing")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float DR = (n.attribute("DR")).toFloat();
     //            float R1 = (n.attribute("R")).toFloat();
     //            float L = (n.attribute("L")).toFloat();
     //            float W = (n.attribute("W")).toFloat();
     //            //GLuint id10 = glGenLists(1);
     //            //glNewList(id10, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            RectangularRing(DR, R1, L, W);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            RectangularRing(DR, R1, L, W);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("EllipticRing")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float DR = (n.attribute("DR")).toFloat();
     //            float L = (n.attribute("L")).toFloat();
     //            float W = (n.attribute("W")).toFloat();
     //            //GLuint id11 = glGenLists(1);
     //            //glNewList(id11, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            EllipticRing(DR, L, W);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            EllipticRing(DR, L, W);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("CircularGasket")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float OR = (n.attribute("OR")).toFloat();
     //            float IR = (n.attribute("IR")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            float Rad = (n.attribute("Rad")).toFloat();
     //            //GLuint id12 = glGenLists(1);
     //            //glNewList(id12, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            CircularGasket(OR, IR, H, Rad);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            CircularGasket(OR, IR, H, Rad);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("TableGasket")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float TR = (n.attribute("TR")).toFloat();
     //            float OR = (n.attribute("OR")).toFloat();
     //            float IR = (n.attribute("IR")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            float Rad = (n.attribute("Rad")).toFloat();
     //            //GLuint id13 = glGenLists(1);
     //            //glNewList(id13, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            TableGasket(TR, OR, IR, H, Rad);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            TableGasket(TR, OR, IR, H, Rad);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("SquareGasket")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float L1 = (n.attribute("L1")).toFloat();
     //            float W1 = (n.attribute("W1")).toFloat();
     //            float L2 = (n.attribute("L2")).toFloat();
     //            float W2 = (n.attribute("W2")).toFloat();
     //            float H = (n.attribute("H")).toFloat();
     //            float CT = (n.attribute("CT")).toFloat();
     //            float Rad = (n.attribute("Rad")).toFloat();
     //            float CL = (n.attribute("CL")).toFloat();
     //            //GLuint id14 = glGenLists(1);
     //            //glNewList(id14, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("PorcelainBushing")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float H = (n.attribute("H")).toFloat();
     //            float R1 = (n.attribute("R")).toFloat();
     //            float R2 = (n.attribute("R1")).toFloat();
     //            float R3 = (n.attribute("R2")).toFloat();
     //            float N = (n.attribute("N")).toFloat();
     //            //GLuint id15 = glGenLists(1);
     //            //glNewList(id15, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            PorcelainBushing(H, R1, R2, R3, N);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            PorcelainBushing(H, R1, R2, R3, N);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //            else if (simpleEntity.getGraphicElement().nodeName() == ("Insulator")) {
     //            QDomElement n = simpleEntity.getGraphicElement().toElement();
     //            QString  vs = simpleEntity.getTransformMatrix();
     //            QStringList splits = vs.split(",");
     //            float mat[16];
     //            for (int i = 0; i < 16; i++)
     //            {
     //                mat[i] = (splits[i]).toFloat();
     //            }
     //            ////获取旋转角度
     //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
     //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
     //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
     //            //获取坐标
     //            float X = mat[12];
     //            float Y = mat[13];
     //            float Z = mat[14];
     //            //获取颜色
     //            float R = (float)simpleEntity.getR() / 255;
     //            float G = (float)simpleEntity.getG() / 255;
     //            float B = (float)simpleEntity.getB() / 255;
     //            float A = (float)simpleEntity.getA() / 255;
     //            //获取长宽高
     //            float N = (n.attribute("N")).toFloat();
     //            float D = (n.attribute("D")).toFloat();
     //            float N1 = (n.attribute("N1")).toFloat();
     //            float H1 = (n.attribute("H1")).toFloat();
     //            float R1 = (n.attribute("R1")).toFloat();
     //            float R2 = (n.attribute("R2")).toFloat();
     //            float R3 = (n.attribute("R")).toFloat();
     //            //float FL = (n.attribute("FL")).toFloat();
     //            //float AL = (n.attribute("AL")).toFloat();
     //            //float LN = (n.attribute("LN")).toFloat();
     //            //GLuint id17 = glGenLists(1);
     //            //glNewList(id17, GL_COMPILE);
     //            //1
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
     //            Insulator(N, D, N1, H1, R1, R2, R3);
     //            glPopMatrix();
     //            //2
     //            glPushMatrix();
     //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     //            glTranslatef(X, Z, Y);
     //            glRotatef(rX, 1, 0, 0);
     //            glRotatef(rY, 0, 1, 0);
     //            glRotatef(rZ, 0, 0, 1);
     //            glColor4f(R, G, B, A);
     //            Insulator(N, D, N1, H1, R1, R2, R3);
     //            glPopMatrix();
     //            //glEndList();
     //            }
     //        }
     //    }
     //}

    qDebug() << "flag1:" << flag1;
    qDebug() << "flag2:" << flag2;
    /*if (flag1 == 1) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLoadName(1);
        glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glEnd();
    }
    if (flag2 == 1) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glLoadName(2);
        glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glEnd();
    }*/

    CreateDisplayLists1();
    CreateDisplayLists2();
    //qDebug() << "gimmodlist大小:" << gimModFileList.size() << endl;
    if (hits == 0) {
        for (int i = 0; i < gimModFileList.size(); i++) {
            //glLoadName(i + 1);
            glCallList(i + 1);
        }
        drawboolEntity();
    }
    else if (hits > 0) {
        QString devname;
        if (name <= gimModFileList.size()) {
            devname = gimModFileList[name - 1].getObjFileName();//设备dev文件名
        }
        else {
            devname = gimModFileList[name - gimModFileList.size() - 1].getObjFileName();//设备dev文件名
        }
        //qDebug() << "选中的devname：" << devname << endl;
        for (int i = 0; i < gimModFileList.size(); i++) {
            if (QString::compare(gimModFileList[i].getObjFileName(), devname) == 0) {//选中
                glCallList(gimModFileList.size() + 1 + i);
            }
            else {//没选中
                glCallList(i + 1); 
            }
        }
        drawboolEntity2(devname);
    }
    //    //for (int i = 1; i < gimModFileList.size() + 1; i++) {
    //    //    if (i==name) {//选中
    //    //        ModFile modFile = gimModFileList[i-1];
    //    //        for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
    //    //            if (!simpleEntity.getGraphicElement().isNull()) {
    //    //                if (simpleEntity.getGraphicElement().nodeName() == ("Cuboid")) {
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                        //qDebug() << mat[i] << endl;
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    float L = (n.attribute("L")).toFloat();
    //    //                    float W = (n.attribute("W")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    /*GLuint id1 = glGenLists(1);
    //    //                    glNewList(id1, GL_COMPILE);*/
    //    //                    //绘制线框
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    Cuboid(L, W, H);
    //    //                    glPopMatrix();
    //    //                    //绘制实体
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    Cuboid(L, W, H);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("Sphere")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float R1 = (n.attribute("R")).toFloat();
    //    //                    //GLuint id2 = glGenLists(1);
    //    //                    //glNewList(id2, GL_COMPILE);
    //    //                    //绘制线框
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    Sphere(R1);
    //    //                    glPopMatrix();
    //    //                    //绘制实体
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    Sphere(R1);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("RotationalEllipsoid")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float LR = (n.attribute("LR")).toFloat();
    //    //                    float WR = (n.attribute("WR")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    //GLuint id3 = glGenLists(1);
    //    //                    //glNewList(id3, GL_COMPILE);
    //    //                    //
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    RotationalEllipsoid(LR, WR, H);
    //    //                    glPopMatrix();
    //    //                    //
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    RotationalEllipsoid(LR, WR, H);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("Table")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float TL1 = (n.attribute("TL1")).toFloat();
    //    //                    float TL2 = (n.attribute("TL2")).toFloat();
    //    //                    float LL1 = (n.attribute("LL1")).toFloat();
    //    //                    float LL2 = (n.attribute("LL2")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    //GLuint id4 = glGenLists(1);
    //    //                    //glNewList(id4, GL_COMPILE);
    //    //                    //
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    Table(TL1, TL2, LL1, LL2, H);
    //    //                    glPopMatrix();
    //    //                    //
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    Table(TL1, TL2, LL1, LL2, H);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("OffsetRectangularTable")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float TL = (n.attribute("TL")).toFloat();
    //    //                    float TW = (n.attribute("TW")).toFloat();
    //    //                    float LL = (n.attribute("LL")).toFloat();
    //    //                    float LW = (n.attribute("LW")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    float XOFF = (n.attribute("XOFF")).toFloat();
    //    //                    float YOFF = (n.attribute("YOFF")).toFloat();
    //    //                    //GLuint id5 = glGenLists(1);
    //    //                    //glNewList(id5, GL_COMPILE);
    //    //                    //
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
    //    //                    glPopMatrix();
    //    //                    //
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("Cylinder")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float R1 = (n.attribute("R")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    //GLuint id6 = glGenLists(1);
    //    //                    //glNewList(id6, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    Cylinder(R1, H);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    Cylinder(R1, H);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("TruncatedCone")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float TR = (n.attribute("TR")).toFloat();
    //    //                    float BR = (n.attribute("BR")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    //GLuint id7 = glGenLists(1);
    //    //                    //glNewList(id7, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    TruncatedCone(TR, BR, H);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    TruncatedCone(TR, BR, H);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("EccentricTruncatedCone")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float TR = (n.attribute("TR")).toFloat();
    //    //                    float BR = (n.attribute("BR")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    float TOPXOFF = (n.attribute("TOPXOFF")).toFloat();
    //    //                    float TOPYOFF = (n.attribute("TOPYOFF")).toFloat();
    //    //                    //GLuint id8 = glGenLists(1);
    //    //                    //glNewList(id8, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("Ring")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float DR = (n.attribute("DR")).toFloat();
    //    //                    float R1 = (n.attribute("R")).toFloat();
    //    //                    float Rad = (n.attribute("Rad")).toFloat();
    //    //                    //GLuint id9 = glGenLists(1);
    //    //                    //glNewList(id9, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    Ring(DR, R1, Rad);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    Ring(DR, R1, Rad);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("RectangularRing")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float DR = (n.attribute("DR")).toFloat();
    //    //                    float R1 = (n.attribute("R")).toFloat();
    //    //                    float L = (n.attribute("L")).toFloat();
    //    //                    float W = (n.attribute("W")).toFloat();
    //    //                    //GLuint id10 = glGenLists(1);
    //    //                    //glNewList(id10, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    RectangularRing(DR, R1, L, W);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    RectangularRing(DR, R1, L, W);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("EllipticRing")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float DR = (n.attribute("DR")).toFloat();
    //    //                    float L = (n.attribute("L")).toFloat();
    //    //                    float W = (n.attribute("W")).toFloat();
    //    //                    //GLuint id11 = glGenLists(1);
    //    //                    //glNewList(id11, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    EllipticRing(DR, L, W);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    EllipticRing(DR, L, W);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("CircularGasket")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float OR = (n.attribute("OR")).toFloat();
    //    //                    float IR = (n.attribute("IR")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    float Rad = (n.attribute("Rad")).toFloat();
    //    //                    //GLuint id12 = glGenLists(1);
    //    //                    //glNewList(id12, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    CircularGasket(OR, IR, H, Rad);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    CircularGasket(OR, IR, H, Rad);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("TableGasket")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float TR = (n.attribute("TR")).toFloat();
    //    //                    float OR = (n.attribute("OR")).toFloat();
    //    //                    float IR = (n.attribute("IR")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    float Rad = (n.attribute("Rad")).toFloat();
    //    //                    //GLuint id13 = glGenLists(1);
    //    //                    //glNewList(id13, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    TableGasket(TR, OR, IR, H, Rad);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    TableGasket(TR, OR, IR, H, Rad);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("SquareGasket")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float L1 = (n.attribute("L1")).toFloat();
    //    //                    float W1 = (n.attribute("W1")).toFloat();
    //    //                    float L2 = (n.attribute("L2")).toFloat();
    //    //                    float W2 = (n.attribute("W2")).toFloat();
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    float CT = (n.attribute("CT")).toFloat();
    //    //                    float Rad = (n.attribute("Rad")).toFloat();
    //    //                    float CL = (n.attribute("CL")).toFloat();
    //    //                    //GLuint id14 = glGenLists(1);
    //    //                    //glNewList(id14, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("PorcelainBushing")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float H = (n.attribute("H")).toFloat();
    //    //                    float R1 = (n.attribute("R")).toFloat();
    //    //                    float R2 = (n.attribute("R1")).toFloat();
    //    //                    float R3 = (n.attribute("R2")).toFloat();
    //    //                    float N = (n.attribute("N")).toFloat();
    //    //                    //GLuint id15 = glGenLists(1);
    //    //                    //glNewList(id15, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    PorcelainBushing(H, R1, R2, R3, N);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    PorcelainBushing(H, R1, R2, R3, N);
    //    //                    glPopMatrix();
    //    //                    //glEndList();
    //    //                }
    //    //                else if (simpleEntity.getGraphicElement().nodeName() == ("Insulator")) {
    //    //                    QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //                    QString  vs = simpleEntity.getTransformMatrix();
    //    //                    QStringList splits = vs.split(",");
    //    //                    float mat[16];
    //    //                    for (int i = 0; i < 16; i++)
    //    //                    {
    //    //                        mat[i] = (splits[i]).toFloat();
    //    //                    }
    //    //                    ////获取旋转角度
    //    //                    float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //                    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //                    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //                    //获取坐标
    //    //                    float X = mat[12];
    //    //                    float Y = mat[13];
    //    //                    float Z = mat[14];
    //    //                    ////获取颜色
    //    //                    //float R = (float)simpleEntity.getR() / 255;
    //    //                    //float G = (float)simpleEntity.getG() / 255;
    //    //                    //float B = (float)simpleEntity.getB() / 255;
    //    //                    //float A = (float)simpleEntity.getA() / 255;
    //    //                    //获取长宽高
    //    //                    float N = (n.attribute("N")).toFloat();
    //    //                    float D = (n.attribute("D")).toFloat();
    //    //                    float N1 = (n.attribute("N1")).toFloat();
    //    //                    float H1 = (n.attribute("H1")).toFloat();
    //    //                    float R1 = (n.attribute("R1")).toFloat();
    //    //                    float R2 = (n.attribute("R2")).toFloat();
    //    //                    float R3 = (n.attribute("R")).toFloat();
    //    //                    //float FL = (n.attribute("FL")).toFloat();
    //    //                    //float AL = (n.attribute("AL")).toFloat();
    //    //                    //float LN = (n.attribute("LN")).toFloat();
    //    //                    //GLuint id17 = glGenLists(1);
    //    //                    //glNewList(id17, GL_COMPILE);
    //    //                    //1
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //                    Insulator(N, D, N1, H1, R1, R2, R3);
    //    //                    glPopMatrix();
    //    //                    //2
    //    //                    glPushMatrix();
    //    //                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //                    glTranslatef(X, Z, Y);
    //    //                    glRotatef(rX, 1, 0, 0);
    //    //                    glRotatef(rY, 0, 1, 0);
    //    //                    glRotatef(rZ, 0, 0, 1);
    //    //                    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //                    Insulator(N, D, N1, H1, R1, R2, R3);
    //    //                    glPopMatrix();
    //    //                }
    //    //            }
    //    //        }
    //    //    }
    //    //    else {//没选中
    //    //        glCallList(i);
    //    //    }

    //    //}
    //    ////1、先画其余设备
    //    //for (int i = 1; i < gimModFileList.size() + 1; i++) {
    //    //    if (i == name) {
    //    //        continue;
    //    //    }
    //    //    glCallList(i);
    //    //    
    //    //}
    //    ////2、再画拾取的设备
    //    //ModFile modFile = gimModFileList[name - 1];
    //    //for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
    //    //    if (!simpleEntity.getGraphicElement().isNull()) {
    //    //        if (simpleEntity.getGraphicElement().nodeName() == ("Cuboid")) {
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //                //qDebug() << mat[i] << endl;
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            float L = (n.attribute("L")).toFloat();
    //    //            float W = (n.attribute("W")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            /*GLuint id1 = glGenLists(1);
    //    //            glNewList(id1, GL_COMPILE);*/
    //    //            //绘制线框
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            Cuboid(L, W, H);
    //    //            glPopMatrix();
    //    //            //绘制实体
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            Cuboid(L, W, H);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("Sphere")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float R1 = (n.attribute("R")).toFloat();
    //    //            //GLuint id2 = glGenLists(1);
    //    //            //glNewList(id2, GL_COMPILE);
    //    //            //绘制线框
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            Sphere(R1);
    //    //            glPopMatrix();
    //    //            //绘制实体
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            Sphere(R1);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("RotationalEllipsoid")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float LR = (n.attribute("LR")).toFloat();
    //    //            float WR = (n.attribute("WR")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            //GLuint id3 = glGenLists(1);
    //    //            //glNewList(id3, GL_COMPILE);
    //    //            //
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            RotationalEllipsoid(LR, WR, H);
    //    //            glPopMatrix();
    //    //            //
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            RotationalEllipsoid(LR, WR, H);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("Table")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float TL1 = (n.attribute("TL1")).toFloat();
    //    //            float TL2 = (n.attribute("TL2")).toFloat();
    //    //            float LL1 = (n.attribute("LL1")).toFloat();
    //    //            float LL2 = (n.attribute("LL2")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            //GLuint id4 = glGenLists(1);
    //    //            //glNewList(id4, GL_COMPILE);
    //    //            //
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            Table(TL1, TL2, LL1, LL2, H);
    //    //            glPopMatrix();
    //    //            //
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            Table(TL1, TL2, LL1, LL2, H);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("OffsetRectangularTable")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float TL = (n.attribute("TL")).toFloat();
    //    //            float TW = (n.attribute("TW")).toFloat();
    //    //            float LL = (n.attribute("LL")).toFloat();
    //    //            float LW = (n.attribute("LW")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            float XOFF = (n.attribute("XOFF")).toFloat();
    //    //            float YOFF = (n.attribute("YOFF")).toFloat();
    //    //            //GLuint id5 = glGenLists(1);
    //    //            //glNewList(id5, GL_COMPILE);
    //    //            //
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
    //    //            glPopMatrix();
    //    //            //
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("Cylinder")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float R1 = (n.attribute("R")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            //GLuint id6 = glGenLists(1);
    //    //            //glNewList(id6, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            Cylinder(R1, H);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            Cylinder(R1, H);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("TruncatedCone")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float TR = (n.attribute("TR")).toFloat();
    //    //            float BR = (n.attribute("BR")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            //GLuint id7 = glGenLists(1);
    //    //            //glNewList(id7, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            TruncatedCone(TR, BR, H);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            TruncatedCone(TR, BR, H);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("EccentricTruncatedCone")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float TR = (n.attribute("TR")).toFloat();
    //    //            float BR = (n.attribute("BR")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            float TOPXOFF = (n.attribute("TOPXOFF")).toFloat();
    //    //            float TOPYOFF = (n.attribute("TOPYOFF")).toFloat();
    //    //            //GLuint id8 = glGenLists(1);
    //    //            //glNewList(id8, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("Ring")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float DR = (n.attribute("DR")).toFloat();
    //    //            float R1 = (n.attribute("R")).toFloat();
    //    //            float Rad = (n.attribute("Rad")).toFloat();
    //    //            //GLuint id9 = glGenLists(1);
    //    //            //glNewList(id9, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            Ring(DR, R1, Rad);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            Ring(DR, R1, Rad);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("RectangularRing")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float DR = (n.attribute("DR")).toFloat();
    //    //            float R1 = (n.attribute("R")).toFloat();
    //    //            float L = (n.attribute("L")).toFloat();
    //    //            float W = (n.attribute("W")).toFloat();
    //    //            //GLuint id10 = glGenLists(1);
    //    //            //glNewList(id10, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            RectangularRing(DR, R1, L, W);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            RectangularRing(DR, R1, L, W);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("EllipticRing")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float DR = (n.attribute("DR")).toFloat();
    //    //            float L = (n.attribute("L")).toFloat();
    //    //            float W = (n.attribute("W")).toFloat();
    //    //            //GLuint id11 = glGenLists(1);
    //    //            //glNewList(id11, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            EllipticRing(DR, L, W);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            EllipticRing(DR, L, W);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("CircularGasket")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float OR = (n.attribute("OR")).toFloat();
    //    //            float IR = (n.attribute("IR")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            float Rad = (n.attribute("Rad")).toFloat();
    //    //            //GLuint id12 = glGenLists(1);
    //    //            //glNewList(id12, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            CircularGasket(OR, IR, H, Rad);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            CircularGasket(OR, IR, H, Rad);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("TableGasket")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float TR = (n.attribute("TR")).toFloat();
    //    //            float OR = (n.attribute("OR")).toFloat();
    //    //            float IR = (n.attribute("IR")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            float Rad = (n.attribute("Rad")).toFloat();
    //    //            //GLuint id13 = glGenLists(1);
    //    //            //glNewList(id13, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            TableGasket(TR, OR, IR, H, Rad);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            TableGasket(TR, OR, IR, H, Rad);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("SquareGasket")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float L1 = (n.attribute("L1")).toFloat();
    //    //            float W1 = (n.attribute("W1")).toFloat();
    //    //            float L2 = (n.attribute("L2")).toFloat();
    //    //            float W2 = (n.attribute("W2")).toFloat();
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            float CT = (n.attribute("CT")).toFloat();
    //    //            float Rad = (n.attribute("Rad")).toFloat();
    //    //            float CL = (n.attribute("CL")).toFloat();
    //    //            //GLuint id14 = glGenLists(1);
    //    //            //glNewList(id14, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("PorcelainBushing")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float H = (n.attribute("H")).toFloat();
    //    //            float R1 = (n.attribute("R")).toFloat();
    //    //            float R2 = (n.attribute("R1")).toFloat();
    //    //            float R3 = (n.attribute("R2")).toFloat();
    //    //            float N = (n.attribute("N")).toFloat();
    //    //            //GLuint id15 = glGenLists(1);
    //    //            //glNewList(id15, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            PorcelainBushing(H, R1, R2, R3, N);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            PorcelainBushing(H, R1, R2, R3, N);
    //    //            glPopMatrix();
    //    //            //glEndList();
    //    //        }
    //    //        else if (simpleEntity.getGraphicElement().nodeName() == ("Insulator")) {
    //    //            QDomElement n = simpleEntity.getGraphicElement().toElement();
    //    //            QString  vs = simpleEntity.getTransformMatrix();
    //    //            QStringList splits = vs.split(",");
    //    //            float mat[16];
    //    //            for (int i = 0; i < 16; i++)
    //    //            {
    //    //                mat[i] = (splits[i]).toFloat();
    //    //            }
    //    //            ////获取旋转角度
    //    //            float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //    //            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //    //            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //    //            //获取坐标
    //    //            float X = mat[12];
    //    //            float Y = mat[13];
    //    //            float Z = mat[14];
    //    //            ////获取颜色
    //    //            //float R = (float)simpleEntity.getR() / 255;
    //    //            //float G = (float)simpleEntity.getG() / 255;
    //    //            //float B = (float)simpleEntity.getB() / 255;
    //    //            //float A = (float)simpleEntity.getA() / 255;
    //    //            //获取长宽高
    //    //            float N = (n.attribute("N")).toFloat();
    //    //            float D = (n.attribute("D")).toFloat();
    //    //            float N1 = (n.attribute("N1")).toFloat();
    //    //            float H1 = (n.attribute("H1")).toFloat();
    //    //            float R1 = (n.attribute("R1")).toFloat();
    //    //            float R2 = (n.attribute("R2")).toFloat();
    //    //            float R3 = (n.attribute("R")).toFloat();
    //    //            //float FL = (n.attribute("FL")).toFloat();
    //    //            //float AL = (n.attribute("AL")).toFloat();
    //    //            //float LN = (n.attribute("LN")).toFloat();
    //    //            //GLuint id17 = glGenLists(1);
    //    //            //glNewList(id17, GL_COMPILE);
    //    //            //1
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
    //    //            Insulator(N, D, N1, H1, R1, R2, R3);
    //    //            glPopMatrix();
    //    //            //2
    //    //            glPushMatrix();
    //    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    //            glTranslatef(X, Z, Y);
    //    //            glRotatef(rX, 1, 0, 0);
    //    //            glRotatef(rY, 0, 1, 0);
    //    //            glRotatef(rZ, 0, 0, 1);
    //    //            glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    //    //            Insulator(N, D, N1, H1, R1, R2, R3);
    //    //            glPopMatrix();
    //    //        }
    //    //    }
    //    //}
    //}


    //glCallList(1);
    //glCallList(2);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(-1.0, 2.0);
    //glEnable(GL_DEPTH_TEST);
    ////glCallList(1);
    //primitives.push_back(new OpenCSG::DisplayListPrimitive(1, OpenCSG::Intersection, 1));
    //// glEnable(GL_DEPTH_TEST);
    //primitives.push_back(new OpenCSG::DisplayListPrimitive(2, OpenCSG::Subtraction, 1));
    ////qDebug() << primitives.size() << endl;
    //OpenCSG::render(primitives, OpenCSG::Algorithm::Goldfeather, OpenCSG::DepthComplexityAlgorithm::NoDepthComplexitySampling);
    ////qDebug() << primitives.size() << endl;
    //glDepthFunc(GL_EQUAL);
    ////box->render();
    ////sphere->render();
    //for (int i = 0; i != primitives.size(); ++i) {
    //    (primitives[i])->render();
    //}
    //glDepthFunc(GL_LESS);
    //glDisable(GL_POLYGON_OFFSET_FILL);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glPopMatrix();
    //glDisable(GL_DEPTH_TEST);
    glFlush();//刷新
}

void myopengl::keyPressEvent(QKeyEvent* event)
{
    ////右向量
    //cameraRight[0] = cameraFront[1] * cameraUp[2] - cameraFront[2] * cameraUp[1];
    //cameraRight[1] = cameraFront[2] * cameraUp[0] - cameraFront[0] * cameraUp[2];
    //cameraRight[2] = cameraFront[0] * cameraUp[1] - cameraFront[1] * cameraUp[0];
    ////标准化
    //normalizedRight[0] = cameraRight[0] / sqrt(pow(cameraRight[0], 2) + pow(cameraRight[1], 2) + pow(cameraRight[2], 2));
    //normalizedRight[1] = cameraRight[1] / sqrt(pow(cameraRight[0], 2) + pow(cameraRight[1], 2) + pow(cameraRight[2], 2));
    //normalizedRight[2] = cameraRight[2] / sqrt(pow(cameraRight[0], 2) + pow(cameraRight[1], 2) + pow(cameraRight[2], 2));
    if (event->key() == Qt::Key_A)
    {
        camerapos -= glm::normalize(glm::cross(camerafront, cameraup)) * cameraSpeed;

        /* cameraPos[0] -= normalizedRight[0] * speed;
         cameraPos[1] -= normalizedRight[1] * speed;
         cameraPos[2] -= normalizedRight[2] * speed;*/
    }
    else if (event->key() == Qt::Key_D)
    {
        camerapos += glm::normalize(glm::cross(camerafront, cameraup)) * cameraSpeed;

        /* cameraPos[0] += normalizedRight[0] * speed;
         cameraPos[1] += normalizedRight[1] * speed;
         cameraPos[2] += normalizedRight[2] * speed;*/
    }
    else if (event->key() == Qt::Key_W)
    {
        camerapos += cameraSpeed * cameraup;

        /*cameraPos[0] += cameraUp[0] * speed;
        cameraPos[1] += cameraUp[1] * speed;
        cameraPos[2] += cameraUp[2] * speed;*/
    }
    else if (event->key() == Qt::Key_S)
    {
        camerapos -= cameraSpeed * cameraup;

        /*cameraPos[0] -= cameraUp[0] * speed;
        cameraPos[1] -= cameraUp[1] * speed;
        cameraPos[2] -= cameraUp[2] * speed;*/
    }
    else if (event->key() == Qt::Key_Q) {
        m_rtri += 10;
    }
    else if (event->key() == Qt::Key_Left) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        yaw -= 1;
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camerafront = glm::normalize(front);

        //front[0] = cos(pitch * PI / 180) * cos(yaw * PI / 180);
        //front[1] = sin(pitch * PI / 180);
        //front[2] = cos(pitch * PI / 180) * sin(yaw * PI / 180);
        ////标准化
        //cameraFront[0] = front[0] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[1] = front[1] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[2] = front[2] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
    }
    else if (event->key() == Qt::Key_Right) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        yaw += 1;
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camerafront = glm::normalize(front);

        //front[0] = cos(pitch * PI / 180) * cos(yaw * PI / 180);
        //front[1] = sin(pitch * PI / 180);
        //front[2] = cos(pitch * PI / 180) * sin(yaw * PI / 180);
        ////标准化
        //cameraFront[0] = front[0] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[1] = front[1] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[2] = front[2] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
    }
    else if (event->key() == Qt::Key_Up) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        pitch -= 1;
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camerafront = glm::normalize(front);

        //front[0] = cos(pitch * PI / 180) * cos(yaw * PI / 180);
        //front[1] = sin(pitch * PI / 180);
        //front[2] = cos(pitch * PI / 180) * sin(yaw * PI / 180);
        ////标准化
        //cameraFront[0] = front[0] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[1] = front[1] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[2] = front[2] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
    }
    else if (event->key() == Qt::Key_Down) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        pitch += 1;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camerafront = glm::normalize(front);

        //front[0] = cos(pitch * PI / 180) * cos(yaw * PI / 180);
        //front[1] = sin(pitch * PI / 180);
        //front[2] = cos(pitch * PI / 180) * sin(yaw * PI / 180);
        ////标准化
        //cameraFront[0] = front[0] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[1] = front[1] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
        //cameraFront[2] = front[2] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
    }
    update();
}
void myopengl::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0) {
        camerapos += cameraSpeed * camerafront;

        /*cameraPos[0] -= speed * cameraFront[0];
        cameraPos[1] -= speed * cameraFront[1];
        cameraPos[2] -= speed * cameraFront[2];*/
    }
    else {
        camerapos -= cameraSpeed * camerafront;

        /*cameraPos[0] += speed * cameraFront[0];
        cameraPos[1] += speed * cameraFront[1];
        cameraPos[2] += speed * cameraFront[2];*/
    }
    //gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
    //    cameraPos[0] + cameraFront[0], cameraPos[1] + cameraFront[1], cameraPos[2] + cameraFront[2],
    //    cameraUp[0], cameraUp[1], cameraUp[2]);//摄像机位置、目标位置、上向量
    update();
}
void myopengl::pick()
{
}
void myopengl::mousePressEvent(QMouseEvent* event)
{
    // Selection buffer
    std::fill(selectBuffer.begin(), selectBuffer.end(), 0);
    glSelectBuffer(selectBufferSize, &selectBuffer[0]);

    // Draw for selection buffer
    glRenderMode(GL_SELECT);

    // Prepare for selection
    glInitNames();
    glPushName(0);

    // Matrix setting
    glMatrixMode(GL_PROJECTION);//进入投影阶段准备拾取
    glPushMatrix();//保存以前的投影矩阵
    glLoadIdentity();//载入单位矩阵
    qDebug() << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << endl;
    gluPickMatrix(event->x(), viewport[3] - event->y(), 1.0, 1.0, viewport);
    const   float  aspect = static_cast <float>(viewport[2]) / viewport[3];
    gluPerspective(45.0, aspect, 1.0, 1000.0);

    // Draw
    paintGL();

    // Reset matrix setting
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();// 返回正常的投影变换   
    glFlush();
    // Revert render mode
    hits = glRenderMode(GL_RENDER);

    // Show selection
    printf("%d hits\n", hits);
    GimReader gim;
    if (hits > 0)
    {
        int  id = 0;
        for (int i = 0; i < hits; i++)
        {
            printf("Level: %u\n", selectBuffer[id + 0]);
            printf("  Min: %f\n", (double)selectBuffer[id + 1] / UINT_MAX);
            printf("  Max: %f\n", (double)selectBuffer[id + 2] / UINT_MAX);
            printf("   ID: %u\n", selectBuffer[id + 3]);
            name = selectBuffer[id + 3];
            id += 4;

            gim.read();
        }
    }
    else {
        name = 0;
    }


    ispressed = true;
    prepoint = event->pos();


    // 如果是鼠标左键按下
    /*if (event->button() == Qt::LeftButton)
    {
        lastx = event->pos().x();
        lasty = event->pos().y();

        qDebug() << "x单击1:" << lastx << endl;
        qDebug() << "y单击1:" << lasty << endl;
    }*/
    //qDebug() << "x单击1:" << event->pos().x() << endl;
    //qDebug() << "y单击1:" << event->pos().y() << endl;
    //qDebug() << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << endl;

}
void myopengl::mouseMoveEvent(QMouseEvent* event)
{
    float dx = event->pos().x() - prepoint.x();
    float dy = prepoint.y() - event->pos().y();

    //m_camera.setViewByMouse();
    // 
    //// 这里必须使用buttons()
    //if (event->buttons() & Qt::LeftButton) //进行的按位与
    //{
    //    firstxpos = event->pos().x();//得到鼠标像素位置
    //    firstypos = event->pos().y();
    //    qDebug() << "x拖拽:" << firstxpos << endl;
    //    qDebug() << "y拖拽:" << firstypos << endl;
    //}
    ///*qDebug() << "x单击2:" << lastx << endl;
    //qDebug() << "y单击2:" << lasty << endl;*/
    //
    ///*qDebug() << "x拖动:" << firstxpos << endl;
    //qDebug() << "y拖动:" << firstypos << endl;*/
    //xoffset = firstxpos - lastx;//计算当前帧和上一帧鼠标位置的偏移量
    //yoffset = lasty - firstypos;
    //qDebug() << "xoffset:" << xoffset << endl;
    //qDebug()<< "yoffset:" << yoffset << endl;
    //lastx = firstxpos;
    //lasty = firstypos;

    float sensitivity = 0.05f;//灵敏度
    dx *= sensitivity;//偏移量x*灵敏度
    dy *= sensitivity;//偏移量y*灵敏度

    yaw += dx;
    pitch += dy;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camerafront = glm::normalize(front);
    //front[0] = cos(pitch * PI / 180) * cos(yaw * PI / 180);
    //front[1] = sin(pitch * PI / 180);
    //front[2] = cos(pitch * PI / 180) * sin(yaw * PI / 180);
    ////标准化
    //cameraFront[0] = front[0] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
    //cameraFront[1] = front[1] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));
    //cameraFront[2] = front[2] / sqrt(pow(front[0], 2) + pow(front[1], 2) + pow(front[2], 2));

    //gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
    //    cameraPos[0] + cameraFront[0], cameraPos[1] + cameraFront[1], cameraPos[2] + cameraFront[2],
    //    cameraUp[0], cameraUp[1], cameraUp[2]);//摄像机位置、目标位置、上向量
    update();


}
void myopengl::mouseReleaseEvent(QMouseEvent* event)
{
    //ispressed = false;
}

void myopengl::Axis() {
    //画下x,y,z轴
    glLineWidth(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f); //画红色的x轴
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10000.0f, 0.0f, 0.0f);
    glEnd();
    glColor3f(0.0f, 1.0f, 0.0f); //画绿色的y轴
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10000.0f, 0.0f);
    glEnd();
    glColor3f(0.0f, 0.0f, 1.0f); //画蓝色的z轴
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10000.0f);
    glEnd();
    glFlush();

    //坐标轴刻度
    glPointSize(5.0f);
    glColor3f(1.0f, 1.0f, 1.0f);//画x轴上点
    glBegin(GL_POINTS);
    for (int x = 1; x < 1000; x = x + 1) {
        glVertex3f((float)x, 0.0f, 0.0f);
    }
    glEnd();

    glPointSize(5.0f);
    glColor3f(1.0f, 1.0f, 1.0f);//画y轴上点
    glBegin(GL_POINTS);
    for (int y = 1; y < 1000; y = y + 1) {
        glVertex3f(0.0f, (float)y, 0.0f);
    }
    glEnd();

    glPointSize(5.0f);
    glColor3f(1.0f, 1.0f, 1.0f);//画z轴上点
    glBegin(GL_POINTS);
    for (int z = 1; z < 1000; z = z + 1) {
        glVertex3f(0.0f, 0.0f, (float)z);
    }
    glEnd();
}
void myopengl::Cylinder(GLfloat R, GLfloat H)
{
    glPushMatrix();
    GLUquadricObj* qobj = gluNewQuadric();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(qobj, R, R, H, 32, 32);
    glScalef(-1.0f, 1.0f, -1.0f);
    gluDisk(qobj, 0.0, R, 32, 32);
    glScalef(-1.0f, 1.0f, -1.0f);
    glTranslatef(0.0f, 0.0f, static_cast<GLfloat>(H));
    gluDisk(qobj, 0.0, R, 32, 32);
    gluDeleteQuadric(qobj);
    glPopMatrix();
}
void myopengl::Cuboid(GLfloat L, GLfloat W, GLfloat H)
{
    //qDebug() << 111111 << endl;
    glPushMatrix();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //

   //由四个点组成一个四边形

    GLfloat M = L / 2;
    GLfloat n = W / 2;

    glBegin(GL_QUADS);
    //画底面
    glVertex3f(M, 0, -n);
    glVertex3f(M, 0, n);
    glVertex3f(-M, 0, n);
    glVertex3f(-M, 0, -n);


    //画上面
    glVertex3f(M, H, -n);
    glVertex3f(-M, H, -n);
    glVertex3f(-M, H, n);
    glVertex3f(M, H, n);
    //画右侧
    glVertex3f(M, 0, -n);
    glVertex3f(M, H, -n);
    glVertex3f(M, H, n);
    glVertex3f(M, 0, n);
    //画左侧
    glVertex3f(-M, 0, n);
    glVertex3f(-M, H, n);
    glVertex3f(-M, H, -n);
    glVertex3f(-M, 0, -n);
    //画前面
    glVertex3f(M, 0, n);
    glVertex3f(M, H, n);
    glVertex3f(-M, H, n);
    glVertex3f(-M, 0, n);




    //画后面
    glVertex3f(M, 0, -n);
    glVertex3f(-M, 0, -n);
    glVertex3f(-M, H, -n);
    glVertex3f(M, H, -n);






    glEnd();

    glPopMatrix();
}
void myopengl::Sphere(GLfloat radius)
{
    glPushMatrix();
    GLUquadric* quadObj1 = gluNewQuadric();
    glScalef(-1.0f, 1.0f, -1.0f);
    gluSphere(quadObj1, radius, 32, 32);
    gluDeleteQuadric(quadObj1);
    glPopMatrix();
}
void myopengl::RotationalEllipsoid(GLfloat RL, GLfloat WR, GLfloat H)
{
    glPushMatrix();
    float step_z = PI / 32;
    float step_xy = 2 * PI / 32;
    float x[4], y[4], z[4];

    float angle_z = 0.0;
    float angle_xy = 0.0;
    int i = 0, j = 0;
    glBegin(GL_QUADS);
    for (i = 0; i < 32; i++)
    {
        angle_z = i * step_z;

        for (j = 0; j < 32; j++)
        {
            angle_xy = j * step_xy;

            x[0] = RL * sin(angle_z) * cos(angle_xy);
            y[0] = WR * sin(angle_z) * sin(angle_xy);
            z[0] = H * cos(angle_z);

            x[1] = RL * sin(angle_z + step_z) * cos(angle_xy);
            y[1] = WR * sin(angle_z + step_z) * sin(angle_xy);
            z[1] = H * cos(angle_z + step_z);

            x[2] = RL * sin(angle_z + step_z) * cos(angle_xy + step_xy);
            y[2] = WR * sin(angle_z + step_z) * sin(angle_xy + step_xy);
            z[2] = H * cos(angle_z + step_z);

            x[3] = RL * sin(angle_z) * cos(angle_xy + step_xy);
            y[3] = WR * sin(angle_z) * sin(angle_xy + step_xy);
            z[3] = H * cos(angle_z);
            for (int k = 0; k < 4; k++)
            {

                glVertex3f(0 + x[k], 0 + y[k], 0 + z[k]);
            }
        }
    }
    glEnd();
    glPopMatrix();
}
void myopengl::Table(GLfloat TL1, GLfloat TL2, GLfloat LL1, GLfloat LL2, GLfloat H)
{ /* TL1	顶面对角线长 1
        TL2	顶面对角线长 2
        LL1	底面对角线长 1
        LL2	底面对角线长 2
        H	高*/
    glPushMatrix();
    GLfloat M = TL1 / 2;
    GLfloat N = TL2 / 2;
    GLfloat P = LL1 / 2;
    GLfloat Q = LL2 / 2;
    glBegin(GL_QUADS);//由四个点组成一个四边形

     //画底面
    glVertex3f(P, 0, 0);
    glVertex3f(0, 0, Q);
    glVertex3f(-P, 0, 0);
    glVertex3f(0, 0, -Q);
    //画上面
    glVertex3f(M, H, 0);
    glVertex3f(0, H, N);
    glVertex3f(-M, H, 0);
    glVertex3f(0, H, -N);
    //画左侧
    glVertex3f(P, 0, 0);
    glVertex3f(0, 0, H);
    glVertex3f(0, H, N);
    glVertex3f(M, H, 0);
    //画前面
    glVertex3f(0, 0, Q);
    glVertex3f(-P, 0, 0);;
    glVertex3f(-M, H, 0);
    glVertex3f(0, H, N);
    //画右侧
    glVertex3f(-P, 0, 0);
    glVertex3f(0, 0, -Q);
    glVertex3f(0, H, -N);
    glVertex3f(-M, H, 0);
    //画后面
    glVertex3f(P, 0, 0);
    glVertex3f(0, 0, -Q);
    glVertex3f(0, H, -N);
    glVertex3f(M, H, 0);
    glEnd();
    glPopMatrix();
}
void myopengl::OffsetRectangularTable(GLfloat TL, GLfloat TW, GLfloat LL, GLfloat LW, GLfloat H, GLfloat XOFF, GLfloat YOFF)
{
    glPushMatrix();
    GLfloat M = TL / 2;
    GLfloat N = TW / 2;
    GLfloat P = LL / 2;
    GLfloat Q = LW / 2;
    glBegin(GL_QUADS);//由四个点组成一个四边形

     //画底面
    glVertex3f(P, 0, -Q);
    glVertex3f(P, 0, Q);
    glVertex3f(-P, 0, Q);
    glVertex3f(-P, 0, -Q);
    //画上面
    glVertex3f(XOFF + M, H, -YOFF + N);
    glVertex3f(XOFF + M, H, YOFF + N);
    glVertex3f(-XOFF + M, H, YOFF + N);
    glVertex3f(-XOFF + M, H, -YOFF + N);
    //画左侧
    glVertex3f(P, 0, -Q);
    glVertex3f(P, 0, Q);
    glVertex3f(XOFF + M, H, YOFF + N);
    glVertex3f(XOFF + M, H, -YOFF + N);
    //画前面
    glVertex3f(P, 0, Q);
    glVertex3f(-P, 0, Q);
    glVertex3f(-XOFF + M, H, YOFF + N);
    glVertex3f(XOFF + M, H, YOFF + N);
    //画右侧
    glVertex3f(-P, 0, Q);
    glVertex3f(-P, 0, -Q);
    glVertex3f(-XOFF + M, H, -YOFF + N);
    glVertex3f(-XOFF + M, H, YOFF + N);
    //画后面
    glVertex3f(P, 0, -Q);
    glVertex3f(-P, 0, -Q);
    glVertex3f(-XOFF + M, H, -YOFF + N);
    glVertex3f(XOFF + M, H, -YOFF + N);
    glEnd();
    glPopMatrix();
}
void myopengl::TruncatedCone(GLfloat TR, GLfloat BR, GLfloat H)
{
    glPushMatrix();
    GLUquadricObj* qobj = gluNewQuadric();

    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(qobj, TR, BR, H, 32, 32);
    //glScalef(-1.0f, 1.0f, -1.0f);
    gluDisk(qobj, 0.0, TR, 32, 32);
    //glScalef(-1.0f, 1.0f, -1.0f);
    glTranslatef(0.0f, 0.0f, H);

    gluDisk(qobj, 0.0, BR, 32, 32);

    gluDeleteQuadric(qobj);
    glPopMatrix();
}
void myopengl::EccentricTruncatedCone(GLfloat TR, GLfloat BR, GLfloat H, GLfloat TOPXOFF, GLfloat TOPYOFF)
{
    glPushMatrix();
    GLUquadricObj* qobj = gluNewQuadric();
    glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 15)
    {
        float p = i * 3.14 / 180;
        glVertex3f(TR * sin(p) + TOPXOFF, TR * cos(p) + TOPYOFF, 1.0f);
        glVertex3f(BR * sin(p), BR * cos(p), 0.0f);
    }
    glEnd();

    gluDisk(qobj, 0.0, BR, 32, 32);

    glTranslatef(TOPXOFF, TOPYOFF, H);

    gluDisk(qobj, 0.0, TR, 32, 32);

    gluDeleteQuadric(qobj);
    glPopMatrix();
}
void myopengl::Ring(GLfloat DR, GLfloat R, GLfloat Rad)
{
    glPushMatrix();
    double sideDelta = 2 * PI / 32;
    double ringDelta = Rad / 32;//控制弧度
    double theta = 0;
    double cosTheta = 1.0;
    double sinTheta = 0.0;

    double phi, sinPhi, cosPhi;
    double dist;
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 32; i++)
    {
        double theta1 = theta + ringDelta;
        double cosTheta1 = cos(theta1);
        double sinTheta1 = sin(theta1);

        glBegin(GL_QUAD_STRIP);
        phi = 0;
        for (int j = 0; j <= 32; j++)
        {
            phi = phi + sideDelta;
            cosPhi = cos(phi);
            sinPhi = sin(phi);
            dist = R + (DR * cosPhi);

            glNormal3f(cosTheta * cosPhi, sinTheta * cosPhi, sinPhi);

            glVertex3f(cosTheta * dist, sinTheta * dist, DR * sinPhi);

            glNormal3f(cosTheta1 * cosPhi, sinTheta1 * cosPhi, sinPhi);

            glVertex3f(cosTheta1 * dist, sinTheta1 * dist, DR * sinPhi);
        }
        glEnd();
        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }
    glPopMatrix();
}
void myopengl::RectangularRing(GLfloat DR, GLfloat R, GLfloat L, GLfloat W)
{
    glPushMatrix();
    GLfloat M = L - 2 * (R - DR);
    GLfloat N = W - 2 * (R - DR);
    GLfloat T = R - DR;

    glPushMatrix();
    GLUquadricObj* qobj = gluNewQuadric();
    glTranslatef(-M / 2, 0, -W / 2);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    gluCylinder(qobj, DR, DR, M, 32, 32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(M / 2, 0, -N / 2);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    Ring(DR, T, PI / 2);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-M / 2, 0, W / 2);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    gluCylinder(qobj, DR, DR, M, 32, 32);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(M / 2, 0, N / 2);
    Ring(DR, T, PI / 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-M / 2, 0, -N / 2);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    Ring(DR, T, PI / 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-M / 2, 0, N / 2);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
    Ring(DR, T, PI / 2);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(L / 2, 0, -N / 2);

    gluCylinder(qobj, DR, DR, N, 32, 32);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-L / 2, 0, -N / 2);

    //glBegin(GL_QUAD_STRIP);//连续填充四边形串
    //int b = 0;
    //for (b = 0; b <= 360; b += 15)
    //{
    //    float p = (float)(b * 3.14 / 180);
    //    glVertex3f(DR * sin(p), DR * cos(p), N);
    //    glVertex3f(DR * sin(p), DR * cos(p), 0.0f);
    //}
    //glEnd();
    gluCylinder(qobj, DR, DR, N, 32, 32);
    glPopMatrix();
    glPopMatrix();
}
void myopengl::EllipticRing(GLfloat DR, GLfloat L, GLfloat W)
{
    glPushMatrix();
    double sideDelta = 2 * PI / 32;
    double ringDelta = 2 * PI / 32;//控制弧度
    double theta = 0;
    double cosTheta = 1.0;
    double sinTheta = 0.0;

    double phi, sinPhi, cosPhi;
    double dist1;
    double dist2;
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 32; i++)
    {
        double theta1 = theta + ringDelta;
        double cosTheta1 = cos(theta1);
        double sinTheta1 = sin(theta1);

        glBegin(GL_QUAD_STRIP);
        phi = 0;
        for (int j = 0; j <= 32; j++)
        {
            phi = phi + sideDelta;
            cosPhi = L * cos(phi);
            sinPhi = W * sin(phi);
            dist1 = L + (DR * cosPhi);
            dist2 = W + (DR * cosPhi);

            glNormal3f(cosTheta * cosPhi, sinTheta * cosPhi, sinPhi);

            glVertex3f(cosTheta * dist1, sinTheta * dist2, DR * sinPhi);

            glNormal3f(cosTheta1 * cosPhi, sinTheta1 * cosPhi, sinPhi);

            glVertex3f(cosTheta1 * dist1, sinTheta1 * dist2, DR * sinPhi);
        }
        glEnd();
        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }
    glPopMatrix();
}
void myopengl::CircularGasket(GLfloat IR, GLfloat OR, GLfloat H, GLfloat Rad)
{
    glPushMatrix();
    GLUquadricObj* qobj = gluNewQuadric();
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glPushMatrix();

    gluPartialDisk(qobj, IR, OR, 32, 32, 0, Rad);
    /* GLUquadric *qobj,
    GLdouble   innerRadius,
        GLdouble   outerRadius,
        GLint      slices,
        GLint      loops,
        GLdouble   startAngle,
        GLdouble   sweepAngle*/
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int j = 0;
    for (j = 0; j <= Rad; j += 15)
    {
        float p = (float)(j * 3.14 / 180);
        glVertex3f(IR * sin(p), IR * cos(p), H);
        glVertex3f(IR * sin(p), IR * cos(p), 0.0f);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    int i = 0;
    for (i = 0; i <= Rad; i += 15)
    {
        float p = (float)(i * 3.14 / 180);
        glVertex3f(OR * sin(p), OR * cos(p), H);
        glVertex3f(OR * sin(p), OR * cos(p), 0.0f);
    }
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, H);
    gluPartialDisk(qobj, IR, OR, 32, 32, 0, Rad);
    glPopMatrix();
    gluDeleteQuadric(qobj);
    glPopMatrix();
}
void myopengl::TableGasket(GLfloat TR, GLfloat OR, GLfloat IR, GLfloat H, GLfloat Rad)
{
    glPushMatrix();
    GLUquadricObj* qobj = gluNewQuadric();
    glRotatef(270, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    gluPartialDisk(qobj, IR, OR, 32, 32, 0, Rad);

    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= Rad; i += 15)
    {
        float p = (float)(i * 3.14 / 180);
        glVertex3f(IR * sin(p), IR * cos(p), H);
        glVertex3f(IR * sin(p), IR * cos(p), 0.0f);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int j = 0;
    for (j = 0; j <= Rad; j += 15)
    {
        float p = (float)(j * 3.14 / 180);
        glVertex3f(TR * sin(p), TR * cos(p), H);
        glVertex3f(OR * sin(p), OR * cos(p), 0);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, H);
    gluPartialDisk(qobj, IR, TR, 32, 32, 0, Rad);
    glPopMatrix();
    gluDeleteQuadric(qobj);
    glPopMatrix();
}
void myopengl::SquareGasket(GLfloat L1, GLfloat W1, GLfloat L2, GLfloat W2, GLfloat H, GLfloat CT, GLfloat R, GLfloat CL)
{
    glPushMatrix();
    if (CT == 1) {
        glRotatef(270, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        //画右底面
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L1 / 2, 0, W1 / 2);
        glVertex3f(L1 / 2, 0, -W1 / 2);
        //右上面
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glVertex3f(L1 / 2, H, W1 / 2);
        glVertex3f(L1 / 2, H, -W1 / 2);
        //画上面底
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L1 / 2, 0, -W1 / 2);
        glVertex3f(L1 / 2, 0, -W1 / 2);
        //画上面上
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-L1 / 2, H, -W1 / 2);
        glVertex3f(L1 / 2, H, -W1 / 2);
        //画左侧底面
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L1 / 2, 0, W1 / 2);
        glVertex3f(-L1 / 2, 0, -W1 / 2);
        //画左侧上面
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(-L1 / 2, H, W1 / 2);
        glVertex3f(-L1 / 2, H, -W1 / 2);
        //画前面底面
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L1 / 2, 0, W1 / 2);
        glVertex3f(L1 / 2, 0, W1 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        //画前面上面
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(-L1 / 2, H, W1 / 2);
        glVertex3f(L1 / 2, H, W1 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);


        glVertex3f(L1 / 2, 0, W1 / 2);
        glVertex3f(L1 / 2, H, W1 / 2);
        glVertex3f(L1 / 2, 0, -W1 / 2);
        glVertex3f(L1 / 2, H, -W1 / 2);

        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);

        glVertex3f(-L1 / 2, 0, -W1 / 2);
        glVertex3f(-L1 / 2, H, -W1 / 2);
        glVertex3f(L1 / 2, 0, -W1 / 2);
        glVertex3f(L1 / 2, H, -W1 / 2);

        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);

        glVertex3f(-L1 / 2, 0, W1 / 2);
        glVertex3f(-L1 / 2, H, W1 / 2);
        glVertex3f(-L1 / 2, H, -W1 / 2);
        glVertex3f(-L1 / 2, H, -W1 / 2);

        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(-L1 / 2, 0, W1 / 2);
        glVertex3f(-L1 / 2, H, W1 / 2);

        glVertex3f(L1 / 2, 0, W1 / 2);
        glVertex3f(L1 / 2, H, W1 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glEnd();
    }
    else if (CT == 3) {
        GLfloat M = (L1 / 2) - CL;
        GLfloat N = (W1 / 2) - CL;
        glRotatef(270, 0.0f, 0.0f, 1.0f);
        //画右底面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(M, 0, W1 / 2);
        glVertex3f(L1 / 2, 0, N);
        glVertex3f(L1 / 2, 0, -N);
        glEnd();
        //画上地面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-M, 0, -W1 / 2);
        glVertex3f(M, 0, -W1 / 2);
        glVertex3f(L1 / 2, 0, -N);
        glEnd();
        //画左地底面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-M, 0, -W1 / 2);
        glVertex3f(-L1 / 2, 0, -N);
        glVertex3f(-L1 / 2, 0, N);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glEnd();
        //画下面底面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L1 / 2, 0, N);
        glVertex3f(-M, 0, W1 / 2);
        glVertex3f(M, 0, W1 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glEnd();


        //画右上面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glVertex3f(M, H, W1 / 2);
        glVertex3f(L1 / 2, H, N);
        glVertex3f(L1 / 2, H, -N);
        glEnd();
        //画上上面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-M, H, -W1 / 2);
        glVertex3f(M, H, -W1 / 2);
        glVertex3f(L1 / 2, H, -N);
        glEnd();
        //画左上面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-M, H, -W1 / 2);
        glVertex3f(-L1 / 2, H, -N);
        glVertex3f(-L1 / 2, H, N);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glEnd();
        //画下上面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(-L1 / 2, H, N);
        glVertex3f(-M, H, W1 / 2);
        glVertex3f(M, H, W1 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glEnd();


        //画侧面
        glBegin(GL_QUAD_STRIP);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);

        glVertex3f(M, 0, W1 / 2);
        glVertex3f(M, H, W1 / 2);
        glVertex3f(L1 / 2, 0, N);
        glVertex3f(L1 / 2, H, N);

        glVertex3f(L1 / 2, 0, N);
        glVertex3f(L1 / 2, H, N);
        glVertex3f(L1 / 2, 0, -N);
        glVertex3f(L1 / 2, H, -N);

        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);

        glVertex3f(-M, 0, -W1 / 2);
        glVertex3f(-M, H, -W1 / 2);
        glVertex3f(M, 0, -W1 / 2);
        glVertex3f(M, H, -W1 / 2);

        glVertex3f(M, 0, -W1 / 2);
        glVertex3f(M, H, -W1 / 2);
        glVertex3f(L1 / 2, 0, -N);
        glVertex3f(L1 / 2, H, -N);

        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-M, 0, -W1 / 2);
        glVertex3f(-M, H, -W1 / 2);

        glVertex3f(-M, 0, -W1 / 2);
        glVertex3f(-M, H, -W1 / 2);
        glVertex3f(-L1 / 2, 0, -N);
        glVertex3f(-L1 / 2, H, -N);

        glVertex3f(-L1 / 2, 0, -N);
        glVertex3f(-L1 / 2, H, -N);
        glVertex3f(-L1 / 2, 0, N);
        glVertex3f(-L1 / 2, H, N);

        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);

        glVertex3f(-L1 / 2, 0, N);
        glVertex3f(-L1 / 2, H, N);
        glVertex3f(-M, 0, W1 / 2);
        glVertex3f(-M, H, W1 / 2);

        glVertex3f(-M, 0, W1 / 2);
        glVertex3f(-M, H, W1 / 2);
        glVertex3f(M, 0, W1 / 2);
        glVertex3f(M, H, W1 / 2);

        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glEnd();
    }
    else {
        glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
        GLUquadricObj* qobj = gluNewQuadric();
        GLfloat P = (L1 / 2) - R;
        GLfloat Q = (W1 / 2) - R;
        //画右底面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(P, 0, Q);
        glVertex3f(L1 / 2, 0, Q);
        glVertex3f(L1 / 2, 0, -Q);
        glVertex3f(P, 0, -Q);
        glEnd();
        glPushMatrix();
        glTranslated(P, 0, Q);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        glPushMatrix();
        glTranslated(P, 0, -Q);
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        //画上底面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(P, 0, -Q);
        glVertex3f(P, 0, -W1 / 2);
        glVertex3f(-P, 0, -W1 / 2);
        glVertex3f(-P, 0, -Q);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glEnd();
        glPushMatrix();
        glTranslated(-P, 0, -Q);
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        //画左底面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-P, 0, -Q);
        glVertex3f(-L1 / 2, 0, -Q);
        glVertex3f(-L1 / 2, 0, Q);
        glVertex3f(-P, 0, Q);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glEnd();
        glPushMatrix();
        glTranslated(-P, 0, Q);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        //画下底面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-P, 0, Q);
        glVertex3f(-P, 0, W1 / 2);
        glVertex3f(P, 0, W1 / 2);
        glVertex3f(P, 0, Q);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glEnd();


        //画右上面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);
        glVertex3f(P, H, Q);
        glVertex3f(L1 / 2, H, Q);
        glVertex3f(L1 / 2, H, -Q);
        glVertex3f(P, H, -Q);
        glEnd();
        glPushMatrix();
        glTranslated(P, H, Q);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        glPushMatrix();
        glTranslated(P, H, -Q);
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        //画上上面
        glBegin(GL_POLYGON);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(P, H, -Q);
        glVertex3f(P, H, -W1 / 2);
        glVertex3f(-P, H, -W1 / 2);
        glVertex3f(-P, H, -Q);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glEnd();
        glPushMatrix();
        glTranslated(-P, H, -Q);
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        //画左上面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-P, H, -Q);
        glVertex3f(-L1 / 2, H, -Q);
        glVertex3f(-L1 / 2, H, Q);
        glVertex3f(-P, H, Q);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glEnd();
        glPushMatrix();
        glTranslated(-P, H, Q);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        gluPartialDisk(qobj, 0, R, 32, 32, 0, 90);
        glPopMatrix();
        //画下上面
        glBegin(GL_POLYGON);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(-P, H, Q);
        glVertex3f(-P, H, W1 / 2);
        glVertex3f(P, H, W1 / 2);
        glVertex3f(P, H, Q);
        glVertex3f(L2 / 2, H, W2 / 2);
        glEnd();


        //画侧面
        glBegin(GL_QUAD_STRIP);
        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);

        glVertex3f(L1 / 2, 0, Q);
        glVertex3f(L1 / 2, H, Q);
        glVertex3f(L1 / 2, 0, -Q);
        glVertex3f(L1 / 2, H, -Q);

        glVertex3f(L2 / 2, 0, -W2 / 2);
        glVertex3f(L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);

        glVertex3f(P, 0, -W1 / 2);
        glVertex3f(P, H, -W1 / 2);
        glVertex3f(-P, 0, -W1 / 2);
        glVertex3f(-P, H, -W1 / 2);

        glVertex3f(-L2 / 2, 0, -W2 / 2);
        glVertex3f(-L2 / 2, H, -W2 / 2);
        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);

        glVertex3f(-L1 / 2, 0, -Q);
        glVertex3f(-L1 / 2, H, -Q);
        glVertex3f(-L1 / 2, 0, Q);
        glVertex3f(-L1 / 2, H, Q);

        glVertex3f(-L2 / 2, 0, W2 / 2);
        glVertex3f(-L2 / 2, H, W2 / 2);
        glVertex3f(L2 / 2, 0, W2 / 2);
        glVertex3f(L2 / 2, H, W2 / 2);

        glVertex3f(-P, 0, W1 / 2);
        glVertex3f(-P, H, W1 / 2);
        glVertex3f(P, 0, W1 / 2);
        glVertex3f(P, H, W1 / 2);
        glEnd();

        glPushMatrix();
        glTranslated(P, H, Q);
        glRotatef(-270.0f, 1.0f, 0.0f, 0.0f);
        glBegin(GL_QUAD_STRIP);//连续填充四边形串
        int j = 0;
        for (j = 0; j <= 90; j += 15)
        {
            float p = (float)(j * 3.14 / 180);
            glVertex3f(R * sin(p), R * cos(p), H);
            glVertex3f(R * sin(p), R * cos(p), 0.0f);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslated(P, 0, -Q);
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        glBegin(GL_QUAD_STRIP);//连续填充四边形串
        int i = 0;
        for (i = 0; i <= 90; i += 15)
        {
            float p = (float)(i * 3.14 / 180);
            glVertex3f(R * sin(p), R * cos(p), H);
            glVertex3f(R * sin(p), R * cos(p), 0.0f);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-P, 0, -Q);
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUAD_STRIP);//连续填充四边形串
        int a = 0;
        for (a = 0; a <= 90; a += 15)
        {
            float p = (float)(a * 3.14 / 180);
            glVertex3f(R * sin(p), R * cos(p), H);
            glVertex3f(R * sin(p), R * cos(p), 0.0f);
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-P, H, Q);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUAD_STRIP);//连续填充四边形串
        int b = 0;
        for (b = 0; b <= 90; b += 15)
        {
            float p = (float)(b * 3.14 / 180);
            glVertex3f(R * sin(p), R * cos(p), H);
            glVertex3f(R * sin(p), R * cos(p), 0.0f);
        }
        glEnd();
        glPopMatrix();
        gluDeleteQuadric(qobj);
    }
    glPopMatrix();
}
void myopengl::StretchedBody()
{
    glPushMatrix();
    float  x[6];
    float  y[6];
    float  z[6];
    x[0] = 0;
    y[0] = 0;
    z[0] = 0;
    x[1] = 1;
    y[1] = 0;
    z[1] = 0;
    x[2] = 1;
    y[2] = 0;
    z[2] = 1;
    x[3] = 0.5;
    y[3] = 1;
    z[3] = 0;
    x[4] = 1.5;
    y[4] = 1;
    z[4] = 0;
    x[5] = 1.5;
    y[5] = 1;
    z[5] = 1;
    int  i = 0, j = 0;
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            glVertex3f(x[i], y[i], z[i]);
            glVertex3f(x[j], y[j], z[j]);
        }
    }
    glEnd();
    glPopMatrix();
}
void myopengl::PorcelainBushing(GLfloat H, GLfloat R, GLfloat R1, GLfloat R2, GLfloat N)
{
    glPushMatrix();
    glRotatef(270, 1.0f, 0.0f, 0.0f);
    GLUquadricObj* qobj = gluNewQuadric();
    glPushMatrix();
    gluCylinder(qobj, R, R, H, 32, 32);
    glPopMatrix();
    GLfloat H2 = H / N / 4.0F;
    for (int i = 0; i < N; i++) {
        //绘制小曲面
        glPushMatrix();

        glTranslatef(0.0f, 0.0F, H2);
        gluCylinder(qobj, R2, R, H2, 32, 32);
        glPopMatrix();
        //绘制大曲面
        glPushMatrix();

        glTranslatef(0.0f, 0.0F, 3 * H2);
        gluCylinder(qobj, R1, R, H2, 32, 32);
        glPopMatrix();
        glTranslatef(0.0f, 0.0F, 4 * H2);
    }
    gluDeleteQuadric(qobj);
    glPopMatrix();
}
void myopengl::ConePorcelainBushing(GLfloat H, GLfloat BR, GLfloat TR, GLfloat BR1, GLfloat BR2, GLfloat TR1, GLfloat TR2, GLfloat N)
{
    glPushMatrix();
    glTranslatef(0.0f, -0.0F, -1.0);
    GLUquadricObj* qobj = gluNewQuadric();
    glPushMatrix();
    gluCylinder(qobj, TR, BR, H, 32, 32);
    glPopMatrix();
    GLfloat H2 = H / N / 8.0F;
    for (int i = 0; i < N; i++) {
        //绘制顶部大曲面
        glPushMatrix();
        glTranslatef(0.0f, 0.0F, H2);
        gluCylinder(qobj, TR, TR1, H2, 32, 32);
        glPopMatrix();

        //绘制顶部小曲面
        glPushMatrix();
        glTranslatef(0.0f, 0.0F, 3 * H2);
        gluCylinder(qobj, TR, TR2, H2, 32, 32);
        glPopMatrix();

        //绘制底部大曲面
        glPushMatrix();
        glTranslatef(0.0f, 0.0F, 5 * H2);
        gluCylinder(qobj, TR, BR1, H2, 32, 32);
        glPopMatrix();


        //绘制底部小曲面
        glPushMatrix();
        glTranslatef(0.0f, 0.0F, 7 * H2);
        gluCylinder(qobj, TR, BR2, H2, 32, 32);
        glPopMatrix();
        glTranslatef(0.0f, 0.0F, 8 * H2);

    }
    gluDeleteQuadric(qobj);
    glPopMatrix();

}
void myopengl::Insulator(int N, GLfloat D, int N1, GLfloat H1, GLfloat R1, GLfloat R2, GLfloat R)
{
    for (int j = 0; j < N; j++) {


        glPushMatrix();
        GLfloat M = H1 * N1;
        glTranslatef(-M / 2, 0, j * D);
        glRotatef(90, 0.0f, 1.0f, 0.0f);


        GLUquadricObj* qobj = gluNewQuadric();

        gluCylinder(qobj, R, R, M, 32, 32);



        GLfloat H = H1 / 4.0F;
        for (int i = 0; i < N1; i++) {
            //绘制大曲面
            gluCylinder(qobj, R, R1, H, 32, 32);


            //绘制小曲面
            glPushMatrix();
            glTranslatef(0.0f, 0.0F, 2 * H);
            gluCylinder(qobj, R, R2, H, 32, 32);
            glPopMatrix();
            glTranslatef(0.0f, 0.0F, 4 * H);
        }

        gluDeleteQuadric(qobj);

        glPopMatrix();

    }
}
void myopengl::VTypeInsulator(GLfloat X, GLfloat AD, int N1, GLfloat H1, GLfloat R, GLfloat R1, GLfloat R2, GLfloat FL, GLfloat AL)
{
    glPushMatrix();
    GLfloat M = H1 * N1;

    GLfloat P = (X - AD) / 2 * M;
    GLfloat Q = acos(P);
    GLfloat H = H1 / 4.0F;

    glPushMatrix();
    glTranslatef(-X / 2, 2.0F, 0.0F);
    glRotatef(30, 1.0f, 0.0f, 0.0f);

    GLUquadricObj* qobj = gluNewQuadric();
    gluCylinder(qobj, R, R, M, 32, 32);

    for (int i = 0; i < N1; i++) {
        //绘制大曲面
        gluCylinder(qobj, R, R1, H, 32, 32);
        //绘制小曲面
        glPushMatrix();
        glTranslatef(0.0f, 0.0F, 2 * H);
        gluCylinder(qobj, R, R2, H, 32, 32);
        glPopMatrix();
        glTranslatef(0.0f, 0.0F, 4 * H);
    }

    glPopMatrix();
    glPushMatrix();
    glTranslatef(-X / 2, -2.0F, 0.0F);
    glRotatef(-30, 1.0f, 0.0f, 0.0f);

    gluCylinder(qobj, R, R, M, 32, 32);
    //GLfloat H = H1 / 4.0F;
    for (int i = 0; i < N1; i++) {
        //绘制大曲面
        gluCylinder(qobj, R, R1, H, 32, 32);
        //绘制小曲面
        glPushMatrix();
        glTranslatef(0.0f, 0.0F, 2 * H);
        gluCylinder(qobj, R, R2, H, 32, 32);
        glPopMatrix();
        glTranslatef(0.0f, 0.0F, 4 * H);
    }
    gluDeleteQuadric(qobj);
    glPopMatrix();
    glPopMatrix();
}
void myopengl::IrregularCuboid(GLfloat L, GLfloat W, GLfloat T, GLfloat CL)
{
    glPushMatrix();
    glBegin(GL_QUADS);//由四个点组成一个四边形

    //GLfloat M = L / 2;
    GLfloat N = W / 2;
    GLfloat P = T / 2;
    //画底面
    glVertex3f(-N, 0, -P);
    glVertex3f(-N, 0, P);
    glVertex3f(N, 0, P);
    glVertex3f(N, 0, -P);
    //画上面
    glVertex3f(-N + CL, L, -P);
    glVertex3f(-N + CL, L, P);
    glVertex3f(N - CL, L, P);
    glVertex3f(N - CL, L, -P);
    //画左侧
    glVertex3f(N, 0, -P);
    glVertex3f(N, L - CL, -P);
    glVertex3f(N, L - CL, P);
    glVertex3f(N, 0, P);
    //画右侧
    glVertex3f(-N, 0, -P);
    glVertex3f(-N, 0, P);
    glVertex3f(-N, L - CL, P);
    glVertex3f(-N, L - CL, -P);

    //右上方四边形
    glVertex3f(N, L - CL, P);
    glVertex3f(N, L - CL, -P);
    glVertex3f(N - CL, L, -P);
    glVertex3f(N - CL, L, P);

    //左上方四边形
    glVertex3f(-N, L - CL, -P);
    glVertex3f(-N, L - CL, P);
    glVertex3f(-N + CL, L, P);
    glVertex3f(-N + CL, L, -P);
    glEnd();
    glBegin(GL_POLYGON);
    //画前面
    glVertex3f(-N, 0, P);
    glVertex3f(N, 0, P);
    glVertex3f(N, L - CL, P);
    glVertex3f(N - CL, L, P);
    glVertex3f(-N + CL, L, P);
    glVertex3f(-N, L - CL, P);

    glEnd();
    glBegin(GL_POLYGON);
    //画后面
    glVertex3f(N, 0, -P);
    glVertex3f(-N, 0, -P);
    glVertex3f(-N, L - CL, -P);
    glVertex3f(-N + CL, L, -P);
    glVertex3f(N - CL, L, -P);
    glVertex3f(N, L - CL, -P);
    glEnd();

    glPopMatrix();
}
void myopengl::TerminalBlock(GLfloat L, GLfloat W, GLfloat T, GLfloat CL, GLfloat CS, GLfloat RS, GLfloat R1, int CN, int RN, GLfloat BL, GLfloat X, GLfloat Y, GLfloat Z, GLfloat rX, GLfloat rY, GLfloat rZ)
{
    glPushMatrix();
    clearPrimtives();
    GLfloat  m = T + 0.5;
    GLfloat Q = RN / 2;
    GLfloat r = (CN - 1) / 2;

    GLfloat P = CN / 2;
    GLfloat s = (RN - 1) / 2;
    if (!glIsList((GLuint)(1002))) {
        //GLuint 100000002 = glGenLists(1);
        glNewList(1002, GL_COMPILE);
        glPushMatrix();
        glScalef(ScalefX, ScalefY, ScalefZ);
        glTranslatef(X, Z, Y);
        glRotatef(rX, 1, 0, 0);
        glRotatef(rY, 0, 1, 0);
        glRotatef(rZ, 0, 0, 1);
        IrregularCuboid(L, W, T, CL);
        glPopMatrix();
        glEndList();
    }
    primitives.push_back(new OpenCSG::DisplayListPrimitive(1002, OpenCSG::Intersection, 1));
    if (CN % 2 == 0 && RN % 2 != 0) {
        int a = 1;
        for (int x = -P; x < P; ++x) {
            int i = a;
            for (int z = -s; z <= s; ++z) {
                GLfloat  o = BL + s * RS;
                int b = 1100 + z + i;
                if (!glIsList((GLuint)(b))) {
                    glNewList(b, GL_COMPILE);
                    //GLuint id100000003 = glGenLists(1);
                    //glNewList(id100000003, GL_COMPILE);
                    glPushMatrix();
                    glTranslatef(X, Z, Y);
                    glRotatef(rX, 1, 0, 0);
                    glRotatef(rY, 0, 1, 0);
                    glRotatef(rZ, 0, 0, 1);
                    glScalef(ScalefX, ScalefY, ScalefZ);
                    glTranslatef(0, o, -m);
                    glPushMatrix();
                    glTranslatef((2 * x + 1) * CS / 2, z * RS, 0);
                    glRotated(-90, 1, 0, 0);
                    Cylinder(R1, L);
                    glPopMatrix();
                    glPopMatrix();
                    glEndList();
                }
                primitives.push_back(new OpenCSG::DisplayListPrimitive(b, OpenCSG::Subtraction, 1));

            }
            a = a + RN;
        }

    }
    if (CN % 2 == 0 && RN % 2 == 0) {
        int a = 4;


        for (int x = -P; x < P; ++x) {

            int i = a;
            for (int z = -Q; z < Q; ++z) {

                GLfloat  n = BL + Q * RS;
                GLfloat c = (2 * x + 1) * CS / 2;
                GLfloat d = (2 * z + 1) * RS / 2;
                int b = 1005 + z + i;



                if (!glIsList((GLuint)(b))) {
                    glNewList(b, GL_COMPILE);
                    //GLuint id100000004 = glGenLists(1);
                    //glNewList(id100000004, GL_COMPILE);
                    glPushMatrix();
                    glTranslatef(X, Z, Y);
                    glRotatef(rX, 1, 0, 0);
                    glRotatef(rY, 0, 1, 0);
                    glRotatef(rZ, 0, 0, 1);
                    glScalef(ScalefX, ScalefY, ScalefZ);
                    glTranslatef(0, n, -m);
                    glPushMatrix();
                    glTranslatef(c, d, 0);
                    glRotated(-90, 1, 0, 0);
                    Cylinder(R1, L);
                    glPopMatrix();
                    glPopMatrix();
                    glEndList();
                }



                primitives.push_back(new OpenCSG::DisplayListPrimitive(b, OpenCSG::Subtraction, 1));

            }
            a = a + CN;

        }
    }
    if (CN % 2 != 0 && RN % 2 == 0) {
        int a = 1;
        for (int x = -r; x <= r; ++x) {
            int i = a;
            for (int z = -Q; z < Q; ++z) {
                int b = 1100 + z + i;
                GLfloat  n = BL + Q * RS;
                if (!glIsList((GLuint)(b))) {
                    glNewList(b, GL_COMPILE);
                    //GLuint id100000005 = glGenLists(1);
                    //glNewList(id100000005, GL_COMPILE);
                    glPushMatrix();
                    glTranslatef(X, Z, Y);
                    glScalef(ScalefX, ScalefY, ScalefZ);


                    glTranslatef(0, n, -m);

                    glPushMatrix();

                    glTranslatef(x * CS, (2 * z + 1) * RS / 2, 0);

                    glRotated(-90, 1, 0, 0);

                    glRotatef(rX, 1, 0, 0);
                    glRotatef(rY, 0, 1, 0);
                    glRotatef(rZ, 0, 0, 1);

                    Cylinder(R1, L);

                    glPopMatrix();
                    glPopMatrix();
                    glEndList();
                }
                primitives.push_back(new OpenCSG::DisplayListPrimitive(b, OpenCSG::Subtraction, 1));

            }
            a = a + RN;
        }

    }
    if (CN % 2 != 0 && RN % 2 != 0) {
        int a = 1;
        for (int x = -r; x <= r; ++x) {
            int i = a;
            for (int z = -s; z <= s; ++z) {
                GLfloat  n = BL + s * RS;
                int b = 1100 + z + i;
                if (!glIsList((GLuint)(b))) {
                    glNewList(b, GL_COMPILE);
                    //GLuint id100000006 = glGenLists(1);
                    //glNewList(id100000006, GL_COMPILE);
                    glPushMatrix();
                    glTranslatef(X, Z, Y);
                    glRotatef(rX, 1, 0, 0);
                    glRotatef(rY, 0, 1, 0);
                    glRotatef(rZ, 0, 0, 1);

                    glScalef(ScalefX, ScalefY, ScalefZ);
                    glTranslatef(0, n, -m);

                    glPushMatrix();

                    glTranslatef(x * CS, z * RS, 0);
                    glRotated(-90, 1, 0, 0);
                    Cylinder(R1, L);
                    glPopMatrix();
                    glPopMatrix();
                    glEndList();
                }
                primitives.push_back(new OpenCSG::DisplayListPrimitive(b, OpenCSG::Subtraction, 1));

            }
            a = a + RN;
        }

    }
    OpenCSG::render(primitives);
    glDepthFunc(GL_EQUAL);
    for (std::vector<OpenCSG::Primitive*>::const_iterator j = primitives.begin(); j != primitives.end(); ++j) {
        (*j)->render();
    }
    glDepthFunc(GL_LESS);

    glPopMatrix();

}

void myopengl::drawboolEntity()
{
    for (int i = 0; i < gimModFileList.size(); i++) {
        ModFile modFile = gimModFileList[i];
        //qDebug() << modFile.getFileName() << endl;
        glLoadName(gimModFileList.size()+1+i);
        //glLoadName(i+1);
        for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
            if (!simpleEntity.getGraphicElement().isNull()) {
                if (simpleEntity.getVisible() == ("true")) {
                    if (simpleEntity.getGraphicElement().nodeName() == ("TerminalBlock")) {

                        QDomElement n = simpleEntity.getGraphicElement().toElement();
                        QString  vs = simpleEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        ////获取旋转角度
                        float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X = mat[12];
                        float Y = mat[13];
                        float Z = mat[14];
                        //获取颜色
                        float R = (float)simpleEntity.getR() / 255;
                        float G = (float)simpleEntity.getG() / 255;
                        float B = (float)simpleEntity.getB() / 255;
                        float A = (float)simpleEntity.getA() / 255;
                        //获取长宽高
                        float L = (n.attribute("L")).toFloat();
                        float W = (n.attribute("W")).toFloat();
                        float T = (n.attribute("T")).toFloat();
                        float CL = (n.attribute("CL")).toFloat();
                        float CS = (n.attribute("CS")).toFloat();
                        float RS = (n.attribute("RS")).toFloat();
                        float R1 = (n.attribute("R")).toFloat();
                        float CN = (n.attribute("CN")).toFloat();
                        float RN = (n.attribute("RN")).toFloat();
                        float BL = (n.attribute("BL")).toFloat();
                        //                  
                        glPushMatrix();
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        glColor4f(R, G, B, A);
                        TerminalBlock(L, W, T, CL, CS, RS, R1, CN, RN, BL, X, Y, Z, rX, rY, rZ);
                        glPopMatrix();
                    }
                    //if (simpleEntity.getGraphicElement().nodeName() == ("Rectangular fixed plate")) {
                    //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                    //    QString  vs = simpleEntity.getTransformMatrix();
                    //    QStringList splits = vs.split(",");
                    //    float mat[16];
                    //    for (int i = 0; i < 16; i++)
                    //    {
                    //        mat[i] = (splits[i]).toFloat();
                    //    }
                    //    //获取旋转角度
                    //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                    //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                    //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                    //    //获取坐标
                    //    float X = mat[12];
                    //    float Y = mat[13];
                    //    float Z = mat[14];
                    //    //获取颜色
                    //    float R = (float)simpleEntity.getR() / 255;
                    //    float G = (float)simpleEntity.getG() / 255;
                    //    float B = (float)simpleEntity.getB() / 255;
                    //    float A = (float)simpleEntity.getA() / 255;
                    //    //获取长宽高
                    //    float L = (n.attribute("L")).toFloat();
                    //    float W = (n.attribute("W")).toFloat();
                    //    float T = (n.attribute("T")).toFloat();
                    //    float CS = (n.attribute("CS")).toFloat();
                    //    float RS = (n.attribute("RS")).toFloat();
                    //    float CN = (n.attribute("CN")).toFloat();
                    //    float RN = (n.attribute("RN")).toFloat();
                    //    float MH = (n.attribute("MH")).toFloat();
                    //    float D = (n.attribute("D")).toFloat();
                    //    //
                    //    glPushMatrix();
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    //    glColor4f(R, G, B, A);
                    //    RectangularFixedPlate(L, W, T, CS, RS, CN, RN, MH, D, 0, 0, 0, rX, rY, rZ);
                    //    glPopMatrix();
                    //}
                    //if (simpleEntity.getGraphicElement().nodeName() == ("Circular fixed plate")) {
                    //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                    //    QString  vs = simpleEntity.getTransformMatrix();
                    //    QStringList splits = vs.split(",");
                    //    float mat[16];
                    //    for (int i = 0; i < 16; i++)
                    //    {
                    //        mat[i] = (splits[i]).toFloat();
                    //    }
                    //    ////获取旋转角度
                    //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                    //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                    //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                    //    //获取坐标
                    //    float X = mat[12];
                    //    float Y = mat[13];
                    //    float Z = mat[14];
                    //    //获取颜色
                    //    float R = (float)simpleEntity.getR() / 255;
                    //    float G = (float)simpleEntity.getG() / 255;
                    //    float B = (float)simpleEntity.getB() / 255;
                    //    float A = (float)simpleEntity.getA() / 255;
                    //    //获取长宽高
                    //    float L = (n.attribute("L")).toFloat();
                    //    float W = (n.attribute("W")).toFloat();
                    //    float T = (n.attribute("T")).toFloat();
                    //    float CS = (n.attribute("CS")).toFloat();
                    //    float N = (n.attribute("N")).toFloat();
                    //    float MH = (n.attribute("MH")).toFloat();
                    //    float D = (n.attribute("D")).toFloat();
                    //    //
                    //    glPushMatrix();
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    //    glColor4f(R, G, B, A);
                    //    CircularFixedPlate(L, W, T, CS, N, MH, D, X, Y, Z, rX, rY, rZ);
                    //    glPopMatrix();
                    //}
                }
            }
        }
        for (BoolEntity boolEntity : modFile.getBoolEntities()) {
            if (!boolEntity.getBoolType().isNull()) {
                if (boolEntity.getVisible() == ("true")) {
                    //qDebug() << 11 << endl;
                    if (boolEntity.getBoolType() == ("Difference")) {
                        QString  vs = boolEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        ////获取旋转角度
                        float rX1 = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY1 = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ1 = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X1 = mat[12];
                        float Y1 = mat[13];
                        float Z1 = mat[14];
                        //获取颜色
                        float R = (float)boolEntity.getR() / 255;
                        float G = (float)boolEntity.getG() / 255;
                        float B = (float)boolEntity.getB() / 255;
                        float A = (float)boolEntity.getA() / 255;
                        int  EntityId1 = boolEntity.getBoolEntityId1();
                        int  EntityId2 = boolEntity.getBoolEntityId2();
                        glPushMatrix();
                        //glTranslatef(X1, Z1, Y1);
                        glColor4f(R, G, B, A);
                        drawDifference(EntityId1, EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);

                        glPopMatrix();
                    }
                    if (boolEntity.getBoolType() == ("Union")) {
                        QString  vs = boolEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        //获取旋转角度
                        float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X1 = mat[12];
                        float Y1 = mat[13];
                        float Z1 = mat[14];
                        //获取颜色
                        float R = (float)boolEntity.getR() / 255;
                        float G = (float)boolEntity.getG() / 255;
                        float B = (float)boolEntity.getB() / 255;
                        float A = (float)boolEntity.getA() / 255;
                        int  EntityId1 = boolEntity.getBoolEntityId1();
                        int  EntityId2 = boolEntity.getBoolEntityId2();
                        glPushMatrix();

                        glColor4f(R, G, B, A);
                        draw(EntityId1, rX, rY, rZ, X1, Y1, Z1);
                        draw(EntityId2, rX, rY, rZ, X1, Y1, Z1);
                        glPopMatrix();
                    }
                    if (boolEntity.getBoolType() == ("Intersection")) {
                        QString  vs = boolEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        ////获取旋转角度
                        float rX1 = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY1 = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ1 = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X1 = mat[12];
                        float Y1 = mat[13];
                        float Z1 = mat[14];
                        //获取颜色
                        float R = (float)boolEntity.getR() / 255;
                        float G = (float)boolEntity.getG() / 255;
                        float B = (float)boolEntity.getB() / 255;
                        float A = (float)boolEntity.getA() / 255;
                        int  EntityId1 = boolEntity.getBoolEntityId1();
                        int  EntityId2 = boolEntity.getBoolEntityId2();
                        glPushMatrix();
                        glColor4f(R, G, B, A);
                        drawIntersection(EntityId1, EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);

                        glPopMatrix();
                    }
                }
            }
        }
    }
}
void myopengl::drawboolEntity2(QString devname)
{
    for (int i = 0; i < gimModFileList.size(); i++) {
        ModFile modFile = gimModFileList[i];
        if (QString::compare(gimModFileList[i].getObjFileName(), devname) == 0) {//选中
            for (BoolEntity boolEntity : modFile.getBoolEntities()) {
                if (!boolEntity.getBoolType().isNull()) {
                    if (boolEntity.getVisible() == ("true")) {
                        if (boolEntity.getBoolType() == ("Difference")) {
                            QString  vs = boolEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX1 = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY1 = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ1 = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X1 = mat[12];
                            float Y1 = mat[13];
                            float Z1 = mat[14];
                            //获取颜色
                           /* float R = (float)boolEntity.getR() / 255;
                            float G = (float)boolEntity.getG() / 255;
                            float B = (float)boolEntity.getB() / 255;
                            float A = (float)boolEntity.getA() / 255;*/
                            int  EntityId1 = boolEntity.getBoolEntityId1();
                            int  EntityId2 = boolEntity.getBoolEntityId2();
                            glPushMatrix();
                            //glTranslatef(X1, Z1, Y1);
                            glColor4f(0, 1, 1, 1);
                            drawDifference(EntityId1, EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);

                            glPopMatrix();
                        }
                        if (boolEntity.getBoolType() == ("Union")) {
                            QString  vs = boolEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            //获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X1 = mat[12];
                            float Y1 = mat[13];
                            float Z1 = mat[14];
                            //获取颜色
                           /* float R = (float)boolEntity.getR() / 255;
                            float G = (float)boolEntity.getG() / 255;
                            float B = (float)boolEntity.getB() / 255;
                            float A = (float)boolEntity.getA() / 255;*/
                            int  EntityId1 = boolEntity.getBoolEntityId1();
                            int  EntityId2 = boolEntity.getBoolEntityId2();
                            glPushMatrix();

                            glColor4f(0, 1, 1, 1);
                            draw(EntityId1, rX, rY, rZ, X1, Y1, Z1);
                            draw(EntityId2, rX, rY, rZ, X1, Y1, Z1);
                            glPopMatrix();
                        }
                        if (boolEntity.getBoolType() == ("Intersection")) {
                            QString  vs = boolEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX1 = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY1 = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ1 = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X1 = mat[12];
                            float Y1 = mat[13];
                            float Z1 = mat[14];
                            //获取颜色
                            /*float R = (float)boolEntity.getR() / 255;
                            float G = (float)boolEntity.getG() / 255;
                            float B = (float)boolEntity.getB() / 255;
                            float A = (float)boolEntity.getA() / 255;*/
                            int  EntityId1 = boolEntity.getBoolEntityId1();
                            int  EntityId2 = boolEntity.getBoolEntityId2();
                            glPushMatrix();
                            glColor4f(0, 1, 1, 1);
                            drawIntersection(EntityId1, EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);

                            glPopMatrix();
                        }
                    }
                }
            }
            for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
                if (!simpleEntity.getGraphicElement().isNull()) {
                    if (simpleEntity.getVisible() == ("true")) {
                        if (simpleEntity.getGraphicElement().nodeName() == ("TerminalBlock")) {

                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                           /* float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;*/
                            //获取长宽高
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            float T = (n.attribute("T")).toFloat();
                            float CL = (n.attribute("CL")).toFloat();
                            float CS = (n.attribute("CS")).toFloat();
                            float RS = (n.attribute("RS")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float CN = (n.attribute("CN")).toFloat();
                            float RN = (n.attribute("RN")).toFloat();
                            float BL = (n.attribute("BL")).toFloat();
                            //                  
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glColor4f(0, 1, 1, 1);
                            TerminalBlock(L, W, T, CL, CS, RS, R1, CN, RN, BL, X, Y, Z, rX, rY, rZ);
                            glPopMatrix();
                        }
                        //if (simpleEntity.getGraphicElement().nodeName() == ("Rectangular fixed plate")) {
                        //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                        //    QString  vs = simpleEntity.getTransformMatrix();
                        //    QStringList splits = vs.split(",");
                        //    float mat[16];
                        //    for (int i = 0; i < 16; i++)
                        //    {
                        //        mat[i] = (splits[i]).toFloat();
                        //    }
                        //    //获取旋转角度
                        //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //    //获取坐标
                        //    float X = mat[12];
                        //    float Y = mat[13];
                        //    float Z = mat[14];
                        //    //获取颜色
                        //    float R = (float)simpleEntity.getR() / 255;
                        //    float G = (float)simpleEntity.getG() / 255;
                        //    float B = (float)simpleEntity.getB() / 255;
                        //    float A = (float)simpleEntity.getA() / 255;
                        //    //获取长宽高
                        //    float L = (n.attribute("L")).toFloat();
                        //    float W = (n.attribute("W")).toFloat();
                        //    float T = (n.attribute("T")).toFloat();
                        //    float CS = (n.attribute("CS")).toFloat();
                        //    float RS = (n.attribute("RS")).toFloat();
                        //    float CN = (n.attribute("CN")).toFloat();
                        //    float RN = (n.attribute("RN")).toFloat();
                        //    float MH = (n.attribute("MH")).toFloat();
                        //    float D = (n.attribute("D")).toFloat();
                        //    //
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        //    glColor4f(R, G, B, A);
                        //    RectangularFixedPlate(L, W, T, CS, RS, CN, RN, MH, D, 0, 0, 0, rX, rY, rZ);
                        //    glPopMatrix();
                        //}
                        //if (simpleEntity.getGraphicElement().nodeName() == ("Circular fixed plate")) {
                        //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                        //    QString  vs = simpleEntity.getTransformMatrix();
                        //    QStringList splits = vs.split(",");
                        //    float mat[16];
                        //    for (int i = 0; i < 16; i++)
                        //    {
                        //        mat[i] = (splits[i]).toFloat();
                        //    }
                        //    ////获取旋转角度
                        //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //    //获取坐标
                        //    float X = mat[12];
                        //    float Y = mat[13];
                        //    float Z = mat[14];
                        //    //获取颜色
                        //    float R = (float)simpleEntity.getR() / 255;
                        //    float G = (float)simpleEntity.getG() / 255;
                        //    float B = (float)simpleEntity.getB() / 255;
                        //    float A = (float)simpleEntity.getA() / 255;
                        //    //获取长宽高
                        //    float L = (n.attribute("L")).toFloat();
                        //    float W = (n.attribute("W")).toFloat();
                        //    float T = (n.attribute("T")).toFloat();
                        //    float CS = (n.attribute("CS")).toFloat();
                        //    float N = (n.attribute("N")).toFloat();
                        //    float MH = (n.attribute("MH")).toFloat();
                        //    float D = (n.attribute("D")).toFloat();
                        //    //
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        //    glColor4f(R, G, B, A);
                        //    CircularFixedPlate(L, W, T, CS, N, MH, D, X, Y, Z, rX, rY, rZ);
                        //    glPopMatrix();
                        //}
                    }
                }
            }
        }
        else {
            for (BoolEntity boolEntity : modFile.getBoolEntities()) {
                if (!boolEntity.getBoolType().isNull()) {
                    if (boolEntity.getVisible() == ("true")) {
                        //qDebug() << 11 << endl;
                        if (boolEntity.getBoolType() == ("Difference")) {
                        QString  vs = boolEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        ////获取旋转角度
                        float rX1 = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY1 = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ1 = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X1 = mat[12];
                        float Y1 = mat[13];
                        float Z1 = mat[14];
                        //获取颜色
                        float R = (float)boolEntity.getR() / 255;
                        float G = (float)boolEntity.getG() / 255;
                        float B = (float)boolEntity.getB() / 255;
                        float A = (float)boolEntity.getA() / 255;
                        int  EntityId1 = boolEntity.getBoolEntityId1();
                        int  EntityId2 = boolEntity.getBoolEntityId2();
                        glPushMatrix();
                        //glTranslatef(X1, Z1, Y1);
                        glColor4f(R, G, B, A);
                        drawDifference(EntityId1, EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);

                        glPopMatrix();
                    }
                        if (boolEntity.getBoolType() == ("Union")) {
                        QString  vs = boolEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        //获取旋转角度
                        float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X1 = mat[12];
                        float Y1 = mat[13];
                        float Z1 = mat[14];
                        //获取颜色
                        float R = (float)boolEntity.getR() / 255;
                        float G = (float)boolEntity.getG() / 255;
                        float B = (float)boolEntity.getB() / 255;
                        float A = (float)boolEntity.getA() / 255;
                        int  EntityId1 = boolEntity.getBoolEntityId1();
                        int  EntityId2 = boolEntity.getBoolEntityId2();
                        glPushMatrix();

                        glColor4f(R, G, B, A);
                        draw(EntityId1, rX, rY, rZ, X1, Y1, Z1);
                        draw(EntityId2, rX, rY, rZ, X1, Y1, Z1);
                        glPopMatrix();
                    }
                        if (boolEntity.getBoolType() == ("Intersection")) {
                        QString  vs = boolEntity.getTransformMatrix();
                        QStringList splits = vs.split(",");
                        float mat[16];
                        for (int i = 0; i < 16; i++)
                        {
                            mat[i] = (splits[i]).toFloat();
                        }
                        ////获取旋转角度
                        float rX1 = (atan2(mat[9], mat[10])) * PI / 180;
                        float rY1 = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        float rZ1 = (atan2(mat[4], mat[0])) * PI / 180;
                        //获取坐标
                        float X1 = mat[12];
                        float Y1 = mat[13];
                        float Z1 = mat[14];
                        //获取颜色
                        float R = (float)boolEntity.getR() / 255;
                        float G = (float)boolEntity.getG() / 255;
                        float B = (float)boolEntity.getB() / 255;
                        float A = (float)boolEntity.getA() / 255;
                        int  EntityId1 = boolEntity.getBoolEntityId1();
                        int  EntityId2 = boolEntity.getBoolEntityId2();
                        glPushMatrix();
                        glColor4f(R, G, B, A);
                        drawIntersection(EntityId1, EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);

                        glPopMatrix();
                    }
                    }
                }
            }
            for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
                if (!simpleEntity.getGraphicElement().isNull()) {
                    if (simpleEntity.getVisible() == ("true")) {
                        if (simpleEntity.getGraphicElement().nodeName() == ("TerminalBlock")) {

                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                            float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            float T = (n.attribute("T")).toFloat();
                            float CL = (n.attribute("CL")).toFloat();
                            float CS = (n.attribute("CS")).toFloat();
                            float RS = (n.attribute("RS")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float CN = (n.attribute("CN")).toFloat();
                            float RN = (n.attribute("RN")).toFloat();
                            float BL = (n.attribute("BL")).toFloat();
                            //                  
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glColor4f(R, G, B, A);
                            TerminalBlock(L, W, T, CL, CS, RS, R1, CN, RN, BL, X, Y, Z, rX, rY, rZ);
                            glPopMatrix();
                        }
                        //if (simpleEntity.getGraphicElement().nodeName() == ("Rectangular fixed plate")) {
                    //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                    //    QString  vs = simpleEntity.getTransformMatrix();
                    //    QStringList splits = vs.split(",");
                    //    float mat[16];
                    //    for (int i = 0; i < 16; i++)
                    //    {
                    //        mat[i] = (splits[i]).toFloat();
                    //    }
                    //    //获取旋转角度
                    //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                    //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                    //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                    //    //获取坐标
                    //    float X = mat[12];
                    //    float Y = mat[13];
                    //    float Z = mat[14];
                    //    //获取颜色
                    //    float R = (float)simpleEntity.getR() / 255;
                    //    float G = (float)simpleEntity.getG() / 255;
                    //    float B = (float)simpleEntity.getB() / 255;
                    //    float A = (float)simpleEntity.getA() / 255;
                    //    //获取长宽高
                    //    float L = (n.attribute("L")).toFloat();
                    //    float W = (n.attribute("W")).toFloat();
                    //    float T = (n.attribute("T")).toFloat();
                    //    float CS = (n.attribute("CS")).toFloat();
                    //    float RS = (n.attribute("RS")).toFloat();
                    //    float CN = (n.attribute("CN")).toFloat();
                    //    float RN = (n.attribute("RN")).toFloat();
                    //    float MH = (n.attribute("MH")).toFloat();
                    //    float D = (n.attribute("D")).toFloat();
                    //    //
                    //    glPushMatrix();
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    //    glColor4f(R, G, B, A);
                    //    RectangularFixedPlate(L, W, T, CS, RS, CN, RN, MH, D, 0, 0, 0, rX, rY, rZ);
                    //    glPopMatrix();
                    //}
                    //if (simpleEntity.getGraphicElement().nodeName() == ("Circular fixed plate")) {
                    //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                    //    QString  vs = simpleEntity.getTransformMatrix();
                    //    QStringList splits = vs.split(",");
                    //    float mat[16];
                    //    for (int i = 0; i < 16; i++)
                    //    {
                    //        mat[i] = (splits[i]).toFloat();
                    //    }
                    //    ////获取旋转角度
                    //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                    //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                    //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                    //    //获取坐标
                    //    float X = mat[12];
                    //    float Y = mat[13];
                    //    float Z = mat[14];
                    //    //获取颜色
                    //    float R = (float)simpleEntity.getR() / 255;
                    //    float G = (float)simpleEntity.getG() / 255;
                    //    float B = (float)simpleEntity.getB() / 255;
                    //    float A = (float)simpleEntity.getA() / 255;
                    //    //获取长宽高
                    //    float L = (n.attribute("L")).toFloat();
                    //    float W = (n.attribute("W")).toFloat();
                    //    float T = (n.attribute("T")).toFloat();
                    //    float CS = (n.attribute("CS")).toFloat();
                    //    float N = (n.attribute("N")).toFloat();
                    //    float MH = (n.attribute("MH")).toFloat();
                    //    float D = (n.attribute("D")).toFloat();
                    //    //
                    //    glPushMatrix();
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    //    glColor4f(R, G, B, A);
                    //    CircularFixedPlate(L, W, T, CS, N, MH, D, X, Y, Z, rX, rY, rZ);
                    //    glPopMatrix();
                    //}
                    }
                }
            }
        }
    }
}
void myopengl::drawDifference(int EntityId1, int EntityId2, GLfloat rX, GLfloat rY, GLfloat rZ, GLfloat X1, GLfloat Y1, GLfloat Z1)
{
    glPushMatrix();
    clearPrimtives();
    if (!glIsList((GLuint)(100000000))) {
        glNewList(100000000, GL_COMPILE);

        glPushMatrix();
        draw(EntityId1, rX, rY, rZ, X1, Y1, Z1);
        glPopMatrix();
        glEndList();
    }
    if (!glIsList((GLuint)(100000001))) {
        glNewList(100000001, GL_COMPILE);

        glPushMatrix();

        draw(EntityId2, rX, rY, rZ, X1, Y1, Z1);
        glPopMatrix();
        glEndList();
    }
    primitives.push_back(new OpenCSG::DisplayListPrimitive(100000000, OpenCSG::Intersection, 1));
    primitives.push_back(new OpenCSG::DisplayListPrimitive(100000001, OpenCSG::Subtraction, 1));
    OpenCSG::render(primitives);
    glDepthFunc(GL_EQUAL);
    for (std::vector<OpenCSG::Primitive*>::const_iterator j = primitives.begin(); j != primitives.end(); ++j) {
        (*j)->render();
    }
    glDepthFunc(GL_LESS);
    glPopMatrix();
}
void myopengl::drawIntersection(int EntityId1, int EntityId2, GLfloat rX1, GLfloat rY1, GLfloat rZ1, GLfloat X1, GLfloat Y1, GLfloat Z1)
{
    glPushMatrix();
    clearPrimtives();
    if (!glIsList((GLuint)(100000017))) {
        glNewList(100000017, GL_COMPILE);

        glPushMatrix();
        draw(EntityId1, rX1, rY1, rZ1, X1, Y1, Z1);
        glPopMatrix();
        glEndList();
    }
    if (!glIsList((GLuint)(100000018))) {
        glNewList(100000018, GL_COMPILE);

        glPushMatrix();

        draw(EntityId2, rX1, rY1, rZ1, X1, Y1, Z1);
        glPopMatrix();
        glEndList();
    }
    primitives.push_back(new OpenCSG::DisplayListPrimitive(100000017, OpenCSG::Intersection, 1));
    primitives.push_back(new OpenCSG::DisplayListPrimitive(100000018, OpenCSG::Intersection, 1));

    OpenCSG::render(primitives);
    glDepthFunc(GL_EQUAL);
    for (std::vector<OpenCSG::Primitive*>::const_iterator j = primitives.begin(); j != primitives.end(); ++j) {
        (*j)->render();
    }
    glDepthFunc(GL_LESS);
    glPopMatrix();

}
void myopengl::draw(int EntityId, GLfloat rX1, GLfloat rY1, GLfloat rZ1, GLfloat X1, GLfloat Y1, GLfloat Z1)
{
    for (int i = 0; i < gimModFileList.size(); i++) {
        ModFile modFile = gimModFileList[i];
        for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
            if (!simpleEntity.getGraphicElement().isNull()) {
                if (simpleEntity.getVisible() != ("true")) {
                    if (simpleEntity.getEntityId() == EntityId) {
                        if ((simpleEntity.getGraphicElement().nodeName() == ("Cuboid"))) {

                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];


                            //获取长宽高

                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();
                            float H = (n.attribute("H")).toFloat();


                            //绘制线框
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            Cuboid(L, W, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                            //绘制实体
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            Cuboid(L, W, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("Sphere")) {

                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float R1 = (n.attribute("R")).toFloat();


                            //GLuint id2 = glGenLists(1);
                           // glNewList(id2, GL_COMPILE);
                            //绘制线框
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Sphere(R1);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //绘制实体
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            Sphere(R1);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("RotationalEllipsoid"))
                        {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            ////获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float LR = (n.attribute("LR")).toFloat();
                            float WR = (n.attribute("WR")).toFloat();
                            float H = (n.attribute("H")).toFloat();


                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            RotationalEllipsoid(LR, WR, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            RotationalEllipsoid(LR, WR, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("Table")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            ////获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float TL1 = (n.attribute("TL1")).toFloat();
                            float TL2 = (n.attribute("TL2")).toFloat();
                            float LL1 = (n.attribute("LL1")).toFloat();
                            float LL2 = (n.attribute("LL2")).toFloat();
                            float H = (n.attribute("H")).toFloat();

                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Table(TL1, TL2, LL1, LL2, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            Table(TL1, TL2, LL1, LL2, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("OffsetRectangularTable"))
                        {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            ////获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float TL = (n.attribute("TL")).toFloat();
                            float TW = (n.attribute("TW")).toFloat();
                            float LL = (n.attribute("LL")).toFloat();
                            float LW = (n.attribute("LW")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float XOFF = (n.attribute("XOFF")).toFloat();
                            float YOFF = (n.attribute("YOFF")).toFloat();

                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            // glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            OffsetRectangularTable(TL, TW, LL, LW, H, XOFF, YOFF);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("Cylinder"))
                        {

                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float R1 = (n.attribute("R")).toFloat();
                            float H = (n.attribute("H")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Cylinder(R1, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            Cylinder(R1, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("TruncatedCone")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float TR = (n.attribute("TR")).toFloat();
                            float BR = (n.attribute("BR")).toFloat();
                            float H = (n.attribute("H")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            /// glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            TruncatedCone(TR, BR, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            TruncatedCone(TR, BR, H);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("EccentricTruncatedCone")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float TR = (n.attribute("TR")).toFloat();
                            float BR = (n.attribute("BR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float TOPXOFF = (n.attribute("TOPXOFF")).toFloat();
                            float TOPYOFF = (n.attribute("TOPYOFF")).toFloat();

                            ////1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            // glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            EccentricTruncatedCone(TR, BR, H, TOPXOFF, TOPYOFF);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("Ring")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float DR = (n.attribute("DR")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();


                            ////1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Ring(DR, R1, Rad);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            Ring(DR, R1, Rad);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("RectangularRing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float DR = (n.attribute("DR")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            RectangularRing(DR, R1, L, W);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            RectangularRing(DR, R1, L, W);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("EllipticRing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float DR = (n.attribute("DR")).toFloat();

                            float L = (n.attribute("L")).toFloat();
                            float W = (n.attribute("W")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            EllipticRing(DR, L, W);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            EllipticRing(DR, L, W);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("CircularGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float OR = (n.attribute("OR")).toFloat();
                            float IR = (n.attribute("IR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            CircularGasket(OR, IR, H, Rad);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            CircularGasket(OR, IR, H, Rad);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("TableGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float TR = (n.attribute("TR")).toFloat();
                            float OR = (n.attribute("OR")).toFloat();
                            float IR = (n.attribute("IR")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            TableGasket(TR, OR, IR, H, Rad);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            TableGasket(TR, OR, IR, H, Rad);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("SquareGasket")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float L1 = (n.attribute("L1")).toFloat();
                            float W1 = (n.attribute("W1")).toFloat();
                            float L2 = (n.attribute("L2")).toFloat();
                            float W2 = (n.attribute("W2")).toFloat();
                            float H = (n.attribute("H")).toFloat();
                            float CT = (n.attribute("CT")).toFloat();
                            float Rad = (n.attribute("Rad")).toFloat();
                            float CL = (n.attribute("CL")).toFloat();


                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            SquareGasket(L1, W1, L2, W2, H, CT, Rad, CL);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("PorcelainBushing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float H = (n.attribute("H")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float R2 = (n.attribute("R1")).toFloat();
                            float R3 = (n.attribute("R2")).toFloat();
                            float N = (n.attribute("N")).toFloat();



                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("ConePorcelainBushing")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            //获取颜色
                           /* float R = (float)simpleEntity.getR() / 255;
                            float G = (float)simpleEntity.getG() / 255;
                            float B = (float)simpleEntity.getB() / 255;
                            float A = (float)simpleEntity.getA() / 255;*/
                            //获取长宽高

                            float H = (n.attribute("H")).toFloat();
                            float R1 = (n.attribute("R")).toFloat();
                            float R2 = (n.attribute("R1")).toFloat();
                            float R3 = (n.attribute("R2")).toFloat();
                            float N = (n.attribute("N")).toFloat();

                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            PorcelainBushing(H, R1, R2, R3, N);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                        }
                        if (simpleEntity.getGraphicElement().nodeName() == ("Insulator")) {
                            QDomElement n = simpleEntity.getGraphicElement().toElement();
                            QString  vs = simpleEntity.getTransformMatrix();
                            QStringList splits = vs.split(",");
                            float mat[16];
                            for (int i = 0; i < 16; i++)
                            {
                                mat[i] = (splits[i]).toFloat();
                            }
                            ////获取旋转角度
                            float rX = (atan2(mat[9], mat[10])) * PI / 180;
                            float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                            float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                            //获取坐标
                            float X = mat[12];
                            float Y = mat[13];
                            float Z = mat[14];
                            ////获取颜色
                            //float R = (float)simpleEntity.getR() / 255;
                            //float G = (float)simpleEntity.getG() / 255;
                            //float B = (float)simpleEntity.getB() / 255;
                            //float A = (float)simpleEntity.getA() / 255;
                            //获取长宽高

                            float N = (n.attribute("N")).toFloat();
                            float D = (n.attribute("D")).toFloat();
                            float N1 = (n.attribute("N1")).toFloat();
                            float H1 = (n.attribute("H1")).toFloat();
                            float R1 = (n.attribute("R1")).toFloat();
                            float R2 = (n.attribute("R2")).toFloat();
                            float R3 = (n.attribute("R")).toFloat();
                            //float FL = (n.attribute("FL")).toFloat();
                            //float AL = (n.attribute("AL")).toFloat();
                            //float LN = (n.attribute("LN")).toFloat();


                            //1
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                            Insulator(N, D, N1, H1, R1, R2, R3);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();
                            //2
                            glPushMatrix();
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            glPushMatrix();
                            glTranslatef(X1, Z1, Y1);
                            glRotatef(rX1, 1, 0, 0);
                            glRotatef(rY1, 0, 1, 0);
                            glRotatef(rZ1, 0, 0, 1);
                            glPushMatrix();
                            glTranslatef(X, Z, Y);
                            glRotatef(rX, 1, 0, 0);
                            glRotatef(rY, 0, 1, 0);
                            glRotatef(rZ, 0, 0, 1);
                            //glColor4f(R, G, B, A);
                            Insulator(N, D, N1, H1, R1, R2, R3);
                            glPopMatrix();
                            glPopMatrix();
                            glPopMatrix();

                        }
                        //if (simpleEntity.getGraphicElement().nodeName() == ("TerminalBlock") && simpleEntity.getEntityId() == EntityId) {
                        //    
                        //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                        //    QString  vs = simpleEntity.getTransformMatrix();
                        //    QStringList splits = vs.split(",");
                        //    float mat[16];
                        //    for (int i = 0; i < 16; i++)
                        //    {
                        //        mat[i] = (splits[i]).toFloat();
                        //    }
                        //    ////获取旋转角度
                        //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //    //获取坐标
                        //    float X = mat[12];
                        //    float Y = mat[13];
                        //    float Z = mat[14];
                        //    ////获取颜色
                        //    //float R = (float)simpleEntity.getR() / 255;
                        //    //float G = (float)simpleEntity.getG() / 255;
                        //    //float B = (float)simpleEntity.getB() / 255;
                        //    //float A = (float)simpleEntity.getA() / 255;
                        //    //获取长宽高
                        //    float L = (n.attribute("L")).toFloat();
                        //    float W = (n.attribute("W")).toFloat();
                        //    float T = (n.attribute("T")).toFloat();
                        //    float CL = (n.attribute("CL")).toFloat();
                        //    float CS = (n.attribute("CS")).toFloat();
                        //    float RS = (n.attribute("RS")).toFloat();
                        //    float R1 = (n.attribute("R")).toFloat();
                        //    float CN = (n.attribute("CN")).toFloat();
                        //    float RN = (n.attribute("RN")).toFloat();
                        //    float BL = (n.attribute("BL")).toFloat();
                        //    //GLuint id18 = glGenLists(1);
                        //    //glNewList(id18, GL_COMPILE);
                        //    //1
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        //    glTranslatef(X, Z, Y);
                        //    glRotatef(rX, 1, 0, 0);
                        //    glRotatef(rY, 0, 1, 0);
                        //    glRotatef(rZ, 0, 0, 1);
                        //    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                        //    TerminalBlock(L, W, T, CL, CS, RS, R1,CN,RN,BL, X,  Y,  Z, rX, rY, rZ);
                        //    glPopMatrix();
                        //    //2
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        //    glTranslatef(X, Z, Y);
                        //    glRotatef(rX, 1, 0, 0);
                        //    glRotatef(rY, 0, 1, 0);
                        //    glRotatef(rZ, 0, 0, 1);
                        //    //glColor4f(R, G, B, A);
                        //    TerminalBlock(L, W, T, CL, CS, RS, R1, CN, RN,BL, X, Y, Z, rX, rY, rZ);
                        //    glPopMatrix();
                        //    // glEndList();
                        //}
                        //if (simpleEntity.getGraphicElement().nodeName() == ("Rectangular fixed plate") && simpleEntity.getEntityId() == EntityId) {
                        //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                        //    QString  vs = simpleEntity.getTransformMatrix();
                        //    QStringList splits = vs.split(",");
                        //    float mat[16];
                        //    for (int i = 0; i < 16; i++)
                        //    {
                        //        mat[i] = (splits[i]).toFloat();
                        //    }
                        //    ////获取旋转角度
                        //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //    //获取坐标
                        //    float X = mat[12];
                        //    float Y = mat[13];
                        //    float Z = mat[14];
                        //    ////获取颜色
                        //   /* float R = (float)simpleEntity.getR() / 255;
                        //    float G = (float)simpleEntity.getG() / 255;
                        //    float B = (float)simpleEntity.getB() / 255;
                        //    float A = (float)simpleEntity.getA() / 255;*/
                        //    //获取长宽高
                        //    float L = (n.attribute("L")).toFloat();
                        //    float W = (n.attribute("W")).toFloat();
                        //    float T = (n.attribute("T")).toFloat();
                        //    float CS = (n.attribute("CS")).toFloat();
                        //    float RS = (n.attribute("RS")).toFloat();
                        //    float CN = (n.attribute("CN")).toFloat();
                        //    float RN = (n.attribute("RN")).toFloat();
                        //    float MH = (n.attribute("MH")).toFloat();
                        //    float D = (n.attribute("D")).toFloat();
                        //    //GLuint id19 = glGenLists(1);
                        //    //glNewList(id19, GL_COMPILE);
                        //    //1
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        //    
                        //    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                        //    RectangularFixedPlate(L, W, T, CS, RS, CN, RN, MH, D,  X,  Y,  Z,  rX,  rY,  rZ);
                        //    glPopMatrix();
                        //    //2
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        //    
                        //    
                        //    //glColor4f(R, G, B, A);
                        //     RectangularFixedPlate(L, W, T, CS, RS, CN, RN, MH, D, X, Y, Z, rX, rY, rZ);
                        //    glPopMatrix();
                        //    //glEndList();
                        //}
                        //if (simpleEntity.getGraphicElement().nodeName() == ("Circular fixed plate") && simpleEntity.getEntityId() == EntityId) {
                        //    QDomElement n = simpleEntity.getGraphicElement().toElement();
                        //    QString  vs = simpleEntity.getTransformMatrix();
                        //    QStringList splits = vs.split(",");
                        //    float mat[16];
                        //    for (int i = 0; i < 16; i++)
                        //    {
                        //        mat[i] = (splits[i]).toFloat();
                        //    }
                        //    ////获取旋转角度
                        //    float rX = (atan2(mat[9], mat[10])) * PI / 180;
                        //    float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
                        //    float rZ = (atan2(mat[4], mat[0])) * PI / 180;
                        //    //获取坐标
                        //    float X = mat[12];
                        //    float Y = mat[13];
                        //    float Z = mat[14];
                        //    ////获取颜色
                        //    //float R = (float)simpleEntity.getR() / 255;
                        //    //float G = (float)simpleEntity.getG() / 255;
                        //    //float B = (float)simpleEntity.getB() / 255;
                        //    //float A = (float)simpleEntity.getA() / 255;
                        //    //获取长宽高
                        //    float L = (n.attribute("L")).toFloat();
                        //    float W = (n.attribute("W")).toFloat();
                        //    float T = (n.attribute("T")).toFloat();
                        //    float CS = (n.attribute("CS")).toFloat();
                        //    float N = (n.attribute("N")).toFloat();
                        //    float MH = (n.attribute("MH")).toFloat();
                        //    float D = (n.attribute("D")).toFloat();
                        //    //GLuint id20 = glGenLists(1);
                        //   // glNewList(id20, GL_COMPILE);
                        //    //1
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        //    glTranslatef(X, Z, Y);
                        //    glRotatef(rX, 1, 0, 0);
                        //    glRotatef(rY, 0, 1, 0);
                        //    glRotatef(rZ, 0, 0, 1);
                        //    glColor4f(0.0F, 0.0F, 0.0F, 1.0F);
                        //    CircularFixedPlate(L, W, T, CS, N, MH, D, X, Y, Z, rX, rY, rZ);
                        //    glPopMatrix();
                        //    //2
                        //    glPushMatrix();
                        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        //    glTranslatef(X, Z, Y);
                        //    glRotatef(rX, 1, 0, 0);
                        //    glRotatef(rY, 0, 1, 0);
                        //    glRotatef(rZ, 0, 0, 1);
                        //    //glColor4f(R, G, B, A);
                        //    CircularFixedPlate(L, W, T, CS, N, MH, D, X, Y, Z, rX, rY, rZ);
                        //    glPopMatrix();
                        //    //glEndList();
                        //}

                    }
                }

            }
        }


    }
}

void myopengl::processHits(GLint hits, GLuint selectBuffer[])
{
    GLint i;
    GLuint name;
    for (i = 0; i < hits; i++)
    {
        name = selectBuffer[3 + i * 4];
        select_point = name;//每个选中物体有占用名字栈中4个单位，第4个是物体名字值
        qDebug() << "第" << name << "个点" << endl;
    }
}
void myopengl::clearPrimtives()
{
    for (std::vector<OpenCSG::Primitive*>::const_iterator i = primitives.begin(); i != primitives.end(); ++i) {
        OpenCSG::DisplayListPrimitive* p = static_cast<OpenCSG::DisplayListPrimitive*>(*i);
        glDeleteLists(1, p->getDisplayListId());
        delete p;
    }
    primitives.clear();
}