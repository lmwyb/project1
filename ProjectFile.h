#pragma once
#include"GimFile.h"
class ProjectFile :public GimFile
{
public:

    QString getBLHA() {
        return BLHA;
    }

    void setBLHA(QString BLHA) {
        this->BLHA = BLHA;
    }

    QString getSubSystem() {
        return subSystem;
    }

    void setSubSystem(QString subSystem) {
        this->subSystem = subSystem;
    }

private:
    //γ�ȣ����ȣ��ߣ�������ƫ��
    QString BLHA;

    //���õ�һ��ȫվ��cbm
    QString subSystem;

};


