#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDebug>
namespace Ui {
class passwordChangeDialog;
}

class passwordChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit passwordChangeDialog(QWidget *parent = 0);
    ~passwordChangeDialog();
signals:
    void toStudentManage();
    void toTeacherManage();
private slots:
    void on_passwordEdit_textChanged(const QString &arg1);
    void on_passwordEdit_2_textChanged(const QString &arg1);
    void on_cancelButton_clicked();
    void on_prePasswordEdit_textChanged(const QString &arg1);
    void on_okButton_clicked();
    void comeStudentManage(QString);
    void comeTeacherManage(QString);
private:
    Ui::passwordChangeDialog *ui;
    QSqlTableModel *model;
    int n;
    bool check = false;
    bool judge();
    void clearAll();
    int t;
};

#endif // PASSWORDCHANGEDIALOG_H
