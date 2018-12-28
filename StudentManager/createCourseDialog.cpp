#include "createCourseDialog.h"
#include "ui_createcoursedialog.h"

createCourseDialog::createCourseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createCourseDialog)
{
    ui->setupUi(this);
    setWindowTitle("创建课程");
    ui->courseNameLineEdit->setPlaceholderText("请输入课程名");
    ui->okButton->setEnabled(false);
}

createCourseDialog::~createCourseDialog()
{
    delete ui;
}

void createCourseDialog::on_courseNameLineEdit_textChanged(const QString &arg1)
{
    if(check)
    {
        ui->okButton->setEnabled(true);
    }
}

void createCourseDialog::cometeacherManage(QString tecID)
{
    model = new QSqlTableModel(this);
    model->setTable("Teacher");
    model->select();
    for(int i=0;i<model->rowCount();i++)
    {
        if(model->record(i).value(1)==tecID)
        {
            tecRecord = model->record(i);
            break;
        }
    }
    check = true;

}

void createCourseDialog::on_okButton_clicked()
{
    model->setTable("Course");
    model->select();
    QSqlRecord record = model->record();
    record.setValue("courseName",ui->courseNameLineEdit->text());
    record.setValue("classhour",ui->classHourBox->text().toInt());
    record.setValue("credit",ui->creditHourBox->text().toInt());
    record.setValue("selectstart",ui->beginTime->dateTime());
    record.setValue("selectend",ui->endTime->dateTime());
    record.setValue("tid",tecRecord.value(0));
    model->insertRecord(-1,record);
    if(model->submitAll())
    {
        QMessageBox::information(this,"提醒","创建课程成功",QMessageBox::Button::Yes);
    }
    else
    {
        QMessageBox::warning(this,"警告","数据上传失败，请稍后再试");
    }
    this->close();
}

void createCourseDialog::on_cancelButton_clicked()
{
    this->close();
}
