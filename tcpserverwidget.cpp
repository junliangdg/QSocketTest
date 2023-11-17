#include "tcpserverwidget.h"
#include "qnamespace.h"
#include "qtimer.h"
#include "ui_tcpserverwidget.h"
#include "utils.h"

TcpServerWidget::TcpServerWidget(QWidget *parent) :
    QWidget(parent),
    paused(false),
    serverState(TcpServer::ServerState::Closed),
    ui(new Ui::TcpServerWidget)
{
    ui->setupUi(this);

    connect(&tcpServer, &TcpServer::peerListUpdated, this, &TcpServerWidget::updateConnectionList);
    connect(&tcpServer, &TcpServer::serverStateChanged, this, &TcpServerWidget::updateServerState);
    connect(&tcpServer, &TcpServer::dataRead, this, &TcpServerWidget::appendReceivedData);
    connect(&tcpServer, &TcpServer::clientConnected, this, &TcpServerWidget::newClientConnectedIn);
    connect(&tcpServer, &TcpServer::clientDisconnected, this, &TcpServerWidget::clientDisconnected);
    connect(&tcpServer, &TcpServer::errorOccurred, this, &TcpServerWidget::serverErrorHandler);
}

TcpServerWidget::~TcpServerWidget()
{
    delete ui;
}

void TcpServerWidget::appendReceivedData(QString client, QByteArray data)
{
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGray));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                     " Received data from " + client);
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGreen));
    ui->conversationTextEdit->append(QString::fromLocal8Bit(data));
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
        ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
        ui->conversationTextEdit->append(Utils::getCurrentTimeString()+ tr(" Stop listening on ") +
                                         ui->ipAddressLineEdit->text() + ":" + ui->portSpinBox->text());
        break;
    case TcpServer::ServerState::Listening:
        ui->portSpinBox->setEnabled(false);
        ui->ipAddressLineEdit->setEnabled(false);
        ui->sendPushButton->setEnabled(true);
        ui->pausePushButton->setEnabled(true);
        ui->actionPushButton->setText(tr("Stop"));
        ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
        ui->conversationTextEdit->append(Utils::getCurrentTimeString() + tr(" Start listening on ") +
                                         ui->ipAddressLineEdit->text() + ":" + ui->portSpinBox->text());
        break;
    default:
        break;
    }

}

void TcpServerWidget::newClientConnectedIn(QString client)
{
    ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " " + client + " Connected");
}

void TcpServerWidget::clientDisconnected(QString client)
{
    ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " " + client + " Disconnected");
}

void TcpServerWidget::serverErrorHandler(QString error)
{
    ui->conversationTextEdit->setTextColor(QColor(Qt::red));
    ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " " + error);
}

void TcpServerWidget::on_actionPushButton_clicked()
{
    QHostAddress address = QHostAddress(ui->ipAddressLineEdit->text());
    quint16 port = ui->portSpinBox->value();
    if (serverState == TcpServer::ServerState::Closed)
        QTimer::singleShot(0, &tcpServer, [&, address, port]{tcpServer.listenServer(address, port);});
    else if (serverState == TcpServer::ServerState::Listening)
        QTimer::singleShot(0, &tcpServer, [&]{tcpServer.closeServer();});
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
    QString text = ui->sendTextEdit->toPlainText();
    if (text.isEmpty()) return;
    QByteArray data = text.toLocal8Bit();
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkGray));
    if(ui->clientListComboBox->currentIndex() == 0)
    {
        for (auto &client : connectionList)
        {
            tcpServer.writeData(client , data);
            ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                             " Sent data to " + ui->clientListComboBox->currentText());
        }
    }
    else
    {
        tcpServer.writeData(curConnection , data);
        ui->conversationTextEdit->append(Utils::getCurrentTimeString() +
                                         " Sent data to " + curConnection);
    }
    ui->conversationTextEdit->setTextColor(QColor(Qt::darkBlue));
    ui->conversationTextEdit->append(text);

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
        ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
        ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " Resume accepting new connections");
    }
    else
    {
        QTimer::singleShot(0, &tcpServer, [&]{tcpServer.pauseAccepting();});
        ui->pausePushButton->setText(tr("Resume"));
        paused = true;
        ui->conversationTextEdit->setTextColor(QColor(Qt::blue));
        ui->conversationTextEdit->append(Utils::getCurrentTimeString() + " Pause accepting new connections");
    }

}

