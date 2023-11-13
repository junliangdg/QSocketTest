#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include "tcpsocketthread.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
signals:
    void peerStringUpdated(QVector<QString>);
private:
    QVector<QPair<QString, TcpSocketThread*>> connection_list;
    QSet<TcpSocketThread*> connection_set;
    QVector<QString> peerString;
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
