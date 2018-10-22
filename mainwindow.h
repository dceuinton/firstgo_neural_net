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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void printMessage(QString message);
    void printLetter(int i);

private slots:
    void on_btnReadFile_clicked();

    void on_btnSaveFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
