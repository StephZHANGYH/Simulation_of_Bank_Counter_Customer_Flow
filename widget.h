#ifndef WIDGET_H
#define WIDGET_H

#include<QString>
#include <QWidget>
#include<QTextBrowser>
#include"customermanagement.h"
#include<QQueue>
#include <QTableWidget>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();



private:
    Ui::Widget *ui;
    customerManagement* customerTask;
    QThread* timeThread;
    QThread* bussinessThread;
    QThread* customerThread;
    QTextBrowser *textbrowser1=new QTextBrowser(this);
    QTextBrowser *textbrowser2=new QTextBrowser(this);
    QTableWidgetItem *item=new QTableWidgetItem;


public slots:
    void printcusdata(int, int, int, int, int);
    void printbusdata(int ,int ,int ,int ,int);//a分b秒开始/结束服务
};
#endif // WIDGET_H
