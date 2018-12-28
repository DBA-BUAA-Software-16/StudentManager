#ifndef CREATECOURSEDIALOG_H
#define CREATECOURSEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
namespace Ui {
class createCourseDialog;
}

class createCourseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit createCourseDialog(QWidget *parent = 0);
    ~createCourseDialog();

private slots:
    void on_courseNameLineEdit_textChanged(const QString &arg1);
    void cometeacherManage(QString);
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::createCourseDialog *ui;
    bool check = false;
    QSqlTableModel *model;
    QSqlRecord tecRecord;
};

#endif // CREATECOURSEDIALOG_H
