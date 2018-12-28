#include "teacherManage.h"
#include "ui_teacherManage.h"

teacherManage::teacherManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teacherManage)
{
    ui->setupUi(this);
    setWindowTitle("教师课程管理");
    QTimer *timer=new QTimer(this);//计时器
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));//调用文本设置函数
    timer->start(1000);//1000ms更新一次
    //一开始默认为显示所有的课程
    ui->coursestudentButton->setEnabled(false);
    ui->coursestudentButton->setText("查看课程");
    //创建所有课程的表模板
    courseModel = new QStandardItemModel;
    courseModel->setColumnCount(3);
    courseModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("课程名"));
    courseModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("课时"));
    courseModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("学生人数"));
    ui->allcourseTableView->setModel(courseModel);
    ui->allcourseTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表为不可编辑
    infoModel = new QStandardItemModel;
    infoModel->setColumnCount(3);
    infoModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("学生姓名"));
    infoModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("学号"));
    infoModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("成绩"));
    ui->studentTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->studentTableView->setModel(infoModel);
    //allcourseTableView是1，studentTableView是0
    ui->stackedWidget->setCurrentIndex(1);
}

teacherManage::~teacherManage()
{
    delete ui;
}

void teacherManage::comeDialog(QString id, QString name)
{
    ui->tecID->setText(id);
    ui->tecName->setText(name);
    setallcourseTableView();
}

void teacherManage::qtimeSlot()
{
    QDateTime time = QDateTime::currentDateTime();
    ui->timeLabel->setText("系统时间："+time.toString("yyyy-MM-dd hh:mm:ss ddd"));
}

void teacherManage::on_exitButton_clicked()
{
    if(QMessageBox::question(this,"注销","是否退出系统",QMessageBox::Button::Yes|QMessageBox::Button::No)==QMessageBox::Button::Yes)
    {
        this->close();
        if(win!=NULL)
        {
            qDebug() <<"退出时调用了win的删除";
            win->close();
            delete win;
            win = NULL;
        }
        delete this;
    }
}

void teacherManage::on_logoutButton_clicked()
{
    if(QMessageBox::question(this,"注销","是否注销当前账号",QMessageBox::Button::Yes|QMessageBox::Button::No)==QMessageBox::Button::Yes)
    {
        emit toDialog();
        if(win!=NULL)
        {
            win->close();
            delete win;
            win = NULL;
        }
        delete this;
    }
}

void teacherManage::on_passwordChangeButton_clicked()
{
    change = new passwordChangeDialog;
    connect(change,SIGNAL(toTeacherManage()),this,SLOT(showNormal()));
    connect(this,SIGNAL(toPasswordChange(QString)),change,SLOT(comeTeacherManage(QString)));
    emit toPasswordChange(ui->tecID->text());
    //模式对话框，在子窗口未被关闭的情况下是无法使用父窗口
    change->exec();
    change->setModal(true);
    return;
}

void teacherManage::on_createCourseButto_clicked()
{
    createCourse = new createCourseDialog;
    connect(createCourse,SIGNAL(toTeacherManage()),this,SLOT(showNormal()));
    connect(this,SIGNAL(tocreateCourseDialog(QString)),createCourse,SLOT(cometeacherManage(QString)));
    emit tocreateCourseDialog(ui->tecID->text());
    //启动模式对话框
    createCourse->exec();
    createCourse->setModal(true);
    setallcourseTableView();//更新课程数据
    return;
}

void teacherManage::on_coursestudentButton_clicked()
{
    if(ui->coursestudentButton->text()=="查看课程")
    {
        int n = ui->allcourseTableView->currentIndex().row();//获得当前选中的行数
        currentCourse = courseModel->item(n,0)->text();
        setstudentTableView(n);
        ui->stackedWidget->setCurrentIndex(0);
        ui->coursestudentButton->setText("所有课程");
    }
    else
    {
        setallcourseTableView();//更新课程数据
        ui->stackedWidget->setCurrentIndex(1);
        ui->coursestudentButton->setText("查看课程");
        ui->coursestudentButton->setEnabled(false);
        if(win!=NULL)
        {
//            qDebug() <<"进入了函数";
//            qDebug() <<win;
            delete win;
            win = NULL;//避免成为野指针
        }
    }
}


void teacherManage::on_allcourseTableView_clicked(const QModelIndex &index)
{
    ui->coursestudentButton->setEnabled(true);
}

