#include "recvfile.h"

RecvFile::RecvFile(QTcpSocket* tcp,QObject *parent)
    : QThread{parent}
{
    m_tcp=tcp;
}

void RecvFile::run()
{
    QFile* file=new QFile("recv.txt");
    file->open(QFile::WriteOnly);

    //接收数据
    connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
        static int num=0;
        static int total=0;
        if(num==0){
            m_tcp->read((char*)&total,4);
        }
        QByteArray list=m_tcp->readAll();

        file->write(list);
        num+=list.size();

        //查看是否接收完全
        if(num==total){
            m_tcp->close();
            m_tcp->deleteLater();
            file->close();
            file->deleteLater();
            emit ok();
        }
    });

    exec();
}


