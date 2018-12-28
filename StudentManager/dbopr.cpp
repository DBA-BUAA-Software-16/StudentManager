#include "dbopr.h"

DBOpr::DBOpr(QObject *parent) : QObject(parent)
{

}

bool DBOpr::signUp(int type,QString usr, QString pw, QString name){
    bool flag = true;
    QSqlQuery query;
    query.prepare("CALL signUp(?,?,?,?)");
    query.bindValue(0,type);
    query.bindValue(1,usr);
    query.bindValue(2,pw);
    query.bindValue(3,name);
    if(!query.exec()){
        flag = false;
    }
    return flag;
}

int DBOpr::signIn(int type,QString usr, QString pw){
    QSqlQuery query;
    if(type == 0){
        query.prepare("SELECT MAX(id) as curid FROM Student where username = '" + usr +"' and password = '" + pw +"'");
    }
    else {
        query.prepare("SELECT MAX(id) as curid FROM Teacher where username = '"+ usr +"' and password = '" + pw +"'");
    }
    query.exec();
    if(query.next()){
        return query.record().value(0).toInt();
    }
    return -1;
}

bool DBOpr::changePassword(int type, int id, QString npw){
    bool flag = true;
    QSqlQuery query;
    query.prepare("CALL changepassword(?,?,?)");
    query.bindValue(0,type);
    query.bindValue(1,id);
    query.bindValue(2,npw);
    if(!query.exec()){
        flag = false;
    }
    return flag;
}

bool DBOpr::createCourse(int tid,int courseHour, int createHour, QDateTime beginTime, QDateTime endTime, int stuNum){
    bool flag = true;
    QSqlQuery query;
    query.prepare("CALL new_course(?,?,?,?,?,?)");
    query.bindValue(0,courseHour);
    query.bindValue(1,createHour);
    query.bindValue(2,beginTime);
    query.bindValue(3,endTime);
    query.bindValue(4,tid);
    query.bindValue(5,stuNum);
    if(!query.exec()){
        flag = false;
    }
    return flag;
}

bool DBOpr::chooseCourse(int stuID, int courseID){

}
