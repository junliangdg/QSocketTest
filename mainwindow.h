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

private slots:
    void on_comboBox_protocolType_activated(int index);

    void on_pushButton_connect_clicked();

private:
    TcpServer tcpServer;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
