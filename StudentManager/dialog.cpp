#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("教务管理系统");
    this->setMinimumSize(590,342);
    this->setMaximumSize(590,342);
    ui->loginButton->setEnabled(false);//设置登陆按钮不可用
    ui->userNameLineEdit->setPlaceholderText("请输入用户名");//placeholderText可实现为灰色字，并没有实际输入
    ui->passwordLineEdit->setPlaceholderText("请输入密码");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);//设置为密码不可见
    radiogroup = new QButtonGroup(this);
    radiogroup->addButton(ui->stuRadioButton,0);
    radiogroup->addButton(ui->tecRadioButton,1);
    QObject::connect(ui->quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_registerButton_clicked()
{
    registerDialog r(this);
    this->hide();
    if(r.exec()==QDialog::Accepted)
    {
        QMessageBox::information(this,"注册信息","注册成功",QMessageBox::Button::Yes);
    }
    this->showNormal();
}

void Dialog::on_userNameLineEdit_textChanged()
{
    ui->loginButton->setEnabled(true);
}

void Dialog::on_passwordLineEdit_textChanged()
{
    ui->loginButton->setEnabled(true);
}

void Dialog::on_loginButton_clicked()
{
    if(!isEmpty())
    {
        model = new QSqlTableModel(this);

        if(radiogroup->checkedId()==0)//学生类型
        {
            model->setTable("Student");
            model->select();
            int i=checkLogin();
            if(i!=-1)
            {
                QSqlRecord record = model->record(i);
                student = new studentManage;
                //建立Student和Dialog的联系
                connect(student,SIGNAL(toDialog()),this,SLOT(showNormal()));
                student->show();
                connect(this,SIGNAL(toStudentManage(QString,QString)),student,SLOT(comedialog(QString,QString)));
                emit toStudentManage(record.value(1).toString(),record.value(3).toString());
                this->clearAll();
                this->hide();
                return;
            }
        }
        else if(radiogroup->checkedId()==1)//教师类型
        {
            model->setTable("Teacher");
            model->select();
            int i = checkLogin();
            if(i!=-1)
            {
                QSqlRecord record = model->record(i);
                teacher = new teacherManage;
                //建立teacher和dialog的联系
                connect(teacher,SIGNAL(toDialog()),this,SLOT(showNormal()));
                teacher->show();
                connect(this,SIGNAL(toTeacherManage(QString,QString)),teacher,SLOT(comeDialog(QString,QString)));
                emit toTeacherManage(record.value(1).toString(),record.value(3).toString());
                this->clearAll();
                this->hide();
                return;
            }
        }
        else
        {
            QMessageBox::information(this,"警告","请选择用户类型");
        }
    }
    else
    {
        this->clearAll();
    }
}

bool Dialog::isEmpty()
{
    if(ui->userNameLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入学号");
        return true;
    }
    else if(ui->passwordLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入密码");
        return true;
    }
    return false;
}

void Dialog::clearAll()
{
    ui->userNameLineEdit->clear();
    ui->passwordLineEdit->clear();
}

int Dialog::checkLogin()
{
    int i;
    for(i=0;i<model->rowCount();i++)
    {
        QSqlRecord record = model->record(i);
        if(record.value(1)==ui->userNameLineEdit->text())
        {
            if(record.value(2)==ui->passwordLineEdit->text())
            {
                QMessageBox::information(this,"提示","登陆成功",QMessageBox::Button::Yes);
                return i;
            }
            else
            {
                QMessageBox::warning(this,"警告","密码错误");
                ui->passwordLineEdit->clear();
                return -1;
            }
        }
    }
    if(i==model->rowCount())
    {
        QMessageBox::warning(this,"警告","学号不存在或未注册");
        this->clearAll();
        return -1;
    }
    return -1;
}
