#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QCloseEvent"
#include <QFileDialog>
#include "QIODevice"
#include "QStandardItemModel"
#include "QStandardItem"

//Разработать графическое приложение, которое выполняет запись/чтение структурированной информации
//в/из локальный файл на компьютер. В качестве структурированной информации использовать данные
//некой сущности БД (из задания по курсовому проекту БД). Приложение должно выводить список всей
//информации в табличном виде и иметь функции добавить новую запись, удалить и изменить.
//При работе со  списками структурированной информацией использовать контейнеры и файл.
//4. При работе со  списками структурированной информацией использовать
//контейнер QHash и сохранять всю информацию в бинарный файл

inline QDataStream &operator <<(QDataStream &stream, const TextDataBase &tdb)
{
    stream << tdb.course;
    stream << tdb.language;
    return stream;
}

inline QDataStream &operator >>(QDataStream &stream, TextDataBase &tdb)
{
    stream >> tdb.course;
    stream >> tdb.language;
    return stream;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createAction = new QAction(tr("&Создать"), this);
    connect(createAction, SIGNAL(triggered(bool)), this, SLOT(create()));
    ui->fileMenu->addAction(createAction);
    openAction = new QAction(tr("&Открыть..."), this);
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(open()));
    ui->fileMenu->addAction(openAction);
    saveAction = new QAction(tr("&Сохранить"), this);
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(save()));
    ui->fileMenu->addAction(saveAction);
    saveAsAction = new QAction(tr("&Сохранить как..."), this);
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    ui->fileMenu->addAction(saveAsAction);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*fileContents.insert(0, TextDataBase(0, "a"));
    fileContents.insert(1, TextDataBase(0, "b"));
    fileContents.insert(2, TextDataBase(0, "c"));*/

    refreshTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create()
{
    QMessageBox msg;
    msg.setText("Сохранить файл?");
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int ret = msg.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        saveAs();
        break;
    case QMessageBox::No:
        break;
    case QMessageBox::Cancel:
        return;
    default:
        return;
    }

    fileName = "";
    fileContents.clear();

    refreshTable();
}

void MainWindow::open()
{
    QMessageBox msg;
    msg.setText("Сохранить файл?");
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int ret = msg.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        saveAs();
        break;
    case QMessageBox::No:
        break;
    case QMessageBox::Cancel:
        return;
    default:
        return;
    }

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("hw1 DB files (*.hw1db)"));
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec())
    {
        fileName = dialog.selectedFiles()[0];
    }

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_10);
        stream >> fileContents;
        if (stream.status() != QDataStream::Ok)
        {
            qDebug("Ошибка чтения файла, open()");
        }
    }
    refreshTable();
}

void MainWindow::save()
{
    if( !fileName.isNull() )
    {
      QFile file(fileName);
      if (file.open(QIODevice::WriteOnly))
      {
          QDataStream stream(&file);

          stream.setVersion(QDataStream::Qt_5_10);
          stream << fileContents;
          file.flush();

          if (stream.status() != QDataStream::Ok)
          {
              qDebug("Ошибка записи, save()");
          }
      }
      file.close();
    }
    else
    {
        saveAs();
    }
}

void MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save Document"),
            QDir::currentPath(),
            tr("hw1 DB files (*.hw1db)") );
        if( !filename.isNull() )
        {
          QFile file(filename);
          if (file.open(QIODevice::WriteOnly))
          {
              QDataStream stream(&file);

              stream.setVersion(QDataStream::Qt_5_10);
              stream << fileContents;
              file.flush();

              if (stream.status() != QDataStream::Ok)
              {
                  qDebug("Ошибка записи, saveAs()");
              }
          }
          file.close();
        }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msg;
    msg.setText("Сохранить файл?");
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int ret = msg.exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        saveAs();
        event->accept();
        break;
    case QMessageBox::No:
        event->accept();
        break;
    case QMessageBox::Cancel:
        event->ignore();
        break;
    default:
        event->ignore();
        break;
    }
}

void MainWindow::on_addButton_clicked()
{
    int course = (ui->courseEdit->text()).toInt();
    QString language = ui->languageEdit->text();
    // i'm sorry
    if (fileContents.keys().isEmpty())
    {
       fileContents.insert(0, TextDataBase(course, language));
       refreshTable();
       return;
    }

    QList<int> tmp = fileContents.keys();
    std::sort(tmp.begin(), tmp.end());
    int id = tmp.last() + 1;

    fileContents.insert(id, TextDataBase(course, language));

    refreshTable();
}

void MainWindow::on_updateButton_clicked()
{
    fileContents.remove(recordId);
    int course = ui->courseEdit->text().toInt();
    QString language = ui->languageEdit->text();
    fileContents.insert(recordId, TextDataBase(course, language));

    refreshTable();
}

void MainWindow::on_deleteButton_clicked()
{
    fileContents.remove(recordId);

    refreshTable();
}

void MainWindow::refreshTable()
{
    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;
    QStringList headers;
    headers << "id" << "course" << "language";
    model->setHorizontalHeaderLabels(headers);
    int i = 0;
    foreach (int k, fileContents.keys()) {

        item = new QStandardItem(QString::number(k));
        model->setItem(i, 0, item);

        item = new QStandardItem(QString::number(fileContents.value(k).course));
        model->setItem(i, 1, item);

        item = new QStandardItem(fileContents.value(k).language);
        model->setItem(i, 2, item);

        i++;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

TextDataBase::TextDataBase(int _course, QString _language)
{
    course = _course;
    language = _language;
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndexList record = ui->tableView->selectionModel()->selectedRows();
    recordId = ui->tableView->model()->itemData(record.first()).first().toInt();
    ui->idEdit->setText(QString::number(recordId));
    ui->courseEdit->setText(QString::number(fileContents.value(recordId).course));
    ui->languageEdit->setText(fileContents.value(recordId).language);
}
