#ifndef STUDENTMANAGE_H
#define STUDENTMANAGE_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QModelIndex>
#include "passwordChangeDialog.h"
#include "addCourseDialog.h"
#include <QDebug>
namespace Ui {
class studentManage;
}

class studentManage : public QWidget
{
    Q_OBJECT

public:
    explicit studentManage(QWidget *parent = 0);
    ~studentManage();

private:
    Ui::studentManage *ui;
    void setTableView();
    QStandardItemModel *itemModel;
    passwordChangeDialog *change;
    addCourseDialog *addCourse;
signals:
    void toDialog();
    void toPasswordChange(QString);
    void toaddCourse(QString);
private slots:
    void qtimeSlot();
    void comedialog(QString,QString);
    void on_exitButton_clicked();
    void on_logoutButton_clicked();
    void on_deleteButton_clicked();
    void on_tableView_clicked();
    void on_changePasswordButton_clicked();
    void on_addcourseButton_clicked();
};

#endif // STUDENTMANAGE_H
