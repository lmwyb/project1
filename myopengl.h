#pragma once
#ifdef _MSC_VER
#pragma once
#endif
#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QtWidgets/qopenglwidget.h>
#include<qopenglfunctions_3_3_compatibility.h>
#include<qopenglfunctions_4_3_compatibility.h>
#include <QtCore/qtimer.h>
#include <string>
#include <fstream>//使用ifstream打开文件流，成功则存在，失败则不存在;
#include <sstream>
#include <iostream>
#include <vector>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"displaylistPrimitive.h"
#include "GimReader.h"
#include<qobject.h>
#include<QtGui/QKeyEvent>
#include<qwidget.h>

#define SIZE 500
extern int name;
class myopengl : public QOpenGLWidget//, protected QOpenGLFunctions_3_3_Compatibility
{
    Q_OBJECT
public:
    myopengl(QWidget* parent = 0);
    ~myopengl();



public://对3个纯虚函数的重定义
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void CreateDisplayLists1();
    void CreateDisplayLists2();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void pick();

    void Axis();
    void Cylinder(GLfloat R, GLfloat H);
    void Cuboid(GLfloat L, GLfloat W, GLfloat H);
    void Sphere(GLfloat radius);
    void RotationalEllipsoid(GLfloat RL, GLfloat WR, GLfloat H);
    void Table(GLfloat TL1, GLfloat TL2, GLfloat LL1, GLfloat LL2, GLfloat H);
    void OffsetRectangularTable(GLfloat TL, GLfloat TW, GLfloat LL, GLfloat LW, GLfloat H, GLfloat XOFF, GLfloat YOFF);
    void TruncatedCone(GLfloat TR, GLfloat BR, GLfloat H);
    void EccentricTruncatedCone(GLfloat TR, GLfloat BR, GLfloat H, GLfloat TOPXOFF, GLfloat TOPYOFF);
    void Ring(GLfloat DR, GLfloat R, GLfloat Rad);
    void RectangularRing(GLfloat DR, GLfloat R, GLfloat L, GLfloat W);
    void EllipticRing(GLfloat DR, GLfloat L, GLfloat W);
    void CircularGasket(GLfloat IR, GLfloat OR, GLfloat H, GLfloat Rad);
    void TableGasket(GLfloat TR, GLfloat OR, GLfloat IR, GLfloat H, GLfloat Rad);
    void SquareGasket(GLfloat L1, GLfloat W1, GLfloat L2, GLfloat W2, GLfloat H, GLfloat CT, GLfloat R, GLfloat CL);
    void StretchedBody();
    void PorcelainBushing(GLfloat H, GLfloat R, GLfloat R1, GLfloat R2, GLfloat N);
    void ConePorcelainBushing(GLfloat H, GLfloat BR, GLfloat TR, GLfloat BR1, GLfloat BR2, GLfloat TR1, GLfloat TR2, GLfloat N);
    void Insulator(int N, GLfloat D, int N1, GLfloat H1, GLfloat R1, GLfloat R2, GLfloat R);
    void VTypeInsulator(GLfloat X, GLfloat AD, int N1, GLfloat H1, GLfloat R, GLfloat R1, GLfloat R2, GLfloat FL, GLfloat AL);
    void IrregularCuboid(GLfloat L, GLfloat W, GLfloat H, GLfloat CL);
    void TerminalBlock(GLfloat L, GLfloat W, GLfloat T, GLfloat CL, GLfloat CS, GLfloat RS, GLfloat R1, int CN, int RN, GLfloat BL, GLfloat X, GLfloat Y, GLfloat Z, GLfloat rX, GLfloat rY, GLfloat rZ);

    void drawboolEntity();
    void drawboolEntity2(QString devname);
    void drawDifference(int EntityId1, int EntityId2, GLfloat rX, GLfloat rY, GLfloat rZ, GLfloat X1, GLfloat Y1, GLfloat Z1);
    void drawIntersection(int EntityId1, int EntityId2, GLfloat rX1, GLfloat rY1, GLfloat rZ1, GLfloat X1, GLfloat Y1, GLfloat Z1);
    void draw(int EntityId, GLfloat rX1, GLfloat rY1, GLfloat rZ1, GLfloat X1, GLfloat Y1, GLfloat Z1);

    void processHits(GLint hits, GLuint selectBuffer[]);
    void clearPrimtives();
public://声明变量
    //void  drawObjects()  const;

    //GLdouble cameraPos[3] = { 0.0f, 1.0f, 50.0f };//摄像机位置
    //GLdouble cameraFront[3] = { 0.0f, 0.0f, -1.0f };//方向向量，摄像机位置+方向向量能保证无论怎么移动，摄像机都会注视着目标方向
    //GLdouble cameraUp[3] = { 0.0f, 1.0f, 0.0f };
    //GLdouble cameraRight[3];
    //GLdouble normalizedRight[3];
    //GLdouble front[3];

    GLfloat speed;
    GLfloat yaw;
    GLfloat pitch;

    GLfloat ScalefX;
    GLfloat ScalefY;
    GLfloat ScalefZ;

    QPoint prepoint;
    GLfloat m_rtri;                                 //控制三角形的角度
    QTimer* pTimer;

    GLboolean ispressed;
    GLint select_point;

    GLint viewport[4];
    GLint hits;
    //GLuint selectBuffer[SIZE];
    using  uint = unsigned   int;
    static   const   int  selectBufferSize = 100;
    std::vector<uint> selectBuffer = std::vector<uint>(selectBufferSize);

    float cameraSpeed = 1.0f;
    glm::mat4 modelview;
    glm::mat4 projection;
    glm::vec3 camerapos = glm::vec3(-3.0f, -1.0f, 20.0f);
    glm::vec3 camerafront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraup = glm::vec3(0.0f, 1.0f, 0.0f);
    //QList<ModFile> modFileList;

    std::vector<OpenCSG::Primitive*>primitives;
};

#endif 