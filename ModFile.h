#pragma once
#include "SimpleEntity.h"
#include "BoolEntity.h"
#include"GimFile.h"

class ModFile :public GimFile
{
public:

    QList<SimpleEntity> getSimpleEntities() {
        return simpleEntities;
    }
    void setSimpleEntities(QList<SimpleEntity> simpleEntities) {
        this->simpleEntities = simpleEntities;
    }

    QList<BoolEntity> getBoolEntities() {
        return boolEntities;
    }
    void setBoolEntities(QList<BoolEntity> boolEntities) {
        this->boolEntities = boolEntities;
    }


    QString getF1SysFileName() {
        return f1SysFileName;
    }
    void setF1SysFileName(QString f1SysFileName) {
        this->f1SysFileName = f1SysFileName;
    }

    QString getF2SysFileName() {
        return f2SysFileName;
    }
    void setF2SysFileName(QString f2SysFileName) {
        this->f2SysFileName = f2SysFileName;
    }

    QString getF3SysFileName() {
        return f3SysFileName;
    }
    void setF3SysFileName(QString f3SysFileName) {
        this->f3SysFileName = f3SysFileName;
    }

    QString getF4SysFileName() {
        return f4SysFileName;
    }
    void setF4SysFileName(QString f4SysFileName) {
        this->f4SysFileName = f4SysFileName;
    }

    QString getPartIndexFileName() {
        return partIndexFileName;
    }
    void setPartIndexFileName(QString partIndexFileName) {
        this->partIndexFileName = partIndexFileName;
    }

    QString getObjFileName() {
        return objFileName;
    }
    void setObjFileName(QString objFileName) {
        this->objFileName = objFileName;
    }

    QString getSymbolName() {
        return symbolName;
    }
    void setSymbolName(QString symbolName) {
        this->symbolName = symbolName;
    }

    QString gettransformMatrix1() {
        return transformMatrix1;
    }
    void settransformMatrix1(QString transformMatrix1) {
        this->transformMatrix1 = transformMatrix1;
    }

    QString gettransformMatrix2() {
        return transformMatrix2;
    }
    void settransformMatrix2(QString transformMatrix2) {
        this->transformMatrix2 = transformMatrix2;
    }
private:
    ////********** phm下读取内容
    //SolidModel<class T> parent;
    ////mod的文件名
    //QString modFileName;
    QString f1SysFileName;
    QString f2SysFileName;
    QString f3SysFileName;
    QString f4SysFileName;
    QString partIndexFileName;
    QString objFileName;
    QString symbolName;
    ////空间变换矩阵
    //QString transformMatrix;
    ////颜色
    //QString color;
    //********** .mod下读取内容
    //设备坐标
    QString transformMatrix1;//部件坐标
    QString transformMatrix2;//设备坐标
    //类型为simple的entity
    QList<SimpleEntity> simpleEntities;
    //类型为boolean的entity
    QList<BoolEntity> boolEntities;
};