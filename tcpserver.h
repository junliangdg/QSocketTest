#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QThread>
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
signals:

private slots:
    void discardSocket();

private:
    QVector<QPair<QString, TcpSocket*>> connection_list;
    QThread th;
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
