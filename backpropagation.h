#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <QDebug>

//-----------------------
//file manipulation
#include <QFile>
#include <QTextStream>
#include <QDataStream>
//-----------------------

#include "globalvariables.h"



class Backpropagation
{
public:
    Backpropagation();
    void initialise();
    void saveWeights(QString fileName);
    void loadWeights(QString fileName);
    void feedForward( );
    void backPropagate();
    double sigmoid( double val );
    double sigmoidDerivative( double val );
    void softmax(double* outputsums, double* actualvals, int size);
    void softmax();
    void assignRandomWeights( void );
    double trainNetwork(int NUMBER_OF_DESIRED_EPOCHS);
    double getError_SSE();
    int action( double *vector );
    double* testNetwork(LetterStructure testPattern);
    double* testNetwork(LetterStructure testPattern, double &error);
    double testDataset(LetterStructure* letters, int size, int startIndex, int range);
    bool check(LetterStructure letter, double* results);
    bool check(LetterStructure letter);
    bool check(LetterStructure letter, double &error);

    double testTrainingData();
    double testTestData();

    double getTrainMSE();
    double getTestMSE();

private:

    void debug(QString message);

    /* Weight Structures */

    /* Input to Hidden Weights (with Biases) */
    double wih[INPUT_NEURONS+1][HIDDEN_NEURONS];

    /* Hidden to Output Weights (with Biases) */
    double who[HIDDEN_NEURONS+1][OUTPUT_NEURONS];

    /* Activations */
    double inputs[INPUT_NEURONS];
    double hidden[HIDDEN_NEURONS];
    double target[OUTPUT_NEURONS];
    double actual[OUTPUT_NEURONS];
    double outputSums[OUTPUT_NEURONS];

    /* Unit Errors */
    double erro[OUTPUT_NEURONS];
    double errh[HIDDEN_NEURONS];

    //-----------------------------------------
    double err, train_mse, test_mse;
    int i, sample, iterations;
    int sum;
};

#endif // BACKPROPAGATION_H
