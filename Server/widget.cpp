#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(700,500);
    move(200,200);

    ed1 = new QTextEdit(this);
    ed1->setReadOnly(true);
    ed2 = new QTextEdit(this); 
    btn1 = new QPushButton("Send",this);
    btn1->setDisabled(true);
    btn2 = new QPushButton("Disconnect",this);
    btn2->setDisabled(true);
    btnAddFile = new QPushButton("Add File..",this);
    btnAddFile->setDisabled(true);
    btnSendFile = new QPushButton("Send File",this);
    btnSendFile->setDisabled(true);

    QGridLayout * gridlayout1 = new QGridLayout(this);

    connect(btn1,SIGNAL(clicked()),this,SLOT(sendData()));
    connect(btn2,SIGNAL(clicked()),this,SLOT(closeConnection()));
    connect(btnAddFile,SIGNAL(clicked()),this,SLOT(addFile()));
    connect(btnSendFile,SIGNAL(clicked()),this,SLOT(sendFile()));

//    gridlayout1->addWidget(addbtn,3,1,1,1);
    gridlayout1->addWidget(ed1,1,1,1,4);
    gridlayout1->addWidget(ed2,2,2,1,3);
    gridlayout1->addWidget(btnAddFile,2,1,1,1);
    gridlayout1->addWidget(btnSendFile,3,1,1,1);
    gridlayout1->addWidget(btn1,3,3,1,1);
    gridlayout1->addWidget(btn2,3,4,1,1);

    gridlayout1->setColumnStretch(1,10);
    gridlayout1->setColumnStretch(2,10);
    gridlayout1->setColumnStretch(3,10);
    gridlayout1->setColumnStretch(4,10);
    gridlayout1->setRowStretch(1,30);
    gridlayout1->setRowStretch(2,9);
    gridlayout1->setRowStretch(3,1);
    gridlayout1->setSpacing(8);
    gridlayout1->setMargin(10);
    setLayout(gridlayout1);

    setWindowTitle("Server");
}

Widget::~Widget()
{

}

void Widget::addFile(){
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open File"),"../",tr("text(*.*)"));
    if(filepath.isEmpty() == false){
        btnAddFile->setText(QString("%1").arg(filepath));
        btnSendFile->setEnabled(true);
    }
    emit addFile(filepath);

}

void Widget::sendFile(){
    btnAddFile->setEnabled(false);
    emit sendFileToPeer(btnAddFile->text());
}

void Widget::sendData(){
    ed1->append(QString("[%1]: %2").arg(QHostAddress::LocalHost).arg(ed2->toPlainText()));
    emit sendToPeer(ed2->toPlainText());
    ed2->clear();
}

void Widget::closeConnection(){
    emit disconnectFromPeer();
}

void Widget::newConnectionBuilt(QString peerInfo){
    ed1->append(QString("[%1]: connection built").arg(peerInfo));
    btn1->setDisabled(false);
    btn2->setDisabled(false);
    btnAddFile->setDisabled(false);
}

void Widget::newDataCome(QString data){
    ed1->append(QString("[Peer]: %1").arg(data));
}

void Widget::connectionClose(){
    btn1->setDisabled(true);
    btn2->setDisabled(true);
    btnAddFile->setDisabled(true);
    btnSendFile->setDisabled(true);
    ed1->append(QString("[Peer]: disconnected"));
}

void Widget::fileSendRes(bool res){
    btnAddFile->setEnabled(true);
    btnAddFile->setText("Add File..");
    btnSendFile->setEnabled(false);
    ed1->append(QString("[%1]: File Send Result %2").arg(QHostAddress::LocalHost).arg(res));
}

void Widget::fileRecvRes(){
    ed1->append(QString("File received"));
}