void teacherManage::setallcourseTableView()
{
    courseModel->removeRows(0,courseModel->rowCount());
    QSqlTableModel *model = new QSqlTableModel;
    QSqlQuery query;
    query.prepare("select id from Teacher where username = '" + ui->tecID->text()+"'");
    query.exec();
    query.next();
    int tid = query.value(0).toInt();
    query.prepare("select * from Course where tid ="+QString::number(tid,10));
    query.exec();
    while (query.next()) {
        courseModel->setItem(courseModel->rowCount(),0,new QStandardItem(query.value("courseName").toString()));//设置课程名
        courseModel->setItem(courseModel->rowCount()-1,1,new QStandardItem(query.value("classHour").toString()));//设置课时
        QSqlQuery tmp("select count(*) from SC where cid ="+ query.value("id").toString());
        tmp.next();
        courseModel->setItem(courseModel->rowCount()-1,2,new QStandardItem(QString::number(tmp.value(0).toInt(),10)));//设置学生人数
    }
}

void teacherManage::setstudentTableView(int n)
{
    all=0;excellent=0;great=0;fine=0;pass=0;notpass=0;marks=0;max=-1;min=110;
    infoModel->removeRows(0,infoModel->rowCount());//清除以前的表
    QSqlQuery query;
    query.prepare("select id from Teacher where username = '"+ui->tecID->text()+"'");
    query.exec();
    query.next();
    int tid = query.value(0).toInt();
    qDebug()<<tid;
    query.prepare("select * from SC inner join Course inner join Student where Course.id = SC.cid and Student.id = SC.sid and Course.tid = "+ QString::number(tid,10));
    query.exec();
    while (query.next()) {
        infoModel->setItem(infoModel->rowCount(),1,new QStandardItem(query.value("sid").toString()));//设置学号
        if(query.value("grade").toInt() != -1){
            infoModel->setItem(infoModel->rowCount()-1,2,new QStandardItem(query.value("grade").toString()));
            all++;
            int g = query.value("grade").toInt();
            marks+=g;
            if(g<min)
            {
                min=g;
            }
            if(g>max)
            {
                max=g;
            }
            if(g>=90)
            {
                excellent++;
            }
            else if(g>=80)
            {
                great++;
            }
            else if(g>=70)
            {
                fine++;
            }
            else if(g>=60)
            {
                pass++;
            }
            else
            {
                notpass++;
            }
        }
        else {
            infoModel->setItem(infoModel->rowCount()-1,2,new QStandardItem("还未设置成绩"));
        }
        infoModel->setItem(infoModel->rowCount()-1,0,new QStandardItem(query.value("name").toString()));
    }
    qDebug() <<all<<" "<<infoModel->rowCount();
    if(all==infoModel->rowCount()&&all!=0)
    {
        qDebug() <<"进入了setcharts";
        setcharts();
    }
}

