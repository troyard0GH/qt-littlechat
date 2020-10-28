#ifndef MYTCPLINK_H
#define MYTCPLINK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QTimer>

class MyTcpLink : public QObject
{
    Q_OBJECT

public:
    explicit MyTcpLink(QObject *parent = nullptr);

public slots:
    void sendData(QString data);
    void addFileHandle(QString filePath);
    void sendFileHandle();
    void closeConnection();


private slots:
    void transmitBuild();
    void transmitFileBuild();
    void receiveData();
    void receiveFileData();
    void transmitClose();

signals:
    void connectionBuilt(QString);
    void dataFromPeer(QString);
    void fileSendResult(bool);
    void fileRecvResult();
    void connectionClose();


private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QTcpServer *tcpfileServer;
    QTcpSocket *tcpfileSocket;

    QFile sendfile;
    QFileInfo sendfileInfo;
    QString sendfileName;
    qint64 sendfileSize;
    qint64 sendSize;

    QFile recvfile;
    QFileInfo recvfileInfo;
    QString recvfileName;
    qint64 recvfileSize;
    qint64 recvSize;

    qint64 headsize;
    bool ishead;
};

#endif // MYTCPLINK_H
