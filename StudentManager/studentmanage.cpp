#include "studentManage.h"
#include "ui_studentmanage.h"

studentManage::studentManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentManage)
{
    ui->setupUi(this);
    setWindowTitle("个人信息管理");
    QTimer *timer=new QTimer(this);//计时器
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));//调用文本设置函数
    timer->start(1000);//1000ms更新一次
    //建立一个表，用于在tableview中显示
    itemModel = new QStandardItemModel;
    itemModel->setColumnCount(4);
    itemModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("课程名"));//设置表头名
    itemModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("学时"));
    itemModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("授课教师"));
    itemModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("成绩"));
    ui->tableView->setModel(itemModel);//在tableView中应用itemModel
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);//用于设置列宽不可变
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0,152);//设置列宽
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,145);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表为不可编辑
    ui->deleteButton->setEnabled(false);//列被选中前删除按钮不可用
    QObject::connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(on_tableView_clicked()));

}

studentManage::~studentManage()
{
    delete ui;
}

void studentManage::qtimeSlot()
{
    QDateTime time = QDateTime::currentDateTime();
    ui->timeLabel->setText("系统时间："+time.toString("yyyy-MM-dd hh:mm:ss ddd"));
}

void studentManage::comedialog(QString num, QString name)
{
    ui->stuIDLabel->setText(num);
    ui->stuNameLabel->setText(name);
    setTableView();
}

void studentManage::on_exitButton_clicked()
{
    if(QMessageBox::question(this,"退出","是否退出系统",QMessageBox::Button::Yes|QMessageBox::Button::No)==QMessageBox::Button::Yes)
    {
        this->close();
        delete this;
    }
}

void studentManage::on_logoutButton_clicked()
{
    if(QMessageBox::question(this,"注销","是否注销当前账号",QMessageBox::Button::Yes|QMessageBox::Button::No)==QMessageBox::Button::Yes)
    {
        emit toDialog();
        delete this;
    }
}

void studentManage::setTableView()
{
    itemModel->removeRows(0,itemModel->rowCount());
    QSqlTableModel *sqlModel = new QSqlTableModel(this);
    sqlModel->setTable("SC");
    sqlModel->select();
    QSqlQuery query;
    query.prepare("select id from Student where username = '"+ui->stuIDLabel->text() +"'");
    query.exec();
    query.next();
    int id = query.record().value(0).toInt();
    int i,count=0;;
    for(i=0;i<sqlModel->rowCount();i++)
    {
        QSqlRecord record = sqlModel->record(i);
        if(record.value(0)==id)
        {
            count++;
            query.prepare("select * from Course where id = " + record.value("cid").toString());
            query.exec();
            query.next();
            qDebug()<<query.value("courseName").toString();
            itemModel->setItem(itemModel->rowCount(),0,new QStandardItem(query.value("courseName").toString()));//设置课程名
            itemModel->setItem(itemModel->rowCount()-1,1,new QStandardItem(query.value("classhour").toString()));//设置课时
            if(record.value(2).toInt()!=-1)//成绩不为空
            {
                itemModel->setItem(itemModel->rowCount()-1,3,new QStandardItem(record.value(2).toString()));//设置成绩
                qDebug() <<itemModel->rowCount();
            }
            else
            {
                itemModel->setItem(itemModel->rowCount()-1,3,new QStandardItem("成绩还未发布"));
            }
            query.prepare("SELECT name from Course  INNER JOIN Teacher WHERE Course.tid = Teacher.id and Course.id = "+ record.value("cid").toString());
            query.exec();
            query.next();
            itemModel->setItem(itemModel->rowCount()-1,2,new QStandardItem(query.value(0).toString()));//设置教师姓名
        }
    }
    delete sqlModel;
}

void studentManage::on_deleteButton_clicked()
{
    int n = ui->tableView->currentIndex().row();
    if(QMessageBox::question(this,"提醒","是否删除"+itemModel->item(n,0)->text(),QMessageBox::Button::Yes|QMessageBox::Button::No)==QMessageBox::Button::Yes)
    {
        QSqlTableModel *sqlModel = new QSqlTableModel(this);
        sqlModel->setTable("studentcourse");
        sqlModel->select();
        QSqlRecord record;
        for(int i=0;i<sqlModel->rowCount();i++)
        {

            if(sqlModel->record(i).value(0)==ui->stuIDLabel->text()&&sqlModel->record(i).value(1)==itemModel->item(n,0)->text())
            {
                sqlModel->setTable("student");
                sqlModel->select();
                for(int j=0;j<sqlModel->rowCount();j++)
                {
                    if(sqlModel->record(j).value(0)==ui->stuIDLabel->text())
                    {
                        int t=sqlModel->record(j).value(2).toInt();
                        QSqlRecord record = sqlModel->record(j);//获取当前记录
                        record.setValue("courseNum",t-1);//更改课程数目
                        qDebug() <<t;
                        sqlModel->setRecord(j,record);//更改当前记录
                        break;
                    }
                }
                sqlModel->setTable("studentcourse");
                sqlModel->select();//要先选中整个表，然后才能指定删除某一行
                sqlModel->removeRow(i);
                itemModel->removeRow(n);
                break;
            }
        }
        QMessageBox::information(this,"提醒","成功退课",QMessageBox::Button::Yes);
    }
}

void studentManage::on_tableView_clicked()
{
    ui->deleteButton->setEnabled(true);
}

void studentManage::on_changePasswordButton_clicked()
{
    change = new passwordChangeDialog;
    connect(change,SIGNAL(toStudentManage()),this,SLOT(showNormal()));
    connect(this,SIGNAL(toPasswordChange(QString)),change,SLOT(comeStudentManage(QString)));
    emit toPasswordChange(ui->stuIDLabel->text());
    //模式对话框，在子窗口未被关闭的情况下是无法使用父窗口
    change->exec();
    change->setModal(true);
    return;
}

void studentManage::on_addcourseButton_clicked()
{
    addCourse =new addCourseDialog;
    connect(addCourse,SIGNAL(toStudentManage()),this,SLOT(showNormal()));
    connect(this,SIGNAL(toaddCourse(QString)),addCourse,SLOT(comestudentManage(QString)));
    emit toaddCourse(ui->stuIDLabel->text());
    addCourse->exec();
    addCourse->setModal(true);
    setTableView();
    delete addCourse;
    return;
}
