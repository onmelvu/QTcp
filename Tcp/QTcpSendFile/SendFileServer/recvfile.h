#ifndef RECVFILE_H
#define RECVFILE_H

#include <QThread>
#include <QFile>
#include <QTcpSocket>

class RecvFile : public QThread
{
    Q_OBJECT
public:
    explicit RecvFile(QTcpSocket* tcp,QObject *parent = nullptr);

    void run() override;

signals:
    void ok();
private:
    QTcpSocket* m_tcp;
};

#endif // RECVFILE_H
