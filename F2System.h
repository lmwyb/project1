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
    //********** һ��ȫվ��.cbm�¶�ȡ����
    //F1System parent;

    //// ����ϵͳ��.cbm���ļ���
    //QString f2FileName;

    //********** ����ϵͳ��.cbm�¶�ȡ����

    //����ϵͳ��������
    QString sysClassifyName;

    //����ϵͳ��.fam������
    QString famFileName;

    //���õ�������ϵͳ��
    QList<QString> subSystems;

    //���õ�ifc�ļ�
    QList<QString> ifcFiles;

    //�������xml
    QString materialSheet;



};