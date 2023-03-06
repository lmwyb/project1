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
    ////********** 一级全站级.cbm/四级设备级.cbm下读取内容
    //F4System  parent;
    //// 逻辑模型.cbm的文件名
    //QString logicModelFileName;

    //********** 逻辑模型.cbm下读取内容

    //引用的sch文件
    QList<QString> schFiles;

};