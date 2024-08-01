#ifndef QUEUE_H
#define QUEUE_H

#include <QQueue>
#include <QObject>
struct Mycus {
    int num;//客户序号
    int h0,m0;//客户到达时间
    int h1,m1;//客户开始接受服务时间
    int h2,m2;//客户结束服务时间
    int sleeptime;//客户办理业务所需时长
    int win;//客户办理业务所在窗口
};
class Myqueue : public QObject
{
    Q_OBJECT
public:
    int isatend;//是否读到文件末尾（后续有无客户继续到来）
    //各个窗口的客户队列
    QQueue<Mycus*> queueNormal_1;//普通窗口1客户队列
    QQueue<Mycus*> queueNormal_2;//普通窗口2客户队列
    QQueue<Mycus*> queueVip;//VIP窗口客户队列
    QQueue<Mycus*> queueOther;//对公窗口客户队列
    //当前时间
    int hour0;
    int minute0;
    Myqueue();
};

extern Myqueue myqueue;

#endif // QUEUE_H
