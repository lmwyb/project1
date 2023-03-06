#pragma once
#include <QtCore/qstring.h>

#include <iostream>
#include"GimFile.h"
#include"qlist.h"
class ObjectModel :public GimFile
{


public:

    QString getSymbolName() {
        return symbolName;
    }

    void setSymbolName(QString symbolName) {
        this->symbolName = symbolName;
    }

    QList<QString> getSubObjectModels() {
        return subObjectModels;
    }

    void setSubObjectModels(QList<QString> subObjectModels) {
        this->subObjectModels = subObjectModels;
    }

    QString getFamFileName() {
        return famFileName;
    }

    void setFamFileName(QString famFileName) {
        this->famFileName = famFileName;
    }

    QList<QString> getSubObjTransformMatrix() {
        return subObjTransformMatrix;
    }

    void setSubObjTransformMatrix(QList<QString> subObjTransformMatrix) {
        this->subObjTransformMatrix = subObjTransformMatrix;
    }

    QList<QString> getSolidModels() {
        return solidModels;
    }

    void setSolidModels(QList<QString> solidModels) {
        this->solidModels = solidModels;
    }

    QList<QString> getSolidTransformMatrix() {
        return solidTransformMatrix;
    }

    void setSolidTransformMatrix(QList<QString> solidTransformMatrix) {
        this->solidTransformMatrix = solidTransformMatrix;
    }


private:
    ////********** 四级设备级.cbm/部件索引.cbm/物理模型.dev下读取内容
    //T parent;
    ////物理模型.dev的文件名
    //QString objectFileName;

    ////物理模型的空间变换矩阵
    //QString transformMatrix;

    ////********** 物理模型.dev下读取内容

    ////物理模型fam的内容
    //QList<TripletPara> objectParas;
    QString famFileName;

    //dev对应的电气设备的模型名称
    QString symbolName;


    //引用的次级物理模型dev
    QList<QString> subObjectModels;

    //次级物理模型的空间变换矩阵
    //    private List<String> transformMatrix;
    QList<QString> subObjTransformMatrix;

    //引用的phm文件
    QList<QString> solidModels;

    //phm模型的空间变换矩阵
    //    private List<String> transformMatrixPHM;
    QList<QString> solidTransformMatrix;
};