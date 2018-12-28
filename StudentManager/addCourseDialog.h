#ifndef ADDCOURSEDIALOG_H
#define ADDCOURSEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
namespace Ui {
class addCourseDialog;
}

class addCourseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addCourseDialog(QWidget *parent = 0);
    ~addCourseDialog();

private slots:
    void on_cancelButton_clicked();

    void on_courseComboBox_currentIndexChanged(int index);

    void on_tecComboBox_currentIndexChanged(int index);

    void on_okButton_clicked();
    void comestudentManage(QString);
signals:
    void toStudentManage();
private:
    Ui::addCourseDialog *ui;
    QString stuID;
    void settecComboBox();
    void setcreditHour();
};

#endif // ADDCOURSEDIALOG_H
