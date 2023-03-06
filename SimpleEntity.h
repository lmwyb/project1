#pragma once
#include <QDomNode>
#include <rpcndr.h>
using namespace std;
class SimpleEntity
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

    QDomNode getGraphicElement() {
        return graphicElement;
    }

    void setGraphicElement(QDomNode graphicElement) {
        this->graphicElement = graphicElement;
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

    QString visible;//  «∑Òœ‘ æ

    QString transformMatrix;

    QDomNode graphicElement;

    int R;
    int G;
    int B;
    int A;

};