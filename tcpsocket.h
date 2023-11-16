#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QThread>
#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = nullptr);
    ~TcpSocket();
    QString getPeerString();
public slots:
    void writeSocket(QByteArray data);
signals:
    void dataRead(QByteArray);
private slots:
    void readSocket();
private:
    QThread th;
};

#endif // TCPSOCKET_H
