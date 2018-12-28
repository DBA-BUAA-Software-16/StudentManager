#ifndef TEACHERMANAGE_H
#define TEACHERMANAGE_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QMainWindow>
#include <QtCharts>
#include <QInputDialog>
#include <QSqlQuery>
#include "passwordChangeDialog.h"
#include "createCourseDialog.h"
using namespace QtCharts;
namespace Ui {
class teacherManage;
}

class teacherManage : public QWidget
{
    Q_OBJECT

public:
    explicit teacherManage(QWidget *parent = 0);
    ~teacherManage();
signals:
    void toDialog();
    void toPasswordChange(QString);
    void tocreateCourseDialog(QString);
private slots:
    void comeDialog(QString,QString);
    void qtimeSlot();
    void on_exitButton_clicked();
    void on_logoutButton_clicked();

    void on_passwordChangeButton_clicked();

    void on_createCourseButto_clicked();

    void on_coursestudentButton_clicked();

    void on_allcourseTableView_clicked(const QModelIndex &index);

    void on_studentTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::teacherManage *ui;
    passwordChangeDialog *change;
    createCourseDialog *createCourse;
    QStandardItemModel *courseModel;
    QStandardItemModel *infoModel;
    QWidget *win = NULL;
    QGraphicsScene scene;
    QGridLayout *chartlayout;
    QString currentCourse;
    int all,excellent,great,fine,pass,notpass,marks,max,min;
    void setallcourseTableView();
    void setstudentTableView(int);
    void setcharts();
};

#endif // TEACHERMANAGE_H
