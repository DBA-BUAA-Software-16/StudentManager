#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>

namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = 0);
    ~registerDialog();

private slots:
    void on_registerButton_clicked();

    void on_passwordLineEdit_textChanged(const QString &arg1);

    void on_passwordLineEdit_2_textChanged(const QString &arg1);

    void on_stuIDLineEdit_textChanged(const QString &arg1);

    void on_stuNameLineEdit_textChanged(const QString &arg1);

private:
    Ui::registerDialog *ui;
    QButtonGroup *radioGroup;
    QSqlTableModel *model;
    void clearAll();
    bool isEmpty();
};

#endif // REGISTERDIALOG_H
