#ifndef UDPPEER_H
#define UDPPEER_H

#include "qabstractsocket.h"
#include "qhostaddress.h"
#include "qthread.h"
#include "qtypes.h"
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpPeer : public QUdpSocket
{
    Q_OBJECT
public:
    enum UdpSocketState
    {
        Closed,
        Listening
    };
    explicit UdpPeer(QObject *parent = nullptr);
    ~UdpPeer();
public slots:
    void writeDatagram(QNetworkDatagram datagram) ;
    void listen(QHostAddress addr, quint16 port);
    void close() override;
private slots:
    void readDatagram();
    void socketStateChangedHandler(QAbstractSocket::SocketState state);
signals:
    void localHostChanged(QHostAddress, quint16);
    void deviceErrorOccurred(QString);
    void UdpSocketStateChanged(UdpSocketState);
    void datagramRead(QNetworkDatagram);
    void dateLengthWritten(qint64);
private:
    QThread th;
};

#endif // UDPPEER_H
