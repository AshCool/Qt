#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QUdpSocket(this);
    socket->bind(55555, QUdpSocket::ShareAddress);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processDatagram()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processDatagram()
{
    QByteArray datagram;
    //delete ui->messageList->takeItem(0);
    while (socket->hasPendingDatagrams())
    {
        datagram.resize(int(socket->pendingDatagramSize()));
        socket->readDatagram(datagram.data(), datagram.size());
        ui->messageList->addItem(tr("Получено сообщение: %1").arg(datagram.constData()));
    }
}
