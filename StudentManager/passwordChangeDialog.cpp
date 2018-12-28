#include "passwordChangeDialog.h"
#include "ui_passwordchangedialog.h"

passwordChangeDialog::passwordChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwordChangeDialog)
{
    ui->setupUi(this);
    setWindowTitle("更改密码");
    ui->okButton->setEnabled(false);
    ui->prePasswordEdit->setPlaceholderText("请输入原密码");
    ui->passwordEdit->setPlaceholderText("请输入密码");
    ui->passwordEdit_2->setPlaceholderText("请确认密码");
    ui->prePasswordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit_2->setEchoMode(QLineEdit::Password);
}

passwordChangeDialog::~passwordChangeDialog()
{
    delete ui;
}

void passwordChangeDialog::on_passwordEdit_textChanged(const QString &arg1)
{
    if(check)
    {
        ui->okButton->setEnabled(true);
    }
}

void passwordChangeDialog::on_passwordEdit_2_textChanged(const QString &arg1)
{
    if(check)
    {
        ui->okButton->setEnabled(true);
    }
}

void passwordChangeDialog::on_cancelButton_clicked()
{
    this->close();
}

void passwordChangeDialog::comeStudentManage(QString stuID)
{
    model = new QSqlTableModel(this);
    model->setTable("Student");
    model->select();
    for(int i=0;i<model->rowCount();i++)
    {
        if(model->record(i).value(1)==stuID)
        {
            n=i;
            check = true;
            t=0;
            break;
        }
    }
}

void passwordChangeDialog::comeTeacherManage(QString tecID)
{
    model = new QSqlTableModel(this);
    model->setTable("Teacher");
    model->select();
    for(int i=0;i<model->rowCount();i++)
    {
        if(model->record(i).value(1)==tecID)
        {
            n=i;
            check = true;
            t=1;
            break;
        }
    }
}

bool passwordChangeDialog::judge()
{
    if(ui->prePasswordEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入原密码");
        this->clearAll();
        return false;
    }
    else if(ui->passwordEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入新密码");
        this->clearAll();
        return false;
    }
    else if (ui->passwordEdit_2->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请确认密码");
        this->clearAll();
        return false;
    }
    if(t==0)//学生更改密码
    {
        model->setTable("Student");
    }
    else if(t==1)
    {
        model->setTable("Teacher");
    }
    model->select();
    if(model->record(n).value(2)!=ui->prePasswordEdit->text())
    {
        QMessageBox::warning(this,"警告","原密码错误");
        this->clearAll();
        return false;
    }
    else if(ui->passwordEdit->text()!=ui->passwordEdit_2->text())
    {
        QMessageBox::warning(this,"警告","两次密码不匹配");
        this->clearAll();
        return false;
    }
    else
    {
        QSqlRecord record = model->record(n);
        record.setValue(2,ui->passwordEdit->text());
        model->setRecord(n,record);
        if(model->submitAll())
        {
            return true;
        }
        else
        {
            QMessageBox::warning(this,"警告","数据上传失败，请稍后再试");
            this->clearAll();
            return false;
        }
    }
}

void passwordChangeDialog::clearAll()
{
    ui->prePasswordEdit->clear();
    ui->passwordEdit->clear();
    ui->passwordEdit_2->clear();
}

void passwordChangeDialog::on_prePasswordEdit_textChanged(const QString &arg1)
{
    if(check)
    {
        ui->okButton->setEnabled(true);
    }
}

void passwordChangeDialog::on_okButton_clicked()
{
    if(judge())
    {
        QMessageBox::information(this,"提示","修改密码成功",QMessageBox::Button::Yes);
        this->close();
    }
}
