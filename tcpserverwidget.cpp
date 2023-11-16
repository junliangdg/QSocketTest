#include "tcpserverwidget.h"
#include "qtimer.h"
#include "ui_tcpserverwidget.h"

TcpServerWidget::TcpServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServerWidget),
    serverState(TcpServer::ServerState::Closed),
    paused(false)
{
    ui->setupUi(this);

    connect(this, &TcpServerWidget::listenTcpServer, &tcpServer, &TcpServer::listenServer);
    connect(this, &TcpServerWidget::closeTcpServer, &tcpServer, &TcpServer::closeServer);
    connect(&tcpServer, &TcpServer::peerListUpdated, this, &TcpServerWidget::updateConnectionList);
    connect(&tcpServer, &TcpServer::serverStateChanged, this, &TcpServerWidget::updateServerState);

    connect(&tcpServer, &TcpServer::dataRead, this, &TcpServerWidget::appendReceivedData);
}

TcpServerWidget::~TcpServerWidget()
{
    delete ui;
}

void TcpServerWidget::appendReceivedData(QString client, QByteArray data)
{
    ui->conversationTextEdit->append(client + "\n" + data);
}

void TcpServerWidget::updateConnectionList(QVector<QString> list)
{
    if (list.size())
        ui->disconnectPushButton->setEnabled(true);
    else
        ui->disconnectPushButton->setEnabled(false);
    connectionList = list;
    ui->clientListComboBox->clear();

    ui->clientListComboBox->addItem(QString("All(%1)").arg(list.size()));
    for (auto &connection : list)
    {
        ui->clientListComboBox->addItem(connection);
    }

    int index = ui->clientListComboBox->findText(curConnection);
    qDebug() << index;
    if (index == -1)
        ui->clientListComboBox->setCurrentIndex(0);
    else
        ui->clientListComboBox->setCurrentIndex(index);
}

void TcpServerWidget:: updateServerState(TcpServer::ServerState state)
{
    serverState = state;

    switch (state) {
    case TcpServer::ServerState::Closed:
        ui->portSpinBox->setEnabled(true);
        ui->ipAddressLineEdit->setEnabled(true);
        ui->sendPushButton->setEnabled(false);
        ui->pausePushButton->setEnabled(false);
        ui->actionPushButton->setText(tr("Start"));
        break;
    case TcpServer::ServerState::Listening:
        ui->portSpinBox->setEnabled(false);
        ui->ipAddressLineEdit->setEnabled(false);
        ui->sendPushButton->setEnabled(true);
        ui->pausePushButton->setEnabled(true);
        ui->actionPushButton->setText(tr("Stop"));
        break;
    default:
        break;
    }

}

void TcpServerWidget::on_actionPushButton_clicked()
{
    QHostAddress address = QHostAddress(ui->ipAddressLineEdit->text());
    quint16 port = ui->portSpinBox->value();
    if (serverState == TcpServer::ServerState::Closed)
        emit listenTcpServer(address, port);
    else if (serverState == TcpServer::ServerState::Listening)
        emit closeTcpServer();
}


void TcpServerWidget::on_clearConversationPushButton_clicked()
{
    ui->conversationTextEdit->clear();
}


void TcpServerWidget::on_clearSendPushButton_clicked()
{
    ui->sendTextEdit->clear();
}


void TcpServerWidget::on_clientListComboBox_activated(int index)
{
    curConnection = ui->clientListComboBox->currentText();
}


void TcpServerWidget::on_sendPushButton_clicked()
{
    QByteArray data = ui->sendTextEdit->toPlainText().toLocal8Bit();
    if(ui->clientListComboBox->currentIndex() == 0)
    {
        for (auto &client : connectionList)
        {
            tcpServer.writeData(client , data);
        }
    }
    else
        tcpServer.writeData(curConnection , data);

}


void TcpServerWidget::on_disconnectPushButton_clicked()
{
    if(ui->clientListComboBox->currentIndex() == 0)
    {
        for (auto &client : connectionList)
        {
            tcpServer.disconnectClient(client);
        }
    }
    else
        tcpServer.disconnectClient(curConnection);
}


void TcpServerWidget::on_pausePushButton_clicked()
{
    if(paused)
    {
        QTimer::singleShot(0, &tcpServer, [&]{tcpServer.resumeAccepting();});
        ui->pausePushButton->setText(tr("Pause"));
        paused = false;
    }
    else
    {
        QTimer::singleShot(0, &tcpServer, [&]{tcpServer.pauseAccepting();});
        ui->pausePushButton->setText(tr("Resume"));
        paused = true;
    }

}

