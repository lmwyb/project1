#pragma once
#ifdef WIN32  
#pragma execution_character_set("utf-8")
#endif
#include <QtWidgets/QWidget>
#include "ui_example12.h"
#include "GimReader.h"
#include<qmenu.h>

extern int flag1;
extern int flag2;
class example12 : public QWidget
{
    Q_OBJECT

public:
    example12(QWidget* parent = Q_NULLPTR);

    void getInfo(ObjectModel objectModel, QList<TripletPara> sysParas);

public slots:
    void slotOpenFile();
    void slotSaveFile();
    void call();
    void close();
    void mainFunc();
    void boolFunc();

public:
    QLabel* label1 = new QLabel();
    QLabel* label2 = new QLabel();
    QLabel* label3 = new QLabel();
    QLabel* label4 = new QLabel();
    QLabel* label5 = new QLabel();
    QLabel* label6 = new QLabel();
    QLabel* label7 = new QLabel();
    QLabel* label8 = new QLabel();

    //QPushButton* statusputton;
    QMenu* m_menu;
    QAction* m_mainAction;
    QAction* m_boolAction;
private:
    Ui::example12Class ui;
    QTimer* pTimer;
};