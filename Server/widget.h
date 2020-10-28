#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QDebug>
#include <QListWidget>
#include <QInputDialog>
#include <QHostAddress>
#include <QFileDialog>

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
    void closeConnection();
    void sendData();
    void addFile();
    void sendFile();

signals:
    void sendToPeer(QString);
    void addFile(QString);
    void sendFileToPeer(QString);
    void disconnectFromPeer();

private:
    QTextEdit *ed1;
    QTextEdit *ed2;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btnAddFile;
    QPushButton *btnSendFile;
};
#endif // WIDGET_H
