#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "tcpserverwidget.h"
#include "tcpclientwidget.h"
#include "udppeerwidget.h"
#include <QMdiSubWindow>
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



void MainWindow::on_actionTcpServer_triggered()
{
    static unsigned int tcpServerWidgetNo = 1;
    TcpServerWidget *widget = new TcpServerWidget;
    widget->setWindowTitle(widget->windowTitle() + QString::number(tcpServerWidgetNo++));
    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWidget(widget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->show();
}


void MainWindow::on_actionTcpClient_triggered()
{
    static unsigned int tcpClientWidgetNo = 1;
    TcpClientWidget *widget = new TcpClientWidget;
    widget->setWindowTitle(widget->windowTitle() + QString::number(tcpClientWidgetNo++));
    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWidget(widget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->show();
}

void MainWindow::on_actionUdpPeer_triggered()
{
    static unsigned int udpPeerWidgetNo = 1;
    UdpPeerWidget *widget = new UdpPeerWidget;
    widget->setWindowTitle(widget->windowTitle() + QString::number(udpPeerWidgetNo++));
    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWidget(widget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->mdiArea->addSubWindow(subWindow);
    subWindow->show();
}

