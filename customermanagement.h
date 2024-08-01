#ifndef CUSTOMERMANAGEMENT_H
#define CUSTOMERMANAGEMENT_H
#include<QFile>
#include<QThread>
#include <QObject>
#include<QDebug>
#include"queue.h"
//负责管理顾客线程；读、处理数据文件；对每个到达的顾客进行分类分流

class customerManagement : public QObject
{
    Q_OBJECT
public:

    explicit customerManagement(QObject *parent = nullptr);
    void runCustomer();
    int generateTime(int);
    void inQueue();
    int findQueue();//寻找最短队列
    void run();
    void cal(Mycus*);

signals:
    void customer_signal();
    void send(int,int);
    void sendtime(int,int);
    void printcustomer(int,int,int,int,int);
    void printbussiness(int,int,int,int,int);//窗口号，开始/结束，客户号
    void time_signal(int hour,int minute);

private:
    int cash=0,vip=0,other=0,number=0,number1=0,number2=0,number3=0;
    int hour,minute;//记录客户到达银行的时间
    int waitm=0,starth,startm,overh,overm;//用于存储初始化客户所需要的数据信息
    //统计三个时间段的等待时间
    int waittime1=0,waittime2=0,waittime3=0;
    double avewait1,avewait2,avewait3;
    //统计每种业务的平均办理时间
    int timeNormal;
    int timeVip;
    int timeOther;
    int numNormal;
    int numVip;
    int numOther;
};

#endif // CUSTOMERMANAGEMENT_H
