#include "customermanagement.h"
#include "queue.h"
#include <synchapi.h>
#include <random>
std::mt19937 rng(std::random_device{}());
customerManagement::customerManagement(QObject *parent)
    : QObject{parent}
{
    myqueue.hour0=9;
    myqueue.minute0=0;
}
void customerManagement::runCustomer()
{
    //注意修改文件路径
    QFile file(":/bank_data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Cannot open file for reading.";
        return;
    }

    // 读取文件内容
    QTextStream in(&file);

    while (!in.atEnd()||!myqueue.queueNormal_1.empty()||!myqueue.queueNormal_2.empty()||!myqueue.queueVip.empty()||!myqueue.queueOther.empty())
    {
        if(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split("\t"); // 使用制表符分割每行内容

            // 解析每行内容
            if (parts.size() >= 3) {
                QString time = parts[0];
                QString businessInfo = parts[2];
                QStringList timeparts = time.split(":");
                hour=timeparts[0].toInt();
                minute=timeparts[1].toInt();
                // 解析业务信息
                QStringList businessParts = businessInfo.split(", ");
                int flag=0;
                for (const QString& business : businessParts) {
                    QStringList businessCount = business.split(": ");
                    QString businessType = businessCount[0];
                    int customerCount = businessCount[1].toInt();

                    //统计对应业务类型的顾客数量
                    if(flag==0) cash=customerCount;
                    else if(flag==1) vip=customerCount;
                    else if(flag==2) other=customerCount;
                    flag++;
                }
                //emit send(hour,minute);
                inQueue();
            }
        }
        run();
        emit(time_signal(myqueue.hour0,myqueue.minute0));
        if(myqueue.minute0<59){
            myqueue.minute0++;
        }
        else{
            myqueue.hour0++;
            myqueue.minute0=0;
        }
        QThread::msleep(50);
    }
    //计算统计信息并输出
    avewait1=double(waittime1)/number1;
    avewait2=double(waittime2)/number2;
    avewait3=double(waittime3)/number3;
    double aveNormal=double(timeNormal)/numNormal;
    double aveVip=double(timeVip)/numVip;
    double aveOther=double(timeOther)/numOther;
    qDebug()<<"三个时段平均等待时间："<<avewait1<<"分钟 "<<avewait2<<"分钟 "<<avewait3<<"分钟";
    qDebug()<<"三种业务平均办理时间："<<aveNormal<<"分钟 "<<aveVip<<"分钟 "<<aveOther<<"分钟";

    // 关闭文件
    file.close();
}

void customerManagement::run()
{
    //对普通窗口1的客户队列进行业务办理后出队
    if(!myqueue.queueNormal_1.empty())
    {
        if(myqueue.hour0==myqueue.queueNormal_1.head()->h1&&myqueue.minute0==myqueue.queueNormal_1.head()->m1)
        {
            emit printbussiness(myqueue.queueNormal_1.head()->h1,myqueue.queueNormal_1.head()->m1,1,1,myqueue.queueNormal_1.head()->num);
        }
        if(myqueue.hour0==myqueue.queueNormal_1.head()->h2&&myqueue.minute0==myqueue.queueNormal_1.head()->m2)
        {
            emit printbussiness(myqueue.queueNormal_1.head()->h2,myqueue.queueNormal_1.head()->m2,1,0,myqueue.queueNormal_1.head()->num);
            myqueue.queueNormal_1.dequeue();
        }
    }
    //对普通窗口2的客户队列进行业务办理后出队
    if(!myqueue.queueNormal_2.empty())
    {
        if(myqueue.hour0==myqueue.queueNormal_2.head()->h1&&myqueue.minute0==myqueue.queueNormal_2.head()->m1)
        {
            emit printbussiness(myqueue.queueNormal_2.head()->h1,myqueue.queueNormal_2.head()->m1,2,1,myqueue.queueNormal_2.head()->num);
        }
        if(myqueue.hour0==myqueue.queueNormal_2.head()->h2&&myqueue.minute0==myqueue.queueNormal_2.head()->m2)
        {
            emit printbussiness(myqueue.queueNormal_2.head()->h2,myqueue.queueNormal_2.head()->m2,2,0,myqueue.queueNormal_2.head()->num);
            myqueue.queueNormal_2.dequeue();
        }
    }
    //对VIP窗口的客户队列进行业务办理后出队
    if(!myqueue.queueVip.empty())
    {
        if(myqueue.hour0==myqueue.queueVip.head()->h1&&myqueue.minute0==myqueue.queueVip.head()->m1)
        {
            emit printbussiness(myqueue.queueVip.head()->h1,myqueue.queueVip.head()->m1,3,1,myqueue.queueVip.head()->num);
        }
        if(myqueue.hour0==myqueue.queueVip.head()->h2&&myqueue.minute0==myqueue.queueVip.head()->m2)
        {
            emit printbussiness(myqueue.queueVip.head()->h2,myqueue.queueVip.head()->m2,3,0,myqueue.queueVip.head()->num);
            myqueue.queueVip.dequeue();
        }
    }
    //对对公窗口的客户队列进行业务办理后出队
    if(!myqueue.queueOther.empty())
    {
        if(myqueue.hour0==myqueue.queueOther.head()->h1&&myqueue.minute0==myqueue.queueOther.head()->m1)
        {
            emit printbussiness(myqueue.queueOther.head()->h1,myqueue.queueOther.head()->m1,4,1,myqueue.queueOther.head()->num);
        }
        if(myqueue.hour0==myqueue.queueOther.head()->h2&&myqueue.minute0==myqueue.queueOther.head()->m2)
        {
            emit printbussiness(myqueue.queueOther.head()->h2,myqueue.queueOther.head()->m2,4,0,myqueue.queueOther.head()->num);
            myqueue.queueOther.dequeue();
        }
    }
}

