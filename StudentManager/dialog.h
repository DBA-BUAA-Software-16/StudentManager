#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "registerdialog.h"
#include "studentManage.h"
#include "teacherManage.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
signals:
    void toStudentManage(QString,QString);
    void toTeacherManage(QString,QString);
private slots:
     void on_registerButton_clicked();

     void on_userNameLineEdit_textChanged();

     void on_passwordLineEdit_textChanged();

     void on_loginButton_clicked();
private:
    Ui::Dialog *ui;
    QButtonGroup *radiogroup;
    QSqlTableModel *model;
    studentManage *student;
    teacherManage *teacher;
    bool isEmpty();
    void clearAll();
    int checkLogin();
};

#endif // DIALOG_H
