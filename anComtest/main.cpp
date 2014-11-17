#include <QtGui/QApplication>
#include <QTextCodec>   //����ͷ�ļ�
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //ʹ����ɴ�������
    MainWindow w;
    w.show();
    QObject::connect(&w,SIGNAL(myquit()),&a,SLOT(quit()));
    a.quit();
    return a.exec();
}
