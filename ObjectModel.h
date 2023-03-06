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
    ////********** �ļ��豸��.cbm/��������.cbm/����ģ��.dev�¶�ȡ����
    //T parent;
    ////����ģ��.dev���ļ���
    //QString objectFileName;

    ////����ģ�͵Ŀռ�任����
    //QString transformMatrix;

    ////********** ����ģ��.dev�¶�ȡ����

    ////����ģ��fam������
    //QList<TripletPara> objectParas;
    QString famFileName;

    //dev��Ӧ�ĵ����豸��ģ������
    QString symbolName;


    //���õĴμ�����ģ��dev
    QList<QString> subObjectModels;

    //�μ�����ģ�͵Ŀռ�任����
    //    private List<String> transformMatrix;
    QList<QString> subObjTransformMatrix;

    //���õ�phm�ļ�
    QList<QString> solidModels;

    //phmģ�͵Ŀռ�任����
    //    private List<String> transformMatrixPHM;
    QList<QString> solidTransformMatrix;
};