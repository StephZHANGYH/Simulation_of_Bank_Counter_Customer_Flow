#ifndef BUSSINESSMANAGEMENT_H
#define BUSSINESSMANAGEMENT_H
//#include"customermanagement.h"
#include <QObject>
#include<QDebug>
#include<QThread>

//负责管理业务线程；处理4个窗口队列
class bussinessManagement : public QObject
{
    Q_OBJECT
public:
    explicit bussinessManagement(QObject *parent = nullptr);
    void runNormal_1();
    void runNormal_2();
    void runVip();
    void runOther();
    void runBussiness();


signals:
    void normal_signal_1();
    void normal_signal_2();
    void vip_signal();




private:
    int arrivetime,number=0,sleeptime,h=0,m=0;
    //void inQueue();//安排所有顾客进入各自队列

    //QTimer timer;
    int normal1wait;
    int normal2wait;
    int vipwait;
    int otherwait;
    //int flag;


public slots:
    void accept(int a,int b)
    {
        h=a;m=b;
        //qDebug()<<h<<m;
    }
};

#endif // BUSSINESSMANAGEMENT_H
