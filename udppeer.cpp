#include "udppeer.h"
#include "qabstractsocket.h"
#include "qudpsocket.h"


UdpPeer::UdpPeer(QObject *parent)
    : QUdpSocket{parent}
{
    moveToThread(&th);
    th.start();
    connect(this, &UdpPeer::readyRead, this, &UdpPeer::readDatagram);
    connect(&th, &QThread::finished, this, &UdpPeer::close);
    connect(this, &UdpPeer::stateChanged, this, &UdpPeer::socketStateChangedHandler);
}

UdpPeer::~UdpPeer()
{
    th.quit();
    th.wait();

}

void UdpPeer::writeDatagram(QNetworkDatagram datagram)
{
    QUdpSocket::writeDatagram(datagram);
    emit dateLengthWritten(datagram.data().size());
}

void UdpPeer::listen(QHostAddress addr, quint16 port)
{
    if (state() != QAbstractSocket::UnconnectedState)
        close();
    bool ret = QUdpSocket::bind(addr, port);
    if (ret)
        emit UdpSocketStateChanged(UdpSocketState::Listening);
    else
    {
        emit deviceErrorOccurred(errorString());
    }

}

void UdpPeer::close()
{
    QUdpSocket::close();
    emit UdpSocketStateChanged(UdpSocketState::Closed);
}

void UdpPeer::readDatagram()
{
    QUdpSocket *socket = qobject_cast<QUdpSocket *>(sender());
    if (socket == 0)
        return;
    auto datagram = socket->receiveDatagram();
    if (datagram.isValid())
        emit datagramRead(datagram);
}

void UdpPeer::socketStateChangedHandler(SocketState state)
{
    emit localHostChanged(localAddress(), localPort());
}
