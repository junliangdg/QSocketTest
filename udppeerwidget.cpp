#include "udppeerwidget.h"
#include "qcombobox.h"
#include "qhostaddress.h"
#include "qnamespace.h"
#include "qnetworkdatagram.h"
#include "qtimer.h"
#include "udppeer.h"
#include "ui_udppeerwidget.h"
#include "utils.h"
#include <QMessageBox>
#include <functional>

UdpPeerWidget::UdpPeerWidget(QWidget *parent) :
    QWidget(parent),
    udpSocketState(UdpPeer::UdpSocketState::Closed),
    ui(new Ui::UdpPeerWidget)
{
    ui->setupUi(this);

    connect(&udpPeer, &UdpPeer::UdpSocketStateChanged, this, &UdpPeerWidget::updateSocketState);
    connect(&udpPeer, &UdpPeer::datagramRead, this, &UdpPeerWidget::receiveDatagram);
    connect(&udpPeer, &UdpPeer::localHostChanged, this, &UdpPeerWidget::updateLocalHost);
    connect(&udpPeer, &UdpPeer::deviceErrorOccurred, this, &UdpPeerWidget::deviceErrorHandler);

    connect(ui->remoteHostComboBox->lineEdit(), &QLineEdit::editingFinished, this, &UdpPeerWidget::on_remoteHostComboBox_editingFinished);

}

UdpPeerWidget::~UdpPeerWidget()
{
    delete ui;
}

void UdpPeerWidget::on_actionPushButton_clicked()
{
    QHostAddress address = QHostAddress(ui->ipAddressLineEdit->text());
    quint16 port = ui->portSpinBox->value();
    if (udpSocketState == UdpPeer::UdpSocketState::Closed)
        QTimer::singleShot(0, &udpPeer, [&, address, port]{udpPeer.listen(address, port);});
    else if (udpSocketState == UdpPeer::UdpSocketState::Listening)
        QTimer::singleShot(0, &udpPeer, [&]{udpPeer.close();});
}

void UdpPeerWidget::deviceErrorHandler(QString error)
{
    ui->conversationTextEdit->setTextColor(QColor(Qt::red));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " " + error);
}

void UdpPeerWidget::updateSocketState(UdpPeer::UdpSocketState state)
{
    udpSocketState = state;

    switch (state) {
    case UdpPeer::UdpSocketState::Closed:
        ui->portSpinBox->setEnabled(true);
        ui->ipAddressLineEdit->setEnabled(true);
        ui->actionPushButton->setText(tr("Start"));
        ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
        ui->conversationTextEdit->append(Utils::getCurrentTimeString()+ tr(" Stop listening"));
        break;
    case UdpPeer::UdpSocketState::Listening:
        ui->portSpinBox->setEnabled(false);
        ui->ipAddressLineEdit->setEnabled(false);
        ui->actionPushButton->setText(tr("Stop"));
        ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
        ui->conversationTextEdit->append(Utils::getCurrentTimeString() + tr(" Start listening on ") +
                                         ui->ipAddressLineEdit->text() + ":" + ui->portSpinBox->text());
        break;
    default:
        break;
    }
}

void UdpPeerWidget::updateLocalHost(QHostAddress addr, quint16 port)
{
    QString host = addr.toString() + ":" + QString::number(port);
    ui->localHostLabel->setText(host);
}

void UdpPeerWidget::receiveDatagram(QNetworkDatagram datagram)
{
    // ipv4 address only
    QHostAddress ip4Address(datagram.senderAddress().toIPv4Address());
    QString peer = ip4Address.toString() + ":" + QString::number(datagram.senderPort());
    auto index = ui->remoteHostComboBox->findText(peer);
    if (index == -1)
        ui->remoteHostComboBox->addItem(peer);

    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGray));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                     " Received data from " + peer);
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGreen));
    ui->conversationTextEdit->append(QString::fromLocal8Bit(datagram.data()));
}


void UdpPeerWidget::on_sendPushButton_clicked()
{
    auto peer = ui->remoteHostComboBox->currentText();
    auto text = ui->sendTextEdit->toPlainText();
    if (peer.isEmpty() || text.isEmpty())
        return;
    auto val = text.toLocal8Bit();

    auto list = peer.split(":", Qt::SkipEmptyParts);
    QTimer::singleShot(0, &udpPeer, [=]{
        QNetworkDatagram data;
        data.setData(val);
        data.setDestination(QHostAddress(list[0]), list[1].toUInt());
        udpPeer.writeDatagram(data);
    });

    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGray));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                     " Sent data to " + peer);
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkBlue));
    ui->conversationTextEdit->append(text);
}


void UdpPeerWidget::on_remoteHostComboBox_editingFinished()
{
    QString text = ui->remoteHostComboBox->currentText();
    auto list = text.split(":", Qt::SkipEmptyParts);
    if (list.size() == 2)
    {
        QHostAddress ip(list[0]);
        bool isInt;
        list[1].toUInt(&isInt);
        if (isInt && ip.protocol() != QAbstractSocket::UnknownNetworkLayerProtocol)
        {
            auto index = ui->remoteHostComboBox->findText(text);
            if (index == -1)
                ui->remoteHostComboBox->addItem(text);
            else
                ui->remoteHostComboBox->setCurrentIndex(index);
            return;
        }
    }
    ui->remoteHostComboBox->setCurrentText("");
    ui->remoteHostComboBox->setCurrentIndex(0);
    QMessageBox::warning(this, tr("Validation"), tr("Invalid remote host"));
}


void UdpPeerWidget::on_clearConversationPushButton_clicked()
{
    ui->conversationTextEdit->clear();
}


void UdpPeerWidget::on_clearHostPushButton_clicked()
{
    ui->remoteHostComboBox->clear();
}


void UdpPeerWidget::on_clearSendPushButton_clicked()
{
    ui->sendTextEdit->clear();
}

