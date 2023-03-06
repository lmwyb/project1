#pragma once
#include"GimFile.h"
#include"qlist.h"
class F2System :public GimFile
{
public:
    F2System() {
        QList<QString> subSystems;
        QList<QString> ifcFiles;
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

    QList<QString> getSubSystems() {
        return subSystems;
    }

    void setSubSystems(QList<QString> subSystems) {
        this->subSystems = subSystems;
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
    //********** 一级全站级.cbm下读取内容
    //F1System parent;

    //// 二级系统级.cbm的文件名
    //QString f2FileName;

    //********** 二级系统级.cbm下读取内容

    //二级系统级的类型
    QString sysClassifyName;

    //二级系统级.fam的内容
    QString famFileName;

    //引用的三级子系统级
    QList<QString> subSystems;

    //引用的ifc文件
    QList<QString> ifcFiles;

    //补充材料xml
    QString materialSheet;



};