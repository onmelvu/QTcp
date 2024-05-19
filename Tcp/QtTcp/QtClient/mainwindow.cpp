#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("客户端");
    ui->port->setText("8899");
    ui->ip->setText("127.0.0.1");

    ui->disconnect->setDisabled(true);

    m_tcp=new QTcpSocket(this);



    connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
        QByteArray data = m_tcp->readAll();
        ui->historyText->append("服务器说：" + data);
    });

    connect(m_tcp,&QTcpSocket::connected,this,[=](){
        m_status->setPixmap(QPixmap(":/connect.png").scaled(20,20));
        ui->historyText->append("连接成功！");
        ui->disconnect->setEnabled(true);
        ui->connect->setDisabled(true);
    });

    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        m_status->setPixmap(QPixmap(":/disconnect.png").scaled(20,20));
        ui->historyText->append("客户端和服务器断开连接！");
        ui->disconnect->setEnabled(false);
        ui->connect->setDisabled(false);
    });

    m_status=new QLabel;
    m_status->setPixmap(QPixmap(":/disconnect.png").scaled(20,20));
    ui->statusbar->addWidget(new QLabel("连接状态："));
    ui->statusbar->addWidget(m_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_send_clicked()
{
    QString msg = ui->sendText->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->historyText->append("客户端说："+msg);
    ui->sendText->clear();
}


void MainWindow::on_connect_clicked()
{
    unsigned short port=ui->port->text().toUShort();
    QString ip=ui->ip->text();
    m_tcp->connectToHost(QHostAddress(ip),port);
}


void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    ui->disconnect->setEnabled(false);
    ui->connect->setDisabled(false);
}