int customerManagement::findQueue()
{
    int flag = 0;
    if(myqueue.queueNormal_1.size()!=0&&myqueue.queueNormal_2.size()!=0&&myqueue.queueVip.size()==0)
        flag=3;
    else
    {
        if(myqueue.queueNormal_1.size()<=myqueue.queueNormal_2.size())
        {
            flag=1;
        }
        else
        {
            flag=2;
        }
    }
    return flag;
}

void customerManagement::cal(Mycus *cust)
{
    //计算当前客户的等待时长，为开始办理业务的时间与客户到达银行的时间之差
    if(cust->h1>cust->h0)
        waitm=60-cust->m0+cust->m1;
    else
        waitm=cust->m1-cust->m0;
    //分时段累加客户的等待时长 9~12、13~14、15~17
    if(cust->h0>8&&cust->h0<13) {
        waittime1+=waitm;
        number1++;
    }
    if(cust->h0>12&&cust->h0<15){
        waittime2+=waitm;
        number2++;
    }
    if(cust->h0>14) {
        waittime3+=waitm;
        number3++;
    }

}

void customerManagement::inQueue()
{
    //何时几号客户到达银行，在哪个窗口排队，预计处理时间
    int bussinessType;
    for(int i=0;i<cash;i++)
    {
        bussinessType=1;
        numNormal++;
        int time = generateTime(bussinessType);
        timeNormal+=time;
        int flag = findQueue();
        if(flag==1)
        {
            number++;//计算客户序号
            //计算客户开始办理业务的时间
            if(myqueue.queueNormal_1.empty()){
                starth=hour;
                startm=minute;
            }
            else{
                int j=myqueue.queueNormal_1.size()-1;//队尾下标
                starth=myqueue.queueNormal_1.at(j)->h1;
                startm=myqueue.queueNormal_1.at(j)->m1;
                startm+=myqueue.queueNormal_1.at(j)->sleeptime;
                if(myqueue.queueNormal_1.at(j)->m2==startm) startm++;
                if(startm>60){
                    startm-=60;
                    starth++;
                }
            }
            //计算客户结束业务办理的时间
            overm=startm+time;
            overh=starth;
            if(overm>59){
                overm-=60;
                overh++;
            }
            //创建客户对象cust并将其入队
            Mycus* cust = new Mycus{number,hour,minute,starth,startm,overh,overm,time,1};
            cal(cust);//统计cust的等待时长
            myqueue.queueNormal_1.enqueue(cust);//入队
            emit printcustomer(hour,minute,number,1,time);//在窗口输出客户到来的信息
        }

        else if(flag==2)
        {
            number++;
            if(myqueue.queueNormal_2.empty())
            {
                starth=hour;
                startm=minute;
            }
            else
            {
                int j=myqueue.queueNormal_2.size()-1;//队尾下标
                starth=myqueue.queueNormal_2.at(j)->h1;
                startm=myqueue.queueNormal_2.at(j)->m1;
                startm+=myqueue.queueNormal_2.at(j)->sleeptime;
                if(myqueue.queueNormal_2.at(j)->m2==startm) startm++;
                if(startm>60)
                {
                    startm-=60;
                    starth++;
                }
            }
            overm=startm+time;
            overh=starth;
            if(overm>59)
            {
                overm-=60;
                overh++;
            }
            Mycus* cust = new Mycus{number,hour,minute,starth,startm,overh,overm,time,1};
            cal(cust);
            myqueue.queueNormal_2.enqueue(cust);
            emit printcustomer(hour,minute,number,2,time);
        }
        else if(flag==3)
        {
            number++;
            if(myqueue.queueVip.empty())
            {
                starth=hour;
                startm=minute;
            }
            else
            {
                int j=myqueue.queueVip.size()-1;//队尾下标
                starth=myqueue.queueVip.at(j)->h1;
                startm=myqueue.queueVip.at(j)->m1;
                startm+=myqueue.queueVip.at(j)->sleeptime;
                if(myqueue.queueVip.at(j)->m2==startm) startm++;
                if(startm>60)
                {
                    startm-=60;
                    starth++;
                }
            }
            overm=startm+time;
            overh=starth;
            if(overm>59)
            {
                overm-=60;
                overh++;
            }
            Mycus* cust = new Mycus{number,hour,minute,starth,startm,overh,overm,time,1};
            cal(cust);
            myqueue.queueVip.enqueue(cust);
            emit printcustomer(hour,minute,number,3,time);
        }
    }
    for(int i=0;i<vip;i++)
    {
        bussinessType=2;
        numVip++;
        int time=generateTime(bussinessType);
        timeVip+=time;
        number++;
        if(myqueue.queueVip.empty())
        {
            starth=hour;
            startm=minute;
        }
        else
        {
            int j=myqueue.queueVip.size()-1;//队尾下标
            starth=myqueue.queueVip.at(j)->h1;
            startm=myqueue.queueVip.at(j)->m1;
            startm+=myqueue.queueVip.at(j)->sleeptime;
            if(myqueue.queueVip.at(j)->m2==startm) startm++;
            if(startm>60)
            {
                startm-=60;
                starth++;
            }
        }
        overm=startm+time;
        overh=starth;
        if(overm>59)
        {
            overm-=60;
            overh++;
        }
        Mycus* cust = new Mycus{number,hour,minute,starth,startm,overh,overm,time,1};
        cal(cust);
        myqueue.queueVip.enqueue(cust);
        emit printcustomer(hour,minute,number,3,time);
    }
    for(int i=0;i<other;i++)
    {
        bussinessType=3;
        numOther++;
        int time=generateTime(bussinessType);
        timeOther+=time;
        number++;
        if(myqueue.queueOther.empty())
        {
            starth=hour;
            startm=minute;
        }
        else
        {
            int j=myqueue.queueOther.size()-1;//队尾下标
            starth=myqueue.queueOther.at(j)->h1;
            startm=myqueue.queueOther.at(j)->m1;
            startm+=myqueue.queueOther.at(j)->sleeptime;
            if(myqueue.queueOther.at(j)->m2==startm) startm++;
            if(startm>60)
            {
                startm-=60;
                starth++;
            }
        }
        overm=startm+time;
        overh=starth;
        if(overm>59)
        {
            overm-=60;
            overh++;
        }
        Mycus* cust = new Mycus{number,hour,minute,starth,startm,overh,overm,time,1};
        cal(cust);
        myqueue.queueOther.enqueue(cust);
        emit printcustomer(hour,minute,number,4,time);
    }
}

int customerManagement::generateTime(int flag)
{
    // 创建分布对象，指定范围为[1, 15]
    if(flag==1){
        std::uniform_int_distribution<int> dist(1, 10);
        // 使用分布对象和随机数生成器生成随机数
        return dist(rng);}
    else if(flag==2){
        std::uniform_int_distribution<int> dist(4, 12);
        // 使用分布对象和随机数生成器生成随机数
        return dist(rng);}
    else if(flag==3){
        std::uniform_int_distribution<int> dist(7, 15);
        // 使用分布对象和随机数生成器生成随机数
        return dist(rng);}
    else return 0;
}

