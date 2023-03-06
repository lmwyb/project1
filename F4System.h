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
    //********** ������ϵͳ��.cbm�¶�ȡ����
    //F3System parent;
    //// �ļ��豸��.cbm���ļ���
    //QString f4FileName;

    //********** �ļ��豸��.cbm�¶�ȡ����

    //�ļ��豸��������
    QString sysClassifyName;

    //�ļ��豸��.fam������
    //QList<TripletPara> f4Paras;
    QString famFileName;

    //�豸����ڱ��վԭ��Ŀռ�任����
    QString transformMatrix;

    //���õ�����ģ��dev
    QString objectModel;

    //���õĲ�������cbm
    QList<QString> subDevice;

    //���õ��߼�ģ��cbm
    QList<QString> subLogicalModels;

};