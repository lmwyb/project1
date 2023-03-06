#include "example12.h"
#include<qdebug.h>
#include<QFileDialog>
#include <stdio.h>
#include <algorithm>    // find

int flag1 = 1;
int flag2 = 0;
example12::example12(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    //���ر�����
    setWindowFlags(Qt::FramelessWindowHint);
    //ȫ����ʾ
    showFullScreen();

    //// ���ñ���ͼƬ
    //setAutoFillBackground(true);    // ���Ҫ����, ���������ʾ��������ͼ.
    //QPalette palette = this->palette();
    ////palette.setColor(QPalette::Window, Qt::red);  // ���ñ���ɫ
    ////palette.setBrush(this->backgroundRole(), Qt::black);// ���ñ���ɫ
    //palette.setBrush(QPalette::Window,
    //    QBrush(QPixmap("G:/Example C/example12/picture/���վ3.jpg").scaled(    // ���ű���ͼ.
    //        this->size(),
    //        Qt::IgnoreAspectRatio,
    //        Qt::SmoothTransformation)));    // ʹ��ƽ�������ŷ�ʽ
    //this->setPalette(palette);
    ////��������ɫ͸��
    //QPalette pll = ui.tableWidget->palette();
    //pll.setBrush(QPalette::Base, QBrush(QColor(190, 190, 190, 255)));
    //ui.tableWidget->setPalette(pll);

    ui.pushButton1->setIcon(QPixmap("G:/Example C/example12/picture/����4.png"));
    ui.pushButton2->setIcon(QPixmap("G:/Example C/example12/picture/����.png"));
    ui.pushButton3->setIcon(QPixmap("G:/Example C/example12/picture/�ر�.png"));
    ui.pushButton4->setIcon(QPixmap("G:/Example C/example12/picture/���1.png"));
    connect(ui.pushButton1, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(ui.pushButton2, SIGNAL(clicked()), this, SLOT(slotSaveFile()));
    connect(ui.pushButton3, SIGNAL(clicked()), this, SLOT(close()));
  
    ui.pushButton1->setFixedSize(100, 34);
    ui.pushButton2->setFixedSize(100, 34);
    ui.pushButton3->setFixedSize(100, 34);
    ui.pushButton4->setFixedSize(100, 34);
    ui.pushButton1->setStyleSheet("font-size : 15px");
    ui.pushButton2->setStyleSheet("font-size : 15px");
    ui.pushButton3->setStyleSheet("font-size : 15px");
    ui.pushButton4->setStyleSheet("font-size : 15px");
    /*ui.pushButton1->setFont(QFont("song", 20));
    ui.pushButton2->setFont(QFont("song", 20));
    ui.pushButton3->setFont(QFont("song", 20));*/

    //����������ѡ��ť
    m_menu = new QMenu();
    m_mainAction = new QAction(m_menu);
    m_boolAction = new QAction(m_menu);
    m_mainAction->setText("����ͼ");
    m_boolAction->setText("��������ͼ");
    m_mainAction->setCheckable(true);//�Ƿ����check
    m_boolAction->setCheckable(true);//�Ƿ����check
    m_mainAction->setChecked(true);//Ĭ����ͼ��ѡ�е�
    m_mainAction->setFont(QFont("song", 12, 50));
    m_boolAction->setFont(QFont("song", 12, 50));
    m_menu->addAction(m_mainAction);
    m_menu->addAction(m_boolAction);
    connect(m_mainAction, &QAction::triggered, this, &example12::mainFunc);
    connect(m_boolAction, &QAction::triggered, this, &example12::boolFunc);
    ui.pushButton4->setMenu(m_menu);//���ò˵�
  /*  connect(m_menu, SIGNAL(triggered(QAction* m_mainAction)),this, SLOT(mainFunc(QAction* m_mainAction)));
    connect(m_menu, SIGNAL(triggered(QAction * m_boolAction)), this, SLOT(boolFunc(QAction* m_boolAction)));*/

  /*  connect(m_menu, SIGNAL(), this, SLOT(setstatus(QAction*)));
    connect(m_mainAction, SIGNAL(), this, SLOT(mainFunc()));
    connect(m_boolAction, SIGNAL(), this, SLOT(boolFunc()));*/
    //ui.pushButton4->setStyleSheet("QPushButton::menu-indicator{image:none;}");//����ʾ��������ͼ��
   

    label1->setFont(QFont("song", 10, 50));
    label1->setText("�ļ�����");
    label1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label1->setMinimumSize(5, 10);

    label3->setFont(QFont("song", 10, 50));
    label3->setText("�豸����");
    label3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label3->setMinimumSize(5, 10);

    label5->setFont(QFont("song", 10, 50));
    label5->setText("��������������");
    label5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label5->setMinimumSize(5, 10);

    label7->setFont(QFont("song", 10, 50));
    label7->setText("�߼�ģ��������");
    label7->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label7->setMinimumSize(5, 10);

    ui.gridLayout->setSpacing(10);
    ui.gridLayout->addWidget(label1, 0, 0);
    ui.gridLayout->addWidget(label2, 0, 1);
    ui.gridLayout->addWidget(label3, 1, 0);
    ui.gridLayout->addWidget(label4, 1, 1);
    ui.gridLayout->addWidget(label5, 2, 0);
    ui.gridLayout->addWidget(label6, 2, 1);
    ui.gridLayout->addWidget(label7, 3, 0);
    ui.gridLayout->addWidget(label8, 3, 1);

    pTimer = new QTimer(this); //����һ����ʱ��
     //����ʱ���ļ�ʱ�ź���updateGL()��
    connect(pTimer, SIGNAL(timeout()), this, SLOT(call()));
    pTimer->start(10);//��10msΪһ����ʱ����
}
void example12::slotOpenFile()
{
    QString s = QFileDialog::getOpenFileName(this, "���ļ�", "G:/gim�ļ�", "Files(*.gim)");
    qDebug() << s << endl;//�õ�gim�ļ�����·��
    qDebug() << "��ʼ��ȡGIM" << endl;
    GimReader gimreaderopen;
    gimreaderopen.ImportGim(s);
    //gimreaderopen.GetGimModFileList2();
    gimreaderopen.run();
    //gimreaderopen.readXmlPara("G:/idea/gim/src/main/resources/gimUncompress/������/MOD/03aa690f-f38c-4bd9-97cf-c4b97e896294.mod");
    qDebug() << "��ȡGIM����" << endl;
}
void example12::slotSaveFile()
{
    QString s = QFileDialog::getSaveFileName(this, "�����ļ�", "G:/gim�ļ�", "Files(*.gim)");
    GimReader gimreadersave;
    gimreadersave.ExportGim(s);
}
void example12::close()
{
    QWidget::close();
}
void example12::mainFunc()
{
    if (m_mainAction->isChecked()) {
        flag1 = 1;
        qDebug() << "��ʾ��ͼ��" << "" << "flag1:" << flag1;
    }
    else {
        flag1 = 0;
        qDebug() << "flag1:" << flag1;
    }
}
void example12::boolFunc()
{
    if (m_boolAction->isChecked()) {
        flag2 = 1;
        qDebug() << "��ʾ����ͼ��"<<""<<"flag2:"<<flag2;
    }
    else {
        flag2 = 0;
        qDebug() << "flag2:" << flag2;
    }
}

void example12::call()
{
    getInfo(objectModel, sysParas);
}
//�豸����DEV��������ʾ
void example12::getInfo(ObjectModel objectModel, QList<TripletPara> sysParas)
{
    //qDebug() << "��������" << endl;
    if (sysParas.size() != 0) {
        label2->setFont(QFont("song", 10, 50));
        label2->setText(objectModel.getFileName());//dev���
        label2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label2->setMinimumSize(5, 10);

        label4->setFont(QFont("song", 10, 50));
        label4->setText(objectModel.getSymbolName());
        label4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label4->setMinimumSize(5, 10);

        label6->setFont(QFont("song", 10, 50));
        label6->setText(QString::number(objectModel.getSubObjectModels().size()));
        label6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label6->setMinimumSize(5, 10);

        label8->setFont(QFont("song", 10, 50));
        label8->setText(QString::number(objectModel.getSolidModels().size()));
        label8->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label8->setMinimumSize(5, 10);

        ui.tableWidget->setColumnCount(3);//��������
        int i = sysParas.size();
        ui.tableWidget->setRowCount(i);//��������
        ui.tableWidget->horizontalHeader()->setVisible(false);//����ˮƽ��ͷ
        ui.tableWidget->verticalHeader()->setVisible(false);//���ش�ֱ��ͷ
        ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        //ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui.tableWidget->setFont(QFont("song", 10, 50));
        for (int i = 0; i < sysParas.size(); i++) {
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(sysParas[i].getParaNameEN()));
            ui.tableWidget->setItem(i, 1, new QTableWidgetItem(sysParas[i].getParaNameCH()));
            ui.tableWidget->setItem(i, 2, new QTableWidgetItem(sysParas[i].getParameter()));
        }
    }
}