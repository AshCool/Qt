#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSettings"
#include "QMessageBox"
#include "globals.h"
#include "QSqlQuery"
#include "QSqlQueryModel"

/*	Разработать графическое приложение, которое выполняет подключение к БД и
 * выполнив запрос на выборку всех сданных из таблицы, отображает их в приложении в табличном виде.
 * Настройки подключения к БД выбираются в зависимости от варианта.
0. Настройки подключения к БД хранятся в файле формата *.ini для работы с которым используется класс QSettings
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("ACProducts", "hw2");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    QSettings cs("connect_settings.ini", QSettings::IniFormat);
    /*cs.setValue("connection/HostName", "localhost");
    cs.setValue("connection/DatabaseName", "cursedb");
    cs.setValue("connection/UserName", "root");
    cs.setValue("connection/Password", "1234");*/

    db = QSqlDatabase::addDatabase("QMYSQL", "cursedb");
    db.setHostName(cs.value("connection/HostName").toString());
    db.setDatabaseName(cs.value("connection/DatabaseName").toString());
    db.setUserName(cs.value("connection/UserName").toString());
    db.setPassword(cs.value("connection/Password").toString());
    bool ok = db.open();

    if (ok)
    {
        QStringList tables = db.tables(QSql::Tables);
        ui->tableNameCombo->insertItems(0, tables);
    }
    else
    {
        QMessageBox msg;
        msg.setText("Ошибка подключения к БД");
        msg.setIcon(QMessageBox::Critical);
        msg.setStandardButtons(QMessageBox::Ok);
        int ret = msg.exec();
        switch (ret) {
        case QMessageBox::Ok:
            exit(0);
        default:
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("ACProducts", "hw2");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::on_tableNameCombo_currentIndexChanged(const QString &tableName)
{
    QSqlQuery query(db);
    if (query.prepare("select * from " + tableName))
    {
        if (query.exec())
        {
            QSqlQueryModel *model = new QSqlQueryModel(this);
            model->setQuery(query);
            ui->tableView->setModel(model);
            ui->tableView->resizeColumnsToContents();
            //ui->tableView->reset();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Execution error", QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Query preparation error", QMessageBox::Ok);
    }
}
