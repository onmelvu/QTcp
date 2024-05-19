#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>

class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);

    void connectServer(QString ip,unsigned short port);

    void sendFile(QString path);

signals:
    void connectOk();
    void connectOver();
    void sendPercent(int perc);
private:
    QTcpSocket* m_tcp;
};

#endif // SENDFILE_H
