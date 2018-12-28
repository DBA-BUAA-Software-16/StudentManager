#include "registerdialog.h"
#include "ui_registerdialog.h"

registerDialog::registerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    setWindowTitle("注册");
    setMaximumSize(436,428);
    setMinimumSize(436,428);
    ui->registerButton->setEnabled(false);//注册按钮初始为不可用
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordLineEdit_2->setEchoMode(QLineEdit::Password);//设置两行密码为不可见
    ui->passwordLineEdit->setPlaceholderText("请输入密码");
    ui->passwordLineEdit_2->setPlaceholderText("请确认密码");
    ui->stuIDLineEdit->setPlaceholderText("请输入学号（数字）");
    ui->stuNameLineEdit->setPlaceholderText("请输入姓名");
    radioGroup = new QButtonGroup(this);
    radioGroup->addButton(ui->stuRadioButton,0);
    radioGroup->addButton(ui->tecRadioButton,1);
    QObject::connect(ui->passwordLineEdit,SIGNAL(textChanged(QString)),this,SLOT(on_passwordLineEdit_textChanged(QString)));
    QObject::connect(ui->passwordLineEdit_2,SIGNAL(textChanged(QString)),this,SLOT(on_passwordLineEdit_2_textChanged(QString)));
    QObject::connect(ui->stuIDLineEdit,SIGNAL(textChanged(QString)),this,SLOT(on_stuIDLineEdit_textChanged(QString)));
    QObject::connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

registerDialog::~registerDialog()
{
    delete ui;
}

void registerDialog::on_registerButton_clicked()
{
    bool check = true;
    if(this->isEmpty())
    {
        this->clearAll();
        return;
    }
    else
    {
        if(ui->passwordLineEdit->text()!=ui->passwordLineEdit_2->text())
        {
            QMessageBox::warning(this,"注册错误","两次密码不匹配");
            this->clearAll();
            return;
        }
        else if(radioGroup->checkedId()==0)//学生注册
        {
            model = new QSqlTableModel(this);
            model->setTable("Student");
            model->select();
            for(int i=0;i<model->rowCount();i++)
            {
                QSqlRecord record = model->record(i);
                if(record.value(1)==ui->stuIDLineEdit->text())
                {
                    QMessageBox::warning(this,"学号重复","此学号已注册");
                    check=false;
                    this->clearAll();
                    return;
                }
            }
            if(check)
            {
                QSqlRecord record1 = model->record();
                record1.setValue("username",ui->stuIDLineEdit->text());
                record1.setValue("password",ui->passwordLineEdit->text());
                record1.setValue("name",ui->stuNameLineEdit->text());
                model->insertRecord(-1,record1);

            }
            if(model->submitAll())
            {
                this->accept();
            }
            else if(!model->submitAll())
            {
                QMessageBox::information(this,"提醒","数据上传失败，请稍后重试",QMessageBox::Yes);
            }
        }
        else if (radioGroup->checkedId()==1) //老师注册
        {
            model = new QSqlTableModel(this);
            model->setTable("Teacher");
            model->select();
            for(int i=0;i<model->rowCount();i++)
            {
                QSqlRecord record = model->record(i);
                if(record.value(1)==ui->stuIDLineEdit->text())
                {
                    QMessageBox::warning(this,"学号重复","此学号已注册");
                    check=false;
                    this->clearAll();
                    return;
                }
            }
            if(check)
            {
                QSqlRecord record1 = model->record();
                record1.setValue("username",ui->stuIDLineEdit->text());
                record1.setValue("password",ui->passwordLineEdit->text());
                record1.setValue("name",ui->stuNameLineEdit->text());
                model->insertRecord(-1,record1);
            }
            if(model->submitAll())
            {
                this->accept();
            }
            else
            {
                QMessageBox::information(this,"提醒","数据上传失败，请稍后再试",QMessageBox::Button::Yes);
            }
        }
        else
        {
            QMessageBox::warning(this,"警告","请选择用户类型");
        }
    }
}


void registerDialog::on_passwordLineEdit_textChanged(const QString &arg1)
{
    ui->registerButton->setEnabled(true);
}

void registerDialog::on_passwordLineEdit_2_textChanged(const QString &arg1)
{
    ui->registerButton->setEnabled(true);
}

void registerDialog::on_stuIDLineEdit_textChanged(const QString &arg1)
{
    ui->registerButton->setEnabled(true);
}

void registerDialog::on_stuNameLineEdit_textChanged(const QString &arg1)
{
    ui->registerButton->setEnabled(true);
}

bool registerDialog::isEmpty()
{
    if(ui->passwordLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"注册信息错误","密码不能为空");
        return true;
    }
    else if(ui->passwordLineEdit_2->text().isEmpty())
    {
        QMessageBox::warning(this,"注册信息错误","请确认密码");
        return true;
    }
    else if(ui->stuIDLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"注册信息错误","学号不能为空");
        return true;
    }
    else if(ui->stuNameLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"注册信息错误","姓名不能为空");
        return true;
    }
    return false;
}

void registerDialog::clearAll()
{
    ui->passwordLineEdit->clear();
    ui->passwordLineEdit_2->clear();
    ui->stuIDLineEdit->clear();
    ui->stuNameLineEdit->clear();
}
