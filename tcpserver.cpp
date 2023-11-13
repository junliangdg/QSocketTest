#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent)
    : QTcpServer{parent}
{


}

TcpServer::~TcpServer()
{
    for (auto &socket : connection_list)
    {
        delete socket.second;
    }

    this->close();
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";
    TcpSocketThread *socket = new TcpSocketThread(socketDescriptor);
    connection_list.append(QPair<QString, TcpSocketThread*>(socket->getPeerString(), socket));
    peerString.append(socket->getPeerString());
    qDebug() << peerString;
    emit peerStringUpdated(peerString);
}
