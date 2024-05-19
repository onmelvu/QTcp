#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("服务器");
    ui->port->setText("8899");

    //创建监听的服务器对象
    m_s=new QTcpServer(this);

    connect(m_s,&QTcpServer::newConnection,this,[=](){
        m_tcp = m_s->nextPendingConnection();

        m_status->setPixmap(QPixmap(":/connect.png").scaled(20,20));
        //检测是否可以接收数据
        connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
            QByteArray data = m_tcp->readAll();
            ui->historyText->append("客户端说：" + data);
        });

        connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
            m_tcp->close();
            //m_tcp->deleteLater();//or  -> delete m_tcp;
            m_status->setPixmap(QPixmap(":/disconnect.png").scaled(20,20));
        });
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

void MainWindow::on_setServer_clicked()
{
    unsigned port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);
    ui->setServer->setDisabled(true);
}


void MainWindow::on_sendMess_clicked()
{
    QString msg = ui->sendText->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->historyText->append("服务器说："+msg);
    ui->sendText->clear();
}

