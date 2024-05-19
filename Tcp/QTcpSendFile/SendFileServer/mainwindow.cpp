#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recvfile.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("8899");
    tcp=new QTcpServer(this);

    connect(tcp,&QTcpServer::newConnection,this,[=](){
        QTcpSocket* m_tcp=tcp->nextPendingConnection();
        RecvFile* threadson=new RecvFile(m_tcp);
        threadson->start();

        connect(threadson,&RecvFile::ok,this,[=](){
            threadson->exit();
            threadson->wait();
            threadson->deleteLater();
            QMessageBox::information(this,"接收文件","接收文件完毕...");

        });
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startServer_clicked()
{
    unsigned short port=ui->port->text().toUShort();
    tcp->listen(QHostAddress::Any,port);

}

