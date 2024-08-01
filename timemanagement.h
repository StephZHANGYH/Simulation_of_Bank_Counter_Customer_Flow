#ifndef TIMEMANAGEMENT_H
#define TIMEMANAGEMENT_H
#include<QDebug>
#include <QObject>
#include<QThread>

//负责管理时间线程
// extern int hour;
// extern int minute;
class timeManagement : public QObject
{
    Q_OBJECT
public:
    explicit timeManagement(QObject *parent = nullptr);
    void runTime();

signals:
         // void time_signal(int hour,int minute);
         // void printbussiness(int,int,int,int,int);//窗口号，开始/结束，客户号


};

#endif // TIMEMANAGEMENT_H
