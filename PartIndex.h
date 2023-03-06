#pragma once
#include <QtCore/qstring.h>
#include <iostream>
#include"GimFile.h"


class PartIndex :public GimFile
{
public:

    QString getPartName() {
        return partName;
    }

    void setPartName(QString partName) {
        this->partName = partName;
    }

    QString getFamFileName() {
        return famFileName;
    }

    void setFamFileName(QString famFileName) {
        this->famFileName = famFileName;
    }

    QString getObjectModel() {
        return objectModel;
    }

    void setObjectModel(QString objectModel) {
        this->objectModel = objectModel;
    }



private:
    ////********** �ļ��豸��.cbm�¶�ȡ����
    //F4System parent;
    //// ��������.cbm���ļ���
    //QString partIndexFileName;

    //********** ��������.cbm�¶�ȡ����

    //��������������
    QString partName;

    //��������.fam������
    //QList<TripletPara> partIndexParas;
    QString famFileName;

    //���õ�����ģ��dev
    //ObjectModel<class T> objectModel;
    QString objectModel;

};