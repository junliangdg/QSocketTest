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

void TcpServer::discardSocket()
{
    TcpSocket* socket = reinterpret_cast<TcpSocket*>(sender());
    QVector<QPair<QString, TcpSocket*>>::iterator it =
            std::find_if(connection_list.begin(), connection_list.end(),
                         [socket](const QPair<QString, TcpSocket*>& p){
      return p.second->socketDescriptor() == socket->socketDescriptor();
    });

    if (it != connection_list.end()){
        qDebug() << QString("INFO :: %1 has just disconnected").arg(it->first);
        connection_list.erase(it);
        delete socket;
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    // new connection
    TcpSocket *socket = new TcpSocket(socketDescriptor);
    qDebug() << socket->getPeerString() << " Connecting...";

    connect(socket, &TcpSocket::disconnected, this, &TcpServer::discardSocket);

    connection_list.append(QPair<QString, TcpSocket*>(socket->getPeerString(), socket));
}
