#include "mainwindow.h"

QString relative_path = "/home/dale/Documents/ai/a2/LetterRecognition/";
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
    bp = new Backpropagation();
    MAX_EPOCHS = 5;
    LEARNING_RATE = 0.1;

    ui->lcdnLearningRate->display(LEARNING_RATE);

    displays[0] = ui->lcdnA;
    displays[1] = ui->lcdnB;
    displays[2] = ui->lcdnC;
    displays[3] = ui->lcdnD;
    displays[4] = ui->lcdnE;
    displays[5] = ui->lcdnF;
    displays[6] = ui->lcdnG;
    displays[7] = ui->lcdnH;
    displays[8] = ui->lcdnI;
    displays[9] = ui->lcdnJ;
    displays[10] = ui->lcdnK;
    displays[11] = ui->lcdnL;
    displays[12] = ui->lcdnM;
    displays[13] = ui->lcdnN;
    displays[14] = ui->lcdnO;
    displays[15] = ui->lcdnP;
    displays[16] = ui->lcdnQ;
    displays[17] = ui->lcdnR;
    displays[18] = ui->lcdnS;
    displays[19] = ui->lcdnT;
    displays[20] = ui->lcdnU;
    displays[21] = ui->lcdnV;
    displays[22] = ui->lcdnW;
    displays[23] = ui->lcdnX;
    displays[24] = ui->lcdnY;
    displays[25] = ui->lcdnZ;
    displays[26] = ui->lcdnUnknown;
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
//    for (int i = 0; i < 2; i++) {
        QString line = in.readLine();
        letters[i] = loadLetterFromString(line);
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
    if (patternsLoadedFromFile) {
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
    } else {
        printMessage("Cannot save as no patterns were loaded.");
    }
}

void MainWindow::on_sbMaxEpochs_valueChanged(int arg1)
{
    MAX_EPOCHS = arg1;
    QString word;
    QTextStream stream(&word);
    stream << "Max Epochs = " << MAX_EPOCHS << "\n";
    printMessage(word);
}

void MainWindow::on_hsLearningRate_sliderMoved(int position)
{
    LEARNING_RATE = position / 1000.0;
    ui->lcdnLearningRate->display(LEARNING_RATE);
}

void MainWindow::on_btnSaveWeights_clicked()
{
    QString filename = ui->pteSaveWeights->toPlainText();
    printMessage("Saving weights to " + filename + "\n");

    bp->saveWeights(relative_path + filename);

    printMessage("Saved weights.\n");
}

void MainWindow::on_btnClearLog_clicked()
{
//    printMessage("Testing");
//    int size = 3;
//    double sums[] = {1.0, 2.0, 3.0};
//    double actual[size];

//    bp->softmax(sums, actual, size);

//    QString word;
//    QTextStream str(&word);
//    for (int i = 0; i < size; i++) {
//        str << actual[i] << " ";
//    }
//    str << "\n";

//    printMessage(word);

    // ------------------------------------------------------

    printMessage("Initializing\n");
    double res[OUTPUT_NEURONS] = {0.1, 0.01, 0.2, 0.02, 0.023, 0.08,
                     0.04, 0.045, 0.0001, 0.000001, 0.00002, 0.0125,
                     0.0004, 0.000045, 0.000123, 0.0001, 0.0014, 0.000078,
                     0.025, 5.4, 0.0250, 0.0001, 0.0014, 0.000078,
                     0.04, 0.045, 0.0001};

    printMessage("Initialized\n");

    qDebug() << "Initialized";

    QString word;
    QTextStream str(&word);

    qDebug() << "Stream init";

    printLetter(0);

    qDebug() << "letter printed";
    str << "Test:: " << bp->check(letters[0], res) << "\nTest:: " << bp->check(letters[1], res);
    str << "\n";
    printMessage(word);
}

void MainWindow::on_btnTrainNetwork_clicked()
{
    if (patternsLoadedFromFile) {
        qDebug() << "Training started";
        int total = MAX_EPOCHS;
        int step = 10;
        int left = total;
        int running_total = 0;

        while (left > 0) {
            left -= step;

            if (left < 0) {
                step += left;
            }

            running_total += step;

            bp->trainNetwork(step);

            QString base = ui->pteSaveWeights->toPlainText().split(".")[0];
            QString filename;
            QTextStream str(&filename);
            str << base << "_" << running_total << ".txt";
            bp->saveWeights(relative_path + filename);
        }

        printMessage("Training complete.\n");
    } else {
        printMessage("Cannot train network as no patterns are loaded.");
    }
}

LetterStructure MainWindow::loadLetterFromString(QString line) {
    LetterStructure letter;
    QStringList parts = line.split(",");
    char sym = *(parts[0].toUtf8().data());

    letter.symbol = Symbol(sym - 65); // 65 is 'A'
    for (int j = 0; j < INPUT_NEURONS; j++) {
        letter.f[j] = atoi(parts[j+1].toUtf8().data());
    }

    for (int j = 0; j < OUTPUT_NEURONS; j++) {
        letter.outputs[j] = 0;

        if (j == letter.symbol) {
            letter.outputs[j] = 1;
        }
    }

    return letter;
}

void MainWindow::displayTestResults(double* results) {
    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        displays[i]->display(results[i]);
    }
}

void MainWindow::on_btnTestCustomData_clicked()
{
    QString sample = ui->pteCustomData->toPlainText();
    if (sample.size() < 33) {
        printMessage("Problem with test data. Too short.");
        return;
    } else {
        LetterStructure letter = loadLetterFromString(sample);
        double* results;
        results = bp->testNetwork(letter);
        displayTestResults(results);
    }
}

void MainWindow::on_btnTestNetwork_clicked()
{
    double ratioTrain = bp->testTrainingData();
    ui->lcdnTrainingPercentageOfGoodClassification->display(ratioTrain*100);
    ui->lcdnTrainingSumOfSquaredErrors->display(bp->getTrainMSE());
    double ratioTest = bp->testTestData();
    ui->lcdnTestPercentageOfGoodClassification->display(ratioTest*100);
    ui->lcdnTestSumOfSquaredErrors->display(bp->getTestMSE());
}

void MainWindow::on_btnInitialiseNetwork_clicked()
{
    bp->initialise();
    printMessage("Initialised.\n");
}

void MainWindow::on_btnShuffleTrainingData_clicked()
{
    if (patternsLoadedFromFile) {
        std::random_shuffle(&letters[0], &letters[NUMBER_OF_PATTERNS]);
        printMessage("Shuffled.\n");
    }
}

void MainWindow::on_btnLoadWeights_clicked()
{
    QString filename = ui->pteLoadWeights->toPlainText();
    qDebug() << "Loading weights from " << filename;

    bp->loadWeights(relative_path + filename);

    printMessage("Weights loaded.\n");
}

void MainWindow::on_btnSaveRelativePath_clicked()
{
    relative_path = ui->pteRelativePath->toPlainText();
    printMessage("Changed relative path to " + relative_path + "\n");
}
