#pragma once
#include<qlist>
#include <QtCore/qstring.h>

#include <iostream>
#include <QtCore/QDebug.h>
#include <iostream>
#include "SchFile.h"
#include"GimFile.h"
class LogicalModel :public GimFile
{
public:

    LogicalModel() {
        this->schFiles = QList<QString>();
    }

    QList<QString> getSchFiles() {
        return schFiles;
    }

    void setSchFiles(QList<QString> schFiles) {
        this->schFiles = schFiles;
    }

private:
    ////********** һ��ȫվ��.cbm/�ļ��豸��.cbm�¶�ȡ����
    //F4System  parent;
    //// �߼�ģ��.cbm���ļ���
    //QString logicModelFileName;

    //********** �߼�ģ��.cbm�¶�ȡ����

    //���õ�sch�ļ�
    QList<QString> schFiles;

};