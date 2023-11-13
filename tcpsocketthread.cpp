#include "tcpsocketthread.h"

TcpSocketThread::TcpSocketThread(qintptr socketDescriptor, QObject *parent)
    : QObject{parent}
{
    this->moveToThread(&th);
    th.start();

    socket = new QTcpSocket();
    // set the ID
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        // something's wrong
        emit error(socket->error());
    }
    connect(socket, &QTcpSocket::readyRead, this, &TcpSocketThread::readSocket);

}

TcpSocketThread::~TcpSocketThread()
{
    socket->close();
    socket->deleteLater();
    th.quit();
    th.wait();
}

QString TcpSocketThread::getPeerString()
{
    QString address = socket->peerAddress().toString();
    QString port = QString::number(socket->peerPort());
    return address + ":" + port;
}

void TcpSocketThread::writeSocket(QByteArray data)
{
    socket->write(data);
}

void TcpSocketThread::readSocket()
{
    // get the information
    QByteArray data = socket->readAll();

    emit dataRead(data);

    qDebug() << socket->socketDescriptor() << " Data in: " << data;
}
