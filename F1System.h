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
    //********** project.cbm�¶�ȡ����
    //ProjectFile parent;

    ////һ��ȫվ��cbm���ļ���
    //QString f1FileName;

    //********** һ��ȫվ��.cbm�¶�ȡ����

    //һ��ȫվ��fam������
    //QList<TripletPara> f1Paras;
    QString famFileName;

    //���õĶ���ϵͳ��
    QList<QString> subSystems;

    //���õ��߼�ģ��
    QList<QString> subLogicalModels;

    //���õ�ifc�ļ�
    QList<QString> ifcFiles;

    //�������xml
    QString materialSheet;

};