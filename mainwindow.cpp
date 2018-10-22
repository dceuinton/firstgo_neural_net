#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalvariables.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QStringList>
#include <QByteArray>
#include <cstdlib>

QString relative_path = "/home/dale/Documents/university/ai/a2/LetterRecognition/";
std::map<int, char> dictionary = {
{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'},
{4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'},
{8, 'I'}, {9, 'J'}, {10, 'K'}, {11, 'L'},
{12, 'M'}, {13, 'N'}, {14, 'O'}, {15, 'P'},
{16, 'Q'}, {17, 'R'}, {18, 'S'}, {19, 'T'},
{20, 'U'}, {21, 'V'}, {22, 'W'}, {23, 'X'},
{24, 'Y'}, {25, 'Z'}, {26, 'u'}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// WARNING if I change the number of input neurons this won't work. Out of bounds error likely.
void MainWindow::on_btnReadFile_clicked()
{
    QString filename = ui->pteReadFile->toPlainText();
    printMessage("Reading file: " + filename + "\n");

    QFile datafile(relative_path + filename);
    datafile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!datafile.exists()) {
        qDebug() << filename << " does not exist";
        return;
    }

    QTextStream in(&datafile);

    for (int i = 0; i < NUMBER_OF_PATTERNS; i++) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        char sym = *(parts[0].toUtf8().data());

        letters[i].symbol = Symbol(sym - 65);
        for (int j = 0; j < INPUT_NEURONS; j++) {
            letters[i].f[j] = atoi(parts[j+1].toUtf8().data());
        }
    }

    QString size;
    QTextStream(&size) << "Size of letters is " << sizeof(letters);
    printMessage(size + "\n");

    printMessage("Loaded file " + filename + "\n");

    patternsLoadedFromFile = true;
    datafile.close();
}

void MainWindow::printMessage(QString message) {
    ui->pteMessagePanel->moveCursor(QTextCursor::End);
    ui->pteMessagePanel->insertPlainText(message);
}

void MainWindow::printLetter(int i) {
    printMessage(QString::fromUtf8(letters[i].toString().c_str()));
}

void MainWindow::on_btnSaveFile_clicked()
{
    QString filename = ui->pteSaveFile->toPlainText();
    printMessage("Saving to " + filename + "\n");
    QFile outputfile(relative_path + filename);
    outputfile.open(QIODevice::WriteOnly | QIODevice::Text);

    char buf[80];
    QByteArray temp;

    QTextStream out(&outputfile);

    for (int i = 0; i < NUMBER_OF_PATTERNS; i++) {
        ::sprintf(buf, "%c", dictionary[letters[i].symbol]);
        temp.append(buf);

        for (int j = 0; j < INPUT_NEURONS; j++) {
            ::sprintf(buf, ",%d", letters[i].f[j]);
            temp.append(buf);
        }
        temp.append("\n");
    }

    out << temp;
    outputfile.close();
    printMessage("Saved.\n");
}
