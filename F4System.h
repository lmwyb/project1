#pragma once
#include"GimFile.h"
class F4System :public GimFile
{
public:
    F4System() {
        QList<QString> subDevice;
        QList<QString> subLogicalModels;
    }


    QString getSysClassifyName() {
        return sysClassifyName;
    }

    void setSysClassifyName(QString sysClassifyName) {
        this->sysClassifyName = sysClassifyName;
    }

    QString getFamFileName() {
        return famFileName;
    }

    void setFamFileName(QString famFileName) {
        this->famFileName = famFileName;
    }

    QString getTransformMatrix() {
        return transformMatrix;
    }

    void setTransformMatrix(QString transformMatrix) {
        this->transformMatrix = transformMatrix;
    }

    QString getObjectModel() {
        return objectModel;
    }

    void setObjectModel(QString objectModel) {
        this->objectModel = objectModel;
    }

    QList<QString> getSubDevice() {
        return subDevice;
    }

    void setSubDevice(QList<QString> subDevice) {
        this->subDevice = subDevice;
    }

    QList<QString> getSubLogicalModels() {
        return subLogicalModels;
    }

    void setSubLogicalModels(QList<QString> subLogicalModels) {
        this->subLogicalModels = subLogicalModels;
    }



private:
    //********** 三级子系统级.cbm下读取内容
    //F3System parent;
    //// 四级设备级.cbm的文件名
    //QString f4FileName;

    //********** 四级设备级.cbm下读取内容

    //四级设备级的类型
    QString sysClassifyName;

    //四级设备级.fam的内容
    //QList<TripletPara> f4Paras;
    QString famFileName;

    //设备相对于变电站原点的空间变换矩阵
    QString transformMatrix;

    //引用的物理模型dev
    QString objectModel;

    //引用的部件索引cbm
    QList<QString> subDevice;

    //引用的逻辑模型cbm
    QList<QString> subLogicalModels;

};