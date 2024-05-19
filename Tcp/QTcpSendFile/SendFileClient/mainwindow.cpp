#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include "sendfile.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ip->setText("127.0.0.1");
    ui->port->setText("8899");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    //创建任务类对象
    SendFile* worker=new SendFile;

    //创建子线程
    QThread* td=new QThread;

    //将任务对象放入子线程中
    worker->moveToThread(td);

    connect(this,&MainWindow::startConnect,worker,&SendFile::connectServer);
    connect(this,&MainWindow::sendfile,worker,&SendFile::sendFile);

    connect(worker,&SendFile::sendPercent,ui->progressBar,&QProgressBar::setValue);

    //处理子线程发送的数据
    connect(worker,&SendFile::connectOk,this,[=](){
        QMessageBox::information(this,"连接服务器","成功连接服务器...");

    });
    connect(worker,&SendFile::connectOver,this,[=](){
        td->quit();
        td->wait();
        worker->deleteLater();
        td->deleteLater();
    });



    td->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    QString ip=ui->ip->text();
    unsigned short port=ui->port->text().toUShort();
    emit startConnect(ip,port);
}


void MainWindow::on_selectFile_clicked()
{
    QString path=QFileDialog::getOpenFileName();
    if(path.isEmpty()){
        QMessageBox::warning(this,"打开文件","打开文件不能为空！");
        return;
    }
    ui->FilePath->setText(path);

}


void MainWindow::on_sendFile_clicked()
{
    emit sendfile(ui->FilePath->text());
}

