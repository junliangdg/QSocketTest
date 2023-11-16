#ifndef TCPCLIENTWIDGET_H
#define TCPCLIENTWIDGET_H

#include <QWidget>

namespace Ui {
class TcpClientWidget;
}

class TcpClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClientWidget(QWidget *parent = nullptr);
    ~TcpClientWidget();

private:
    Ui::TcpClientWidget *ui;
};

#endif // TCPCLIENTWIDGET_H
