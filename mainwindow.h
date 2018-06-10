#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QHash"
#include "QString"
#include "QDataStream"

namespace Ui {
class MainWindow;
class TextDataBase;
}

class TextDataBase
{
public:
    int course;
    QString language;
    TextDataBase(int _course = 0, QString _language = "None");
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void refreshTable();

    friend QDataStream &operator <<(QDataStream &stream, const TextDataBase &tdb);
    friend QDataStream &operator >>(QDataStream &stream, TextDataBase &tdb);

private slots:
    void create();
    void open();
    void save();
    void saveAs();

    void on_addButton_clicked();
    void on_updateButton_clicked();
    void on_deleteButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QAction *createAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QString fileName;
    QHash<int, TextDataBase> fileContents;
    int recordId;
};

#endif // MAINWINDOW_H
