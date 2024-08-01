#include "widget.h"
#include "./ui_widget.h"
#include"queue.h"
#include<synchapi.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFont font1("Arial",12);
    QFont font2("Arial",15);
    ui->labelTime->setFont(font1);
    ui->labelHour->setFont(font2);
    ui->labelMinute->setFont(font2);
    //定义顾客线程
    customerTask = new customerManagement;
    customerThread = new QThread(this);
    customerTask->moveToThread(customerThread);
    customerThread->start();

    //按下开始，所有同时线程启动
    connect(customerTask,SIGNAL(printcustomer(int,int,int,int,int)),this,SLOT(printcusdata(int,int,int,int,int)));
    connect(customerTask,SIGNAL(printbussiness(int,int,int,int,int)),this,SLOT(printbusdata(int,int,int,int,int)));

    connect(ui->btnStart,&QPushButton::clicked,customerTask,&customerManagement::runCustomer);

    //用两个label来显示当前时间
    connect(customerTask,&customerManagement::time_signal,[=](){
        ui->labelHour->setText(QString::number(myqueue.hour0));
    });
    connect(customerTask,&customerManagement::time_signal,[=](){
        ui->labelMinute->setText(QString::number(myqueue.minute0));
    });

    connect(this,&QObject::destroyed,[=](){
        customerThread->exit();
        customerThread->wait();
        delete customerTask;
    });

    //界面设计
    textbrowser1->move(10,30);
    textbrowser1->resize(420,250);
    textbrowser2->move(440,30);
    textbrowser2->resize(290,250);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(3);

    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    item=new QTableWidgetItem("窗口");
    ui->tableWidget->setItem(0, 0,item);
    item=new QTableWidgetItem("普通窗口1");
    ui->tableWidget->setItem(1, 0,item);
    item=new QTableWidgetItem("普通窗口2");
    ui->tableWidget->setItem(2, 0,item);
    item=new QTableWidgetItem("VIP窗口");
    ui->tableWidget->setItem(3, 0,item);
    item=new QTableWidgetItem("对公窗口1");
    ui->tableWidget->setItem(4, 0,item);
    item=new QTableWidgetItem("客户序号");
    ui->tableWidget->setItem(0,1,item);
    item=new QTableWidgetItem("排队人数");
    ui->tableWidget->setItem(0,2,item);

    for(int i=1;i<5;i++)
    {
        item=new QTableWidgetItem(QString::number(0));
        ui->tableWidget->setItem(i,1,item);
        item=new QTableWidgetItem(QString::number(0));
        ui->tableWidget->setItem(i,2,item);
    }

    //设置等宽,除第一列外等宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Widget::printcusdata(int a, int b, int c, int d, int e)//时、分、客户号、窗口号、时长
{
    //某时某刻几号客户在某窗口办理业务，预计时长
    QString text;
    if (a < 10)text += "0";
    text += QString::number(a);
    text += ":";
    if (b < 10)text += "0";
    text += QString::number(b);
    text += " ";
    text += QString::number(c);
    text += "号客户在";
    switch (d)
    {
    case 1:
        text += "普通窗口1办理业务 预计办理时长为 ";
        break;
    case 2:
        text += "普通窗口2办理业务 预计办理时长为 ";
        break;
    case 3:
        text += "VIP窗口办理业务 预计办理时长为 ";
        break;
    case 4:
        text += "对公窗口办理业务 预计办理时长为 ";
        break;
    }
    text += QString::number(e);
    text += "分钟";
    textbrowser1->append(text);
}

void Widget::printbusdata(int a,int b,int c,int d,int e)//a分b秒开始/结束服务
{
    //几号窗口于何时开始几号客户的服务，于何时结束几号客户的服务
    //c为窗口号 d为开始/结束 e为客户号
    QString text,n,waitnum;
    switch(c)
    {
    case 1:
        text+="普通窗口1 在";
        waitnum=QString::number(myqueue.queueNormal_1.size());
        item=new QTableWidgetItem(waitnum);
        ui->tableWidget->setItem(c,2,item);
        break;
    case 2:
        text+="普通窗口2 在";
        waitnum=QString::number(myqueue.queueNormal_2.size());
        item=new QTableWidgetItem(waitnum);
        ui->tableWidget->setItem(c,2,item);
        break;
    case 3:
        text+="VIP窗口 在";
        waitnum=QString::number(myqueue.queueVip.size());
        item=new QTableWidgetItem(waitnum);
        ui->tableWidget->setItem(c,2,item);
        break;
    case 4:
        text+="对公窗口 在";
        waitnum=QString::number(myqueue.queueOther.size());
        item=new QTableWidgetItem(waitnum);
        ui->tableWidget->setItem(c,2,item);
        break;
    }
    text+=QString::number(a);
    text+=":";
    if(b<10)text+="0";
    text+=QString::number(b);
    switch(d)
    {
    case 1:
        text+="开始";
        n=QString::number(e);
        item=new QTableWidgetItem(n);
        ui->tableWidget->setItem(c, 1,item);
        break;
    case 0:
        text+="结束";
        break;
    }
    text+=QString::number(e);
    text+="号客户的服务";
    textbrowser2->append(text);
}

Widget::~Widget()
{
    delete ui;
}
