#ifndef TCPSOCKETTHREAD_H
#define TCPSOCKETTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class TcpSocketThread : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocketThread(qintptr socketDescriptor, QObject *parent = nullptr);
    ~TcpSocketThread();
    QString getPeerString();
public slots:
    void writeSocket(QByteArray data);
signals:
    void error(QAbstractSocket::SocketError);
    void dataRead(QByteArray);
private slots:
    void readSocket();
private:
    QTcpSocket *socket;
    QThread th;
};

#endif // TCPSOCKETTHREAD_H
