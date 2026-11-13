#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 本地文件方式，没有冒号
    QFile file("dark_gold.qss");
    if(file.open(QFile::ReadOnly)){
        QString style = QLatin1String(file.readAll());
        a.setStyleSheet(style);
        file.close();
        qDebug() << "QSS加载成功";
    }else{
        qDebug() << "QSS加载失败:" << file.errorString();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
