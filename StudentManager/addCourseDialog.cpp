#include "addCourseDialog.h"
#include "ui_addCourseDialog.h"

addCourseDialog::addCourseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addCourseDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加课程");
    ui->tecComboBox->setMaxCount(5);//设置下拉最多5项
    ui->courseComboBox->setMaxCount(5);
    ui->okButton->setEnabled(false);
    ui->tecComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);//设置按照字典序排序
    ui->courseComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Course");
    model->select();
    for(int i=0;i<model->rowCount();i++)
    {
        ui->courseComboBox->addItem(model->record(i).value("courseName").toString());
    }
    delete model;
    settecComboBox();
}

addCourseDialog::~addCourseDialog()
{
    delete ui;
}

void addCourseDialog::on_cancelButton_clicked()
{
    this->reject();
}

void addCourseDialog::on_courseComboBox_currentIndexChanged(int index)
{
    settecComboBox();
}

void addCourseDialog::settecComboBox()
{
    ui->tecComboBox->clear();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Course");
    model->select();
    QSqlTableModel *tModel = new QSqlTableModel;
    tModel->setTable("Teacher");
    tModel->select();
    for(int i=0;i<model->rowCount();i++)
    {
        if(model->record(i).value("courseName")==ui->courseComboBox->currentText())
        {
            for (int j = 0;j < tModel->rowCount();j++) {
                if(model->record(i).value("tid") == tModel->record(j).value("id")){
                    ui->tecComboBox->addItem(tModel->record(j).value("name").toString());
                    break;
                }
            }

        }
    }
    setcreditHour();
    delete model;
}

void addCourseDialog::setcreditHour()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Course");
    model->select();
    QSqlTableModel *tModel = new QSqlTableModel;
    tModel->setTable("Teacher");
    tModel->select();
    for(int i=0;i<tModel->rowCount();i++)
    {
        if(tModel->record(i).value("name")==ui->tecComboBox->currentText())
        {
            for (int j = 0;j < model->rowCount();j++) {
                if(model->record(j).value("tid") == tModel->record(i).value("id") && model->record(j).value("courseName") == ui->courseComboBox->currentText()){
                    ui->creditHour->setText("学时："+model->record(j).value("classhour").toString());
                    return;
                }
            }
        }
    }
    delete model;
}

void addCourseDialog::on_tecComboBox_currentIndexChanged(int index)
{
    setcreditHour();
}

void addCourseDialog::on_okButton_clicked()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("Course");
    model->select();
    QSqlTableModel *tModel = new QSqlTableModel;
    tModel->setTable("Teacher");
    tModel->select();
    for(int i=0;i<model->rowCount();i++)
    {
        if(model->record(i).value("courseName")==ui->courseComboBox->currentText())
        {
            for (int j = 0;j < tModel->rowCount();j++) {
                if(tModel->record(j).value("name") == ui->tecComboBox->currentText()){
                    QString tecID = tModel->record(i).value(2).toString();
                    int courseID = model->record(j).value("id").toInt();
                    model->setTable("SC");
                    model->select();
                    QSqlQuery query("select id from Student where username = '"+stuID+"'");
                    query.next();
                    int tmpid = query.record().value(0).toInt();
                    QSqlRecord record1 = model->record();

                    qDebug()<<tmpid;
                    qDebug()<<courseID;
                    record1.setValue(0,tmpid);
                    record1.setValue(1,courseID);
                    record1.setValue(2,-1);
                    if(model->insertRecord(-1,record1))
                    {
                        QMessageBox::information(this,"提醒","添加课程成功",QMessageBox::Button::Yes);
                        delete model;
                        this->accept();
                    }
                    else
                    {
                        QMessageBox::warning(this,"警告","数据上传失败");
                        delete model;
                        this->reject();
                    }
                    break;
                }
            }
        }
    }
}

void addCourseDialog::comestudentManage(QString id)
{
    stuID =id;
    ui->okButton->setEnabled(true);
}
