#include "dialog.h"
#include <QApplication>

#include <QSqlDatabase>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>
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
        Dialog w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::warning(NULL,"warning","数据库打开失败",QMessageBox::Yes);
    }
    return a.exec();
}
