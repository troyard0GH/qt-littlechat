#include "mytcplink.h"

MyTcpLink::MyTcpLink(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    tcpFileSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(transmitBuild()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(transmitClose()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(tcpFileSocket,SIGNAL(connected()),this,SLOT(transmitFileBuild()));
    connect(tcpFileSocket,SIGNAL(readyRead()),this,SLOT(receiveFileData()));

    sendfileName.clear();
    sendfileSize = 0;
    sendSize = 0;

    recvfileName.clear();
    recvfileSize = 0;
    recvSize = 0;

    headsize = 0;
    ishead = true;
}

void MyTcpLink::buildConnection(QString IP,qint16 Port){
    tcpSocket->connectToHost(QHostAddress(IP),Port);
    if(!tcpSocket->waitForConnected()){
        qDebug() << "connetFalse";
    }
    tcpFileSocket->connectToHost(QHostAddress(IP),9999);
    if(!tcpFileSocket->waitForConnected()){
        qDebug() << "connetFileFalse";
    }
}

void MyTcpLink::sendData(QString senddata){

    tcpSocket->write(senddata.toUtf8().data());
    if(!tcpSocket->waitForBytesWritten()){
        qDebug() << "writeFalse";
    }
}

void MyTcpLink::addFileHandle(QString filePath){
    sendfileInfo.setFile(filePath);
    sendfileName = sendfileInfo.fileName();
    sendfileSize = sendfileInfo.size();
    sendSize = 0;

}

void MyTcpLink::sendFileHandle(){
    QString head = QString("%1##%2##").arg(sendfileName).arg(sendfileSize);
    head.resize(128,'-');
    qint64 len = tcpFileSocket->write(head.toUtf8());
    if(len <=  0){
        qDebug() << "Failed to send Head";
    }

    //
    sendfile.setFileName(sendfileInfo.absoluteFilePath());
    if(sendfile.open(QIODevice::ReadOnly) == true){
        char perData[6*1024] = {0};
        qint64 readlen = 0;
        qint64 sendlen = 0;
        do{
            readlen = sendfile.read(perData,sizeof(perData));
            sendlen = tcpFileSocket->write(perData,readlen);
            sendSize += sendlen;
        }while(sendlen > 0);

        if(sendSize == sendfileSize){
            qDebug() << "File send success";
        }else{
            qDebug() << "File send warning";
        }
        sendfile.close();
    }else{
        qDebug() << "Failed to open file in ReadOnly mode";
    }
    emit fileSendResult(sendSize == sendfileSize);
    sendSize = 0;

}

void MyTcpLink::closeConnection(){

    tcpSocket->disconnectFromHost();
    if(!tcpSocket->waitForDisconnected()){
        qDebug() << "disconnet1False";
    }
    tcpSocket->close();

    tcpFileSocket->disconnectFromHost();
    if(!tcpFileSocket->waitForDisconnected()){
        qDebug() << "disconnet2False";
    }
    tcpFileSocket->close();
}

void MyTcpLink::transmitBuild(){
    QString ip = tcpSocket->peerAddress().toString();
    qint16 port = tcpSocket->peerPort();
    qDebug() << QString("%1 %2").arg(ip).arg(port);
    QString peerinfo = QString("[%1:%2]").arg(ip).arg(port);
    emit connectionBuilt(peerinfo);
}

void MyTcpLink::transmitFileBuild(){
//    QString ip = tcpfileSocket->peerAddress().toString();
//    qint16 port = tcpfileSocket->peerPort();
//    qDebug() << QString("%1 %2").arg(ip).arg(port);
//    QString peerinfo = QString("[%1:%2]").arg(ip).arg(port);
//    emit connectionBuilt(peerinfo);
}

void MyTcpLink::receiveData(){
    QByteArray data = tcpSocket->readAll();
    emit dataFromPeer(QString(data));
}

void MyTcpLink::receiveFileData(){
    qDebug() << "readyRead cufa" << headsize;
    static QByteArray head;
    QByteArray headdata = tcpFileSocket->read(128-headsize);
    qDebug() << headdata.size();
    headsize += headdata.size();
    head.append(headdata);
    if(headsize < 128){
        ishead = true;
        return;
    }else{
        if(ishead == true){
            recvfileName = QString(head).section("##",0,0);
            recvfileSize = (QString(head).section("##",1,1)).toInt();
            recvSize = 0;
            recvfile.setFileName(QString("./%1").arg(recvfileName));
            qDebug() << "recvfile" << recvfileSize ;
            if(recvfile.open(QIODevice::WriteOnly) == false){
                qDebug() << "Failed to open recvfile";
                return;
            }
            ishead = false;
        }
    }
    QByteArray data = tcpFileSocket->readAll();
    qint64 recvlen = recvfile.write(data);
    recvSize += recvlen;

    //qDebug() << "recvSize " << recvSize;
    if(recvSize >= recvfileSize){
        headsize = 0;
        ishead = true;
        head.clear();
        emit fileRecvResult();
        recvfile.close();
    }
}

void MyTcpLink::transmitClose(){
    emit connectionClose();
}
