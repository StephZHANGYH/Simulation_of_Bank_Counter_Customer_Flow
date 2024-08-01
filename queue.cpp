#include "queue.h"

Myqueue::Myqueue() : QObject(), isatend(0)//令isatend初始化为0
{
    //初始化，清空每一个队列
    queueNormal_1.clear();
    queueNormal_2.clear();
    queueVip.clear();
    queueOther.clear();
}
Myqueue myqueue;
