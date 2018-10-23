#include "mainwindow.h"
#include <QApplication>
#include "globalvariables.h"



LetterStructure letters[20001];
LetterStructure testPattern;

bool patternsLoadedFromFile;
int MAX_EPOCHS = 100;
double LEARNING_RATE = 0.1;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    patternsLoadedFromFile = false;

    MainWindow w;
    w.show();

    return a.exec();
}
