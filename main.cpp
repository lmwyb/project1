#include "example12.h"
#include <QtWidgets/QApplication>
void initQss()
{
    QFile file("G:/Example C/QSSCollect/QSSCollect/orange-dark.qss");
    if (!file.open(QIODevice::ReadOnly))
        exit(0);

    QTextStream in(&file);
    QString css = in.readAll();
    qApp->setStyleSheet(css);
    //qApp->setFont(QFont("΢���ź�", 9));

    return;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initQss();//step 2
    
    example12 w;
    w.show();
    return a.exec();
}
