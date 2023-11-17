#include "tcpclientwidget.h"
#include "qabstractsocket.h"
#include "qnamespace.h"
#include "qtimer.h"
#include "ui_tcpclientwidget.h"
#include "utils.h"

TcpClientWidget::TcpClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClientWidget),
    socketState(QAbstractSocket::SocketState::UnconnectedState)
{
    ui->setupUi(this);
    connect(&tcpClient, &TcpClient::connected, this, &TcpClientWidget::connected);
    connect(&tcpClient, &TcpClient::disconnected, this, &TcpClientWidget::disconnected);
    connect(&tcpClient, &TcpClient::stateChanged, this, &TcpClientWidget::socketStateHandler);
    connect(&tcpClient, &TcpClient::errorOccurred, this, &TcpClientWidget::socketErrorHandler);
    connect(&tcpClient, &TcpClient::dataRead, this, &TcpClientWidget::receiveData);
}

TcpClientWidget::~TcpClientWidget()
{
    delete ui;
}

void TcpClientWidget::on_actionPushButton_clicked()
{
    QHostAddress address(ui->ipAddressLineEdit->text());
    quint16 port = ui->portSpinBox->value();
    if (socketState == QAbstractSocket::SocketState::UnconnectedState)
    {
        QTimer::singleShot(0, &tcpClient, [&, address, port]{tcpClient.connectToHost(address, port);});
    }
    else if (socketState == QAbstractSocket::SocketState::ConnectedState)
    {
        QTimer::singleShot(0, &tcpClient, &TcpClient::disconnectFromHost);
    }

}


void TcpClientWidget::on_sendPushButton_clicked()
{
    QString text = ui->sendTextEdit->toPlainText();
    if (text.isEmpty()) return;
    QByteArray data = text.toLocal8Bit();

    QTimer::singleShot(0, &tcpClient, [&, data]{tcpClient.write(data);});

    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGray));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                     " Sent data");
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkBlue));
    ui->conversationTextEdit->append(text);

}

void TcpClientWidget::connected()
{
    ui->actionPushButton->setText(tr("Disconnect"));
    ui->sendPushButton->setEnabled(true);
    ui->ipAddressLineEdit->setEnabled(false);
    ui->portSpinBox->setEnabled(false);

    QString server = ui->ipAddressLineEdit->text() + ":" + ui->portSpinBox->text();
    ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " Connected to server " + server);
}

void TcpClientWidget::disconnected()
{
    ui->actionPushButton->setText(tr("Connect"));
    ui->sendPushButton->setEnabled(false);
    ui->ipAddressLineEdit->setEnabled(true);
    ui->portSpinBox->setEnabled(true);

    QString server = ui->ipAddressLineEdit->text() + ":" + ui->portSpinBox->text();
    ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " Disconnected from server " + server);
}

void TcpClientWidget::socketStateHandler(QAbstractSocket::SocketState state)
{
    socketState = state;

}

void TcpClientWidget::socketErrorHandler(QAbstractSocket::SocketError error)
{
    // TODO
    auto errorString = QVariant::fromValue(error).toString();
    ui->conversationTextEdit->setTextColor(QColor(Qt::red));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " " + errorString);
}

void TcpClientWidget::receiveData(QByteArray data)
{
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGray));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                     " Received data");
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGreen));
    ui->conversationTextEdit->append(QString::fromLocal8Bit(data));
}


void TcpClientWidget::on_clearConversationPushButton_clicked()
{
    ui->conversationTextEdit->clear();
}


void TcpClientWidget::on_clearSendPushButton_clicked()
{
    ui->sendTextEdit->clear();
}

