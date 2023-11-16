#include "tcpclientwidget.h"
#include "ui_tcpclientwidget.h"

TcpClientWidget::TcpClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClientWidget)
{
    ui->setupUi(this);
}

TcpClientWidget::~TcpClientWidget()
{
    delete ui;
}
