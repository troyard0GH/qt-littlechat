#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QDockWidget>
#include <QDebug>
#include <QListWidget>
#include <QInputDialog>
#include <QHostAddress>
#include <QFileDialog>

#include "mytcplink.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void newConnectionBuilt(QString);
    void newDataCome(QString);
    void connectionClose();
    void fileSendRes(bool res);
    void fileRecvRes();

private slots:
    void getIPInfo();
    void closeConnection();
    void sendData();
    void addFile();
    void sendFile();

signals:
    void connetToPeer(QString,qint16);    
    void addFile(QString);
    void sendFileToPeer(QString);
    void sendToPeer(QString);
    void disconnectFromPeer();

private:
    QTextEdit *ed1;
    QTextEdit *ed2;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *addbtn;
    QPushButton *btnAddFile;
    QPushButton *btnSendFile;
    MyTcpLink *tcplink1;
};
#endif // WIDGET_H
