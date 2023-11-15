#include "tcpserverwidget.h"
#include "ui_tcpserverwidget.h"

TcpServerWidget::TcpServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServerWidget),
    serverState(TcpServer::ServerState::Closed)
{
    ui->setupUi(this);

    connect(this, &TcpServerWidget::listenTcpServer, &tcpServer, &TcpServer::listenServer);
    connect(this, &TcpServerWidget::closeTcpServer, &tcpServer, &TcpServer::closeServer);
    connect(&tcpServer, &TcpServer::peerListUpdated, this, &TcpServerWidget::updateConnectionList);
    connect(&tcpServer, &TcpServer::serverStateChanged, this, &TcpServerWidget::updateServerState);
}

TcpServerWidget::~TcpServerWidget()
{
    delete ui;
}

void TcpServerWidget::updateConnectionList(QVector<QString> list)
{
    connectionList = list;
    ui->clientListComboBox->clear();

    ui->clientListComboBox->addItem(QString("All(%1)").arg(list.size()));
    for (auto &connection : list)
    {
        ui->clientListComboBox->addItem(connection);
    }
}

void TcpServerWidget:: updateServerState(TcpServer::ServerState state)
{
    serverState = state;

    switch (state) {
    case TcpServer::ServerState::Closed:
        ui->portSpinBox->setEnabled(true);
        ui->ipAddressLineEdit->setEnabled(true);
        ui->sendPushButton->setEnabled(false);
        ui->actionPushButton->setText(tr("Start"));
        break;
    case TcpServer::ServerState::Listening:
        ui->portSpinBox->setEnabled(false);
        ui->ipAddressLineEdit->setEnabled(false);
        ui->sendPushButton->setEnabled(true);
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

