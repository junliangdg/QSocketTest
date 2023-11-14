#include "tcpserver.h"
#include "qapplication.h"
#include "qtimer.h"
#include <functional>

TcpServer::TcpServer(QObject *parent)
    : QTcpServer{parent}
{
    moveToThread(&th);
    th.start();
    connect(&th, &QThread::finished, this, &TcpServer::closeServer);
}

TcpServer::~TcpServer()
{
    th.quit();
    th.wait();

}

void TcpServer::listenServer(QHostAddress address, quint16 port)
{
    bool ret = listen(address, port);
    if (ret)
        emit serverStateChanged(ServerState::Listening);
    else
        emit serverStateChanged(ServerState::Closed);
}

void TcpServer::closeServer()
{
    for (auto &socket : connection_list)
    {
        delete socket.second;
    }
    if (this->isListening())
        close();
    connection_list.clear();
    emit emit peerListUpdated(getPeerList());
    emit serverStateChanged(ServerState::Closed);
}

void TcpServer::discardSocket()
{
    TcpSocket* socket = reinterpret_cast<TcpSocket*>(sender());
    QVector<QPair<QString, TcpSocket*>>::iterator it =
            std::find_if(connection_list.begin(), connection_list.end(),
                         [socket](const QPair<QString, TcpSocket*>& p){
      return p.second == socket;
    });

    if (it != connection_list.end()){
        qDebug() << QString("INFO :: %1 has just disconnected").arg(it->first);
        connection_list.erase(it);
        delete socket;
    }
    emit peerListUpdated(getPeerList());
}

void TcpServer::readSocket(QByteArray data)
{
    TcpSocket* socket = reinterpret_cast<TcpSocket*>(sender());
    emit dataRead(socket->getPeerString(), data);
}

QVector<QString> TcpServer::getPeerList()
{
    QVector<QString> list(0);
    for (auto &p : connection_list)
    {
        list.append(p.first);
    }
    return list;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    // new connection
    TcpSocket *socket = new TcpSocket(socketDescriptor);
    qDebug() << socket->getPeerString() << " Connecting...";

    connect(socket, &TcpSocket::disconnected, this, &TcpServer::discardSocket);
    connect(socket, &TcpSocket::dataRead, this, &TcpServer::readSocket);
    connect(this, &TcpServer::writeData, socket, &TcpSocket::writeSocket);

    connection_list.append(QPair<QString, TcpSocket*>(socket->getPeerString(), socket));
    qDebug() << getPeerList();
    emit peerListUpdated(getPeerList());
}
