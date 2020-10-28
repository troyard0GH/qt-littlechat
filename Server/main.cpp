#include "widget.h"
#include "mytcplink.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyTcpLink link1;
    Widget w;
//    QObject::connect(&w,SIGNAL(connetToPeer(QString)),&link1,SLOT(buildConnection(QString IPInfo)));
    QObject::connect(&w,SIGNAL(sendToPeer(QString)),&link1,SLOT(sendData(QString)));
    QObject::connect(&w,SIGNAL(disconnectFromPeer()),&link1,SLOT(closeConnection()));
    QObject::connect(&w,SIGNAL(addFile(QString)),&link1,SLOT(addFileHandle(QString)));
    QObject::connect(&w,SIGNAL(sendFileToPeer(QString)),&link1,SLOT(sendFileHandle()));

    QObject::connect(&link1,SIGNAL(fileSendResult(bool)),&w,SLOT(fileSendRes(bool)));
    QObject::connect(&link1,SIGNAL(fileRecvResult()),&w,SLOT(fileRecvRes()));
    QObject::connect(&link1,SIGNAL(connectionBuilt(QString)),&w,SLOT(newConnectionBuilt(QString)));
    QObject::connect(&link1,SIGNAL(dataFromPeer(QString)),&w,SLOT(newDataCome(QString)));
    QObject::connect(&link1,SIGNAL(connectionClose()),&w,SLOT(connectionClose()));
    w.show();
    return a.exec();
}