void teacherManage::setcharts()
{
    if(win!=NULL)//以前有还没有关闭的win
    {
        win->close();
        delete win;
        win = NULL;
    }
    win = new QWidget;
    //饼状图设置
    QPieSeries *pieseries = new QPieSeries();
    pieseries->setHoleSize(0.35);//设置中间的洞的大小
    QPieSlice *slice = pieseries->append("优秀"+QString::number((double)excellent/(double)all*100)+"%", (double)excellent/(double)all);
    slice->setExploded();
    slice->setLabelVisible();
    //pieseries->append("优秀"+QString::number((double)excellent/(double)all*100)+"%", (double)excellent/(double)all);
    pieseries->append("良好"+QString::number((double)great/(double)all*100)+"%", (double)great/(double)all);
    pieseries->append("一般"+QString::number((double)fine/(double)all*100)+"%", (double)fine/(double)all);
    pieseries->append("及格"+QString::number((double)pass/(double)all*100)+"%", (double)pass/(double)all);
    pieseries->append("不及格"+QString::number((double)notpass/(double)all*100)+"%",(double)notpass/(double)all);
    //人数条形图设置
    QBarSeries *barseries = new QBarSeries();
    QBarSet *set1 = new QBarSet("成绩");
    *set1 <<excellent<<great<<all-notpass<<notpass;
    barseries->append(set1);
    //设置人数条形图的X轴坐标参数
    QStringList categories;
    categories <<"优秀人数"<<"良好人数"<<"总及格人数"<<"不及格人数";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    //分数条形图设计
    QBarSeries *barseries2 = new QBarSeries();
    QBarSet *set2 = new QBarSet("成绩");
    double ave = (double)marks/(double)all;
    *set2 <<ave<<max<<min;
    barseries2->append(set2);
    //设置分数条形图的X轴坐标参数
    QStringList categories2;
    categories2 <<"平均分"<<"最高分"<<"最低分";
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories2);

    //加载饼状图
    QChartView *chartView1 = new QChartView();
    chartView1->setRenderHint(QPainter::Antialiasing);
    chartView1->chart()->setTitle("成绩分布");
    chartView1->chart()->addSeries(pieseries);
    chartView1->chart()->legend()->setAlignment(Qt::AlignBottom);
    chartView1->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView1->chart()->legend()->setFont(QFont("Arial", 15));

    //加载人数条形图
    QChartView *chartView2 = new QChartView();
    chartView2->setRenderHint(QPainter::Antialiasing);
    chartView2->chart()->setTitle("成绩指标");
    chartView2->chart()->addSeries(barseries);
    chartView2->chart()->createDefaultAxes();//设置默认坐标系，应该在添加series之后设置坐标轴
    chartView2->chart()->setAxisX(axis,barseries);
    chartView2->chart()->legend()->setVisible(true);
    chartView2->chart()->legend()->setAlignment(Qt::AlignBottom);
    chartView2->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView2->chart()->legend()->setFont(QFont("Arial", 15));

    //加载分数条形图
    QChartView *chartView3 = new QChartView();
    chartView3->setRenderHint(QPainter::Antialiasing);
    chartView3->chart()->setTitle("成绩指标");
    chartView3->chart()->addSeries(barseries2);
    chartView3->chart()->createDefaultAxes();//设置默认坐标系，应该在添加series之后设置坐标轴
    chartView3->chart()->setAxisX(axis2,barseries2);
    chartView3->chart()->legend()->setVisible(true);
    chartView3->chart()->legend()->setAlignment(Qt::AlignBottom);
    chartView3->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView3->chart()->legend()->setFont(QFont("Arial", 15));

    //设置布局
    chartlayout = new QGridLayout();
    chartlayout->addWidget(chartView1,0,0,1,2);
    chartlayout->addWidget(chartView2,1,0);
    chartlayout->addWidget(chartView3,1,1);

    win->setLayout(chartlayout);
    win->show();
}

void teacherManage::on_studentTableView_doubleClicked(const QModelIndex &index)
{
    if(ui->studentTableView->currentIndex().column()==2)
    {
        int row = ui->studentTableView->currentIndex().row();
        if(QMessageBox::information(this,"提醒","是否更改当前学生的成绩",QMessageBox::Button::Yes|QMessageBox::Button::No)==QMessageBox::Button::Yes)
        {
            bool ok;
            int grade = QInputDialog::getInt(this, tr("更改成绩"),tr("成绩:"), 25, 0, 100, 1, &ok);
            if (ok)
            {
                marks+=grade;
                if(infoModel->item(row,2)->text()=="还未设置成绩")
                {
                    all++;
                }
                else
                {
                    int g =infoModel->item(row,2)->text().toInt();
                    marks-=g;
                    if(g>=90)
                    {
                        excellent--;
                    }
                    else if(g>=80)
                    {
                        great--;
                    }
                    else if(g>=70)
                    {
                        fine--;
                    }
                    else if(g>=60)
                    {
                        pass--;
                    }
                    else
                    {
                        notpass--;
                    }
                }
                if(grade>=90)
                {
                    excellent++;
                }
                else if(grade>=80)
                {
                    great++;
                }
                else if(grade>=70)
                {
                    fine++;
                }
                else if(grade>=60)
                {
                    pass++;
                }
                else
                {
                    notpass++;
                }
                infoModel->setItem(row,2,new QStandardItem(QString::number(grade,10)));
                QSqlQuery query;
                query.prepare("select id from Teacher where username = '"+ui->tecID->text()+"'");
                query.exec();
                query.next();
                int tid = query.value(0).toInt();
                qDebug()<<"currentCourse:"+currentCourse;
                qDebug()<<"tid:"<<tid;
                query.prepare("select id from Course where courseName = '" + currentCourse + "' and tid = " +QString::number(tid,10));
                query.exec();
                query.next();
                int cid = query.value("id").toInt();
                qDebug()<<infoModel->item(row,1)->text();
                qDebug()<<"cid"<<cid;
                query.prepare("update SC set grade = "+ QString::number(grade,10) + " where sid = "+ infoModel->item(row,1)->text() + " and cid = "+ QString::number(cid,10));
                query.exec();
                query.next();
                if(all == infoModel->rowCount())
                {
                    setcharts();
                }
            }
        }
    }
}
