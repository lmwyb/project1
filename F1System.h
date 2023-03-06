#pragma once
#include <QtCore/qstring.h>
#include"qlist.h"
#include"GimFile.h"
class F1System :public GimFile
{
public:

    QString getFamFileName() {
        return famFileName;
    }
    void setFamFileName(QString famFileName) {
        this->famFileName = famFileName;
    }
    QList<QString> getSubSystems() {
        return subSystems;
    }
    void setSubSystems(QList<QString> subSystems) {
        this->subSystems = subSystems;
    }
    QList<QString> getSubLogicalModels() {
        return subLogicalModels;
    }
    void setSubLogicalModels(QList<QString> subLogicalModels) {
        this->subLogicalModels = subLogicalModels;
    }
    QList<QString> getIfcFiles() {
        return ifcFiles;
    }
    void setIfcFiles(QList<QString> ifcFiles) {
        this->ifcFiles = ifcFiles;
    }
    QString getMaterialSheet() {
        return materialSheet;
    }
    void setMaterialSheet(QString materialSheet) {
        this->materialSheet = materialSheet;
    }

private:
    //********** project.cbm下读取内容
    //ProjectFile parent;

    ////一级全站级cbm的文件名
    //QString f1FileName;

    //********** 一级全站级.cbm下读取内容

    //一级全站级fam的内容
    //QList<TripletPara> f1Paras;
    QString famFileName;

    //引用的二级系统级
    QList<QString> subSystems;

    //引用的逻辑模型
    QList<QString> subLogicalModels;

    //引用的ifc文件
    QList<QString> ifcFiles;

    //补充材料xml
    QString materialSheet;

};