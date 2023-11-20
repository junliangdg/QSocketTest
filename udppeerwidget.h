#ifndef UDPPEERWIDGET_H
#define UDPPEERWIDGET_H

#include "qhostaddress.h"
#include "qnetworkdatagram.h"
#include "udppeer.h"
#include <QWidget>

namespace Ui {
class UdpPeerWidget;
}

class UdpPeerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UdpPeerWidget(QWidget *parent = nullptr);
    ~UdpPeerWidget();

private slots:
    void on_actionPushButton_clicked();

    void deviceErrorHandler(QString error);

    void updateSocketState(UdpPeer::UdpSocketState state);

    void updateLocalHost(QHostAddress addr, quint16 port);

    void receiveDatagram(QNetworkDatagram datagram);

    void on_sendPushButton_clicked();

    void on_remoteHostComboBox_editingFinished();

    void on_clearConversationPushButton_clicked();

    void on_clearHostPushButton_clicked();

    void on_clearSendPushButton_clicked();

private:
    UdpPeer::UdpSocketState udpSocketState;
    UdpPeer udpPeer;
    Ui::UdpPeerWidget *ui;
};

#endif // UDPPEERWIDGET_H
