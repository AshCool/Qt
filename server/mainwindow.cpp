#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QHostAddress"
#include "QMessageBox"
#include "QString"

//0. Разработать систему оповещения, отправляющее данные в текстовом виде на все клиенты.
//Сервер приложения должен по протоколу UDP отправить сообщение на все клиенты,
//клиент при получении сообщения должен вывести полученное сообщение.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    QByteArray datagram = (ui->messageEdit->text()).toUtf8();
    socket->writeDatagram(datagram, QHostAddress::Broadcast, 55555);
    QMessageBox::information(this, "Сообщение отправленно",  "Отправлено сообщение: " + ui->messageEdit->text());
}
