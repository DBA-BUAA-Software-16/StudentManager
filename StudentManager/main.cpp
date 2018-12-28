#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDateTime>
#include "dbopr.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("140.143.30.64");
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("dba");

    if(db.open())
    {
        DBOpr::createCourse(1,20,2,QDateTime::currentDateTime(),QDateTime::currentDateTime(),20);
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::warning(NULL,"warning","数据库打开失败",QMessageBox::Yes);
    }
}
