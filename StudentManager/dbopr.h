#ifndef DBOPR_H
#define DBOPR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>
class DBOpr : public QObject
{
    Q_OBJECT
public:
    explicit DBOpr(QObject *parent = nullptr);
    static bool signUp(int type,QString Usr, QString pw,QString name);
    static int signIn(int type,QString Usr, QString pw);
    static bool changePassword(int type, int id, QString npw);
    static bool createCourse(int tid,int courseHour, int createHour, QDateTime beginTime,QDateTime endTime,int stuNum);
    static bool chooseCourse(int stuID,int courseID);
    static bool dropCourse(int stuID, int courseID);
    static bool setGrade(int stuID,int courseID,int score);

signals:

public slots:
};

#endif // DBOPR_H
