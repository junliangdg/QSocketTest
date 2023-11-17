#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QThread>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();
private slots:
    void readSocket();
signals:
    void dataRead(QByteArray);
private:
    QThread th;
};

#endif // TCPCLIENT_H
