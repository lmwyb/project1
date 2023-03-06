#pragma once
#pragma once
#include <rpcndr.h>
#include<qdebug.h>
#include<qstring.h>
#include<glm/mat4x4.hpp>
class BoolEntity
{
public:


    int getEntityId() {
        return entityId;
    }

    void setEntityId(int entityId) {
        this->entityId = entityId;
    }

    QString getVisible() {
        return visible;
    }

    void setVisible(QString visible) {
        this->visible = visible;
    }

    QString getTransformMatrix() {
        return transformMatrix;
    }

    void setTransformMatrix(QString transformMatrix) {
        this->transformMatrix = transformMatrix;
    }

    QString getBoolType() {
        return boolType;
    }

    void setBoolType(QString boolType) {
        this->boolType = boolType;
    }

    int getBoolEntityId1() {
        return boolEntityId1;
    }

    void setBoolEntityId1(int boolEntityId1) {
        this->boolEntityId1 = boolEntityId1;
    }

    int getBoolEntityId2() {
        return boolEntityId2;
    }

    void setBoolEntityId2(int boolEntityId2) {
        this->boolEntityId2 = boolEntityId2;
    }

    int getR() {
        return R;
    }

    void setR(int r) {
        R = r;
    }

    int getG() {
        return G;
    }

    void setG(int g) {
        G = g;
    }

    int getB() {
        return B;
    }

    void setB(int b) {
        B = b;
    }

    int getA() {
        return A;
    }

    void setA(int a) {
        A = a;
    }


private:
    int entityId;

    QString visible;

    QString transformMatrix;

    QString boolType; //Intersection——求交，Union——求并，Difference——求差
    int boolEntityId1;
    int boolEntityId2;

    int R;
    int G;
    int B;
    int A;

};