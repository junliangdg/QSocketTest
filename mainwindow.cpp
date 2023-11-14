#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qnamespace.h"
#include "qtmetamacros.h"
#include "tcpserver.h"
#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,tcpServerState(TcpServer::ServerState::Closed)
{
    ui->setupUi(this);

    connect(this, &MainWindow::listenTcpServer, &tcpServer, &TcpServer::listenServer);
    connect(this, &MainWindow::closeTcpServer, &tcpServer, &TcpServer::closeServer);
    connect(&tcpServer, &TcpServer::peerListUpdated, this, &MainWindow::updateTcpServerConnectionList);
    connect(&tcpServer, &TcpServer::serverStateChanged, this, &MainWindow::updateTcpServerState);
}

MainWindow::~MainWindow()
{
    emit closeTcpServer();

    delete ui;
}

void MainWindow::updateTcpServerConnectionList(QVector<QString> list)
{
    tcpServerConnectionList = list;
    ui->comboBox_target->clear();

    ui->comboBox_target->addItem(QString("All(%1)").arg(list.size()));
    for (auto &connection : list)
    {
        ui->comboBox_target->addItem(connection);
    }
}

void MainWindow:: updateTcpServerState(TcpServer::ServerState state)
{
    tcpServerState = state;
    if (ui->comboBox_protocolType->currentIndex() == SocketProtocolType::TCPServer)
    {
        switch (state) {
        case TcpServer::ServerState::Closed:
            ui->pushButton_connect->setText(tr("Listen"));
            break;
        case TcpServer::ServerState::Listening:
            ui->pushButton_connect->setText(tr("Close"));
            break;
        default:
            break;
        }
    }
}


void MainWindow::on_comboBox_protocolType_activated(int index)
{
    switch(index)
    {
    case 0:
        ui->pushButton_connect->setText(tr("Listen"));
        break;
    case 1:
        ui->pushButton_connect->setText(tr("Connect"));
        break;
    case 2:
        ui->pushButton_connect->setText(tr("Listen"));
        break;
    default:
        break;
    }

}


void MainWindow::on_pushButton_connect_clicked()
{
    SocketProtocolType protocolType = (SocketProtocolType)ui->comboBox_protocolType->currentIndex();

    QHostAddress address = QHostAddress(ui->lineEdit_ipAddress->text());
    quint16 port = ui->spinBox_port->value();
    switch (protocolType) {
    case SocketProtocolType::TCPServer:
    {
        if (tcpServerState == TcpServer::ServerState::Closed)
            emit listenTcpServer(address, port);
        else if (tcpServerState == TcpServer::ServerState::Listening)
            emit closeTcpServer();
    }
    break;
    default:
        break;
    }

}

