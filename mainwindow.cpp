#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    int protocolType = ui->comboBox_protocolType->currentIndex();
    QHostAddress address = QHostAddress(ui->lineEdit_ipAddress->text());
    quint16 port = ui->spinBox_port->value();
    switch (protocolType) {
    case 0:
    {
        tcpServer.listen(address, port);
    }
    break;
    default:
        break;
    }

}

