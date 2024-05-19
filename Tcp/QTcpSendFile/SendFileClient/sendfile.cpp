#include "sendfile.h"
#include <QFileInfo>

SendFile::SendFile(QObject *parent)
    : QObject{parent}
{}

void SendFile::connectServer( QString ip,unsigned short port)
{
    m_tcp=new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip),port);
    connect(m_tcp,&QTcpSocket::connected,this,&SendFile::connectOk);
    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        m_tcp->close();
        m_tcp->deleteLater();
        emit connectOver();
    });
}

void SendFile::sendFile(QString path)
{
    QFile file(path);
    QFileInfo info(path);
    int filesize=info.size();

    file.open(QFile::ReadOnly);
    while(!file.atEnd()){
        static int num=0;
        if(num==0){
            m_tcp->write((char*)&filesize,4);
        }
        QByteArray line=file.readLine();

        num+=line.size();

        int percent=(num*100/filesize);
        emit sendPercent(percent);
        m_tcp->write(line);
    }

}
