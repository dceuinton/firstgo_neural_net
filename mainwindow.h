#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "globalvariables.h"
#include "backpropagation.h"

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QStringList>
#include <QByteArray>
#include <cstdlib>
#include <algorithm>

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
    LetterStructure loadLetterFromString(QString line);
    void displayTestResults(double* results);

private slots:
    void on_btnReadFile_clicked();

    void on_btnSaveFile_clicked();

    void on_sbMaxEpochs_valueChanged(int arg1);

    void on_hsLearningRate_sliderMoved(int position);

    void on_btnSaveWeights_clicked();

    void on_btnClearLog_clicked();

    void on_btnTrainNetwork_clicked();

    void on_btnTestCustomData_clicked();

    void on_btnTestNetwork_clicked();

    void on_btnInitialiseNetwork_clicked();

    void on_btnShuffleTrainingData_clicked();

    void on_btnLoadWeights_clicked();

    void on_btnSaveRelativePath_clicked();

private:
    Ui::MainWindow *ui;
    Backpropagation *bp;
    QLCDNumber* displays[OUTPUT_NEURONS];
};

#endif // MAINWINDOW_H
