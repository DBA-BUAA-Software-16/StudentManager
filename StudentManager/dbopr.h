#ifndef DBOPR_H
#define DBOPR_H

#include <QObject>

class DBOpr : public QObject
{
    Q_OBJECT
public:
    explicit DBOpr(QObject *parent = nullptr);
    static bool signIn(QString Usr, QString pw,QString name, int type);
    static bool signUp(QString Usr, QString pw,QString name, int type);
    static bool changePassword(QString usr, QString oldpw, QString npw, int type);
    static bool createCourse(int courseHour, int createHour, QDateTime beginTime,QDateTime endTime, QDate term,int stuNum,int tid);
    static bool chooseCourse(int stuID,int courseID);
    static bool dropCourse(int stuID, int courseID);
    static bool setGrade(int stuID,int courseID,int score);

signals:

public slots:
};

#endif // DBOPR_H
