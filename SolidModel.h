#pragma once
#include <iostream>
#include"GimFile.h"
#include"qlist.h"
class SolidModel : public GimFile
{
public:

    QList<QString> getcolorList() {
        return colorList;
    }

    void setcolorList(QList<QString> colorList) {
        this->colorList = colorList;
    }

    QList<QString> gettransformMatrixList() {
        return transformMatrixList;
    }

    void settransformMatrixList(QList<QString> transformMatrixList) {
        this->transformMatrixList = transformMatrixList;
    }
    QList<QString> getchildren() {
        return children;
    }

    void setchildren(QList<QString> children) {
        this->children = children;
    }
private:
    //********** ����ģ��.dev��/���ģ���ļ�.phm��ȡ����
   // ObjectModel parent;
    //���ģ���ļ�phm���ļ���
    //QString phmFileName;

    //phmģ�͵Ŀռ�任����
   // QString transformMatrix;

    //��ɫ
    //QString color;

    //********** ���ģ���ļ�.phm�¶�ȡ����

    //phm
   // QList<SolidModel> solidModelList;

    //mod
   // QList<ModFile> modFileList;

    //stl
   // QList<StlFile> stlFileList;
    QList<QString> colorList;
    QList<QString> transformMatrixList;
    QList<QString> children;

};