#ifndef TCPCLIENTWIDGET_H
#define TCPCLIENTWIDGET_H

#include "tcpclient.h"
#include <QWidget>

namespace Ui {
class TcpClientWidget;
}

class TcpClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClientWidget(QWidget *parent = nullptr);
    ~TcpClientWidget();

private slots:
    void on_actionPushButton_clicked();

    void on_sendPushButton_clicked();

    void connected();

    void disconnected();

    void socketStateHandler(QAbstractSocket::SocketState state);

    void socketErrorHandler(QAbstractSocket::SocketError error);

    void receiveData(QByteArray data);

    void on_clearConversationPushButton_clicked();

    void on_clearSendPushButton_clicked();

private:
    QAbstractSocket::SocketState socketState;
    TcpClient tcpClient;
    Ui::TcpClientWidget *ui;
};

#endif // TCPCLIENTWIDGET_H
