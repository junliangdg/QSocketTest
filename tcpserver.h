#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QThread>
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    enum ServerState
    {
        Closed,
        Listening,
    };
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
    void writeData(QString peerString, QByteArray data);
public slots:
    void listenServer(QHostAddress address, quint16 port);
    void closeServer();
    void disconnectClient(QString peer);
signals:
    void errorOccurred(QString);
    void clientConnected(QString);
    void clientDisconnected(QString);
    void peerListUpdated(QVector<QString>);
    void dataRead(QString, QByteArray);
    void serverStateChanged(ServerState);
private slots:
    void discardSocket();
    void readSocket(QByteArray data);
private:
    QVector<QString> getPeerList();
    QVector<QPair<QString, TcpSocket*>> connection_list;
    QThread th;
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
