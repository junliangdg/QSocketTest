#include "tcpsocket.h"

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent)
    : QTcpSocket{parent}
{
    setSocketDescriptor(socketDescriptor);
    moveToThread(&th);
    th.start();
    connect(this, &QTcpSocket::readyRead, this, &TcpSocket::readSocket);
    connect(&th, &QThread::finished, this, &TcpSocket::close);
}

TcpSocket::~TcpSocket()
{
    th.quit();
    th.wait();
}

QString TcpSocket::getPeerString()
{
    QString address = peerAddress().toString();
    QString port = QString::number(peerPort());
    return address + ":" + port;
}

void TcpSocket::readSocket()
{
    // get the information
    QByteArray data = readAll();

    emit dataRead(data);

    qDebug() << socketDescriptor() << " Data in: " << data;
}
