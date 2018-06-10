#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_tableNameCombo_currentIndexChanged(const QString &tableName);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
