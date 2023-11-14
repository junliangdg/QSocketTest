#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum SocketProtocolType
    {
        TCPServer,
        TCPClient,
        UDP
    };

private slots:
    void updateTcpServerConnectionList(QVector<QString> list);
    void updateTcpServerState(TcpServer::ServerState state);
    void on_comboBox_protocolType_activated(int index);

    void on_pushButton_connect_clicked();
signals:
    void listenTcpServer(QHostAddress, quint16);
    void closeTcpServer();
private:
    // tcp server
    QVector<QString> tcpServerConnectionList;
    TcpServer tcpServer;
    TcpServer::ServerState tcpServerState;

    QThread th;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
