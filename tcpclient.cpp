#include "tcpclient.h"
#include "qobject.h"

TcpClient::TcpClient(QObject *parent)
    : QTcpSocket{parent}
{
    moveToThread(&th);
    th.start();
    connect(&th, &QThread::finished, this, &QTcpSocket::close);
    connect(this, &TcpClient::readyRead, this, &TcpClient::readSocket);
}

TcpClient::~TcpClient()
{
    th.quit();
    th.wait();
}

void TcpClient::readSocket()
{
    QByteArray data = readAll();
    emit dataRead(data);

}
