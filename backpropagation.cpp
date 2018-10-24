#include "backpropagation.h"

#define sqr(x)	((x) * (x))

//FILE *out;

//#define LEARN_RATE		0.2

#define RAND_WEIGHT	( ((float)rand() / (float)RAND_MAX) - 0.5)

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)      (int)((float)(x)*rand()/(RAND_MAX+1.0))


///////////////////////////////////////////////////////////////////

void Backpropagation::debug(QString message) {
    qDebug() << message;
}

Backpropagation::Backpropagation()
{
   initialise();
}

void Backpropagation::initialise()
{
    err=0;
    sample=0;
    iterations=0;
    sum = 0;
    train_mse = 0.0;
    test_mse = 0.0;


    /* Seed the random number generator */
    srand( time(NULL) );

    assignRandomWeights();
}

double Backpropagation::getError_SSE(){
    return err;
}


////for debugging - very good
//void Backpropagation::saveWeights(){
//    int out, hid, inp;

//    QFile file3("weights.txt");
//    file3.open(QIODevice::WriteOnly | QIODevice::Text);

//    QTextStream out3(&file3);

//    char tempBuffer3[80];
//    QByteArray temp3;

//    //----------------------------------------------
//    // weights
//    //

//    qDebug() << "updating weights...";
//    qDebug() << "OUTPUT_NEURONS = " << OUTPUT_NEURONS;
//    qDebug() << "HIDDEN_NEURONS = " << HIDDEN_NEURONS;
//    qDebug() << "INPUT_NEURONS = " << INPUT_NEURONS;

//    // Update the weights for the output layer (step 4 for output cell)
//    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
//      temp3.clear();
//      for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//          //---save------------------------------------


//            ::sprintf(tempBuffer3,"who[%d][%d]=%f , ",hid,out,who[hid][out]);
//            temp3.append(tempBuffer3);

//          //---------------------------------------
//      }

//      // Update the Bias
//      //---save------------------------------------
//        ::sprintf(tempBuffer3,"who[%d][%d]=%f",HIDDEN_NEURONS,out,who[HIDDEN_NEURONS][out]);
//        temp3.append(tempBuffer3);
//        temp3.append("\n");
//        out3 << temp3;

//      //---------------------------------------

//    }

//    // Update the weights for the hidden layer (step 4 for hidden cell)
//    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//      temp3.clear();
//      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {

//        //---save------------------------------------


//          ::sprintf(tempBuffer3,"wih[%d][%d]=%f , ",inp,hid,wih[inp][hid]);
//          temp3.append(tempBuffer3);

//        //---------------------------------------
//      }

//      // Update the Bias
//      //---save------------------------------------
//        ::sprintf(tempBuffer3,"wih[%d][%d]=%f",INPUT_NEURONS,hid,wih[INPUT_NEURONS][hid]);
//        temp3.append(tempBuffer3);
//        temp3.append("\n");
//        out3 << temp3;

//      //---------------------------------------

//    }

//    //----------------------------------------------


//    file3.close();
//    qDebug() << "Weights saved to file.";
//}



//void Backpropagation::saveWeights(QString fileName){
//    int out, hid, inp;

//    QFile file3(fileName);
//    file3.open(QIODevice::WriteOnly | QIODevice::Text);

//    QTextStream out3(&file3);

//    char tempBuffer3[80];
//    QByteArray temp3;

//    //----------------------------------------------
//    // weights
//    //

//    qDebug() << "updating weights...";
//    qDebug() << "OUTPUT_NEURONS = " << OUTPUT_NEURONS;
//    qDebug() << "HIDDEN_NEURONS = " << HIDDEN_NEURONS;
//    qDebug() << "INPUT_NEURONS = " << INPUT_NEURONS;

//    // Update the weights for the output layer (step 4 for output cell)
//    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
//      temp3.clear();
//      for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//          //---save------------------------------------


//            ::sprintf(tempBuffer3,"%f,",who[hid][out]);
//            temp3.append(tempBuffer3);

//          //---------------------------------------
//      }

//      // Update the Bias
//      //---save------------------------------------
//        ::sprintf(tempBuffer3,"%f",who[HIDDEN_NEURONS][out]);
//        temp3.append(tempBuffer3);
//        temp3.append("\n");
//        out3 << temp3;

//      //---------------------------------------

//    }

//    // Update the weights for the hidden layer (step 4 for hidden cell)
//    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//      temp3.clear();
//      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {

//        //---save------------------------------------


//          ::sprintf(tempBuffer3,"%f,",wih[inp][hid]);
//          temp3.append(tempBuffer3);

//        //---------------------------------------
//      }

//      // Update the Bias
//      //---save------------------------------------
//        ::sprintf(tempBuffer3,"%f",wih[INPUT_NEURONS][hid]);
//        temp3.append(tempBuffer3);
//        temp3.append("\n");
//        out3 << temp3;

//      //---------------------------------------

//    }

//    //----------------------------------------------


//    file3.close();
//    qDebug() << "Weights saved to file.";
//}

void Backpropagation::saveWeights(QString filename) {
    qDebug() << "Saving weights to " << filename;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream output(&file);

    // Saving the weights for the output layer
    for (int out = 0; out < OUTPUT_NEURONS; out++) {
        for (int hid = 0; hid < HIDDEN_NEURONS; hid++) {
            output << who[hid][out] << ",";
        }
        // Save the bias
        output << who[HIDDEN_NEURONS][out] << "\n";
    }

    // Save the input - hidden layer weights
    for (int hid = 0; hid < HIDDEN_NEURONS; hid++) {
        for (int inp = 0; inp < INPUT_NEURONS; inp++) {
            output << wih[inp][hid] << ",";
        }
        // Save the bias
        output << wih[INPUT_NEURONS][hid] << "\n";
    }

    file.close();
    qDebug() << "Weights saved!";
}

void Backpropagation::loadWeights(QString filename) {
    qDebug() << "Loading weights from " << filename;
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (!file.exists()) {
        qDebug() << "ERROR! loadWeights - File doesn't exist.";
        return;
    }

    QTextStream in(&file);
    QString line;

    // Loading weights for hidden - output layers and bias
    for (int out = 0; out < OUTPUT_NEURONS; out++) {
        line = in.readLine();
        QStringList parts = line.split(",");
        for (int hid = 0; hid <= HIDDEN_NEURONS; hid++) {
            who[hid][out] = parts[hid].toDouble();
        }
    }

    // Loading the weights for the input - hidden layers and bias
    for (int hid = 0; hid < HIDDEN_NEURONS; hid++) {
        line = in.readLine();
        QStringList parts = line.split(",");
        for (int inp = 0; inp <= INPUT_NEURONS; inp++) {
            wih[inp][hid] = parts[inp].toDouble();
        }
    }

    file.close();
    qDebug() << "Weights loaded.";
}

//void Backpropagation::loadWeights(QString fileName){
//    int out, hid, inp;

//    QFile file3(fileName);
//    file3.open(QIODevice::ReadOnly | QIODevice::Text);

//    if(!file3.exists()){
//        qDebug() << "Backpropagation::loadWeights-file does not exist!";
//        return;
//    }

//    QTextStream in(&file3);

//    char tChar;
//    char tempBuffer3[80];
//    QByteArray temp3;

//    //----------------------------------------------
//    // weights
//    //

//    QString strLine;
//    //QTextStream streamLine;



//    qDebug() << "loading weights...";
//    qDebug() << "OUTPUT_NEURONS = " << OUTPUT_NEURONS;
//    qDebug() << "HIDDEN_NEURONS = " << HIDDEN_NEURONS;
//    qDebug() << "INPUT_NEURONS = " << INPUT_NEURONS;

//    // Update the weights for the output layer (step 4 for output cell)
//    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
//      strLine = in.readLine();
//      QTextStream streamLine(&strLine);

//      for (hid = 0 ; hid <= HIDDEN_NEURONS ; hid++) {
//          //---load------------------------------------

////            if(hid != HIDDEN_NEURONS-1){
////               streamLine >> who[hid][out] >> tChar;
////            } else {
////               streamLine >> who[hid][out];
////            }
//          streamLine >> who[hid][out] >> tChar;


//          //---------------------------------------
//      }

//      // Update the Bias
//      //---load------------------------------------

//        streamLine >> who[HIDDEN_NEURONS][out];// >> tChar;


//      //---------------------------------------

//    }

//    /* Update the weights for the hidden layer (step 4 for hidden cell) */
//    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
//      strLine = in.readLine();
//      QTextStream streamLine(&strLine);
//      for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {

//        //---load------------------------------------
////          if(hid != INPUT_NEURONS-1){
////             in >> wih[inp][hid] >> tChar;
////          } else {
////             in >> wih[inp][hid];
////          }

//          streamLine >> wih[inp][hid] >> tChar;

//        //---------------------------------------
//      }

//      // Update the Bias
//      //---load------------------------------------
////        in >> wih[INPUT_NEURONS][hid] >> tChar;
//      streamLine >> wih[INPUT_NEURONS][hid];

//      //---------------------------------------

//    }

//    //----------------------------------------------


//    file3.close();
//    qDebug() << "Weights loaded.";
//}

int Backpropagation::action( double *vector )
{
  int index, sel;
  double max;

  sel = 0;
  max = vector[sel];

  for (index = 1 ; index < OUTPUT_NEURONS ; index++) {
    if (vector[index] > max) {
      max = vector[index]; sel = index;
    }
  }

  return( sel );
}

double* Backpropagation::testNetwork(LetterStructure testPattern){
//    //retrieve input patterns
//    for(int j=0; j < INPUT_NEURONS; j++){
//       inputs[j] = testPattern.f[j];
// //       qDebug() << "f[" << j << "] = " << inputs[j];
//    }

    setInputs(testPattern);

    for(int i=0; i < OUTPUT_NEURONS; i++){
        target[i] = testPattern.outputs[i];
//        qDebug() << "desired_output[" << i << "] = " << target[i];
    }

    feedForward();


//    for (int out = 0 ; out < OUTPUT_NEURONS ; out++) {

//       qDebug() << "Output[" << out << "] = " << actual[out];

//    }

    return actual;


}

double* Backpropagation::testNetwork(LetterStructure testPattern, double &error) {
//    //retrieve input patterns
//    for(int j=0; j < INPUT_NEURONS; j++){
//       inputs[j] = testPattern.f[j];
//    }
    setInputs(testPattern);

    for(int i=0; i < OUTPUT_NEURONS; i++){
        target[i] = testPattern.outputs[i];
    }

    feedForward();

    for (int i = 0; i < OUTPUT_NEURONS; i++) {
        error += sqr(actual[i] - target[i]);
    }

    return actual;
}


double Backpropagation::trainNetwork(int NUMBER_OF_DESIRED_EPOCHS)
{
    if(!patternsLoadedFromFile) {
        qDebug() << "unable to train network, no training patterns loaded.";
        return -999.99;
    }
    double accumulatedErr=0.0;
    iterations=0;
    int epochs=0;
    err = 0.0;
    while (1) {

        if (++sample == NUMBER_OF_TRAINING_PATTERNS) {
            sample = 0;
            err = 0.0;
            epochs++;
        }

//        //retrieve input patterns
//        for(int j=0; j < INPUT_NEURONS; j++){
//           inputs[j] = letters[sample].f[j];
//        }
        setInputs(letters[sample]);

        for(int i=0; i < OUTPUT_NEURONS; i++){
            target[i] = letters[sample].outputs[i];
        }

        feedForward();

        /* need to iterate through all ... */

        //err = 0.0;
        for (int k = 0 ; k < OUTPUT_NEURONS ; k++) {

          err += sqr( (letters[sample].outputs[k] - actual[k]) );
        }

        err = 0.5 * err;

        accumulatedErr = accumulatedErr + err;

        if(epochs > NUMBER_OF_DESIRED_EPOCHS) {

            break;
        }

        backPropagate();

      }
      qDebug() << "training complete.";
      return accumulatedErr;
}




/*
 *  assignRandomWeights()
 *
 *  Assign a set of random weights to the network.
 *
 */

void Backpropagation::assignRandomWeights( void )
{
  int hid, inp, out;

  for (inp = 0 ; inp < INPUT_NEURONS+1 ; inp++) {
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      wih[inp][hid] = RAND_WEIGHT;
    }
  }

  for (hid = 0 ; hid < HIDDEN_NEURONS+1 ; hid++) {
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      who[hid][out] = RAND_WEIGHT;
    }
  }

}


/*
 *  sigmoid()
 *
 *  Calculate and return the sigmoid of the val argument.
 *
 */

double Backpropagation::sigmoid( double val )
{
  return (1.0 / (1.0 + exp(-val)));
}


/*
 *  sigmoidDerivative()
 *
 *  Calculate and return the derivative of the sigmoid for the val argument.
 *
 */

double Backpropagation::sigmoidDerivative( double val )
{
  return ( val * (1.0 - val) );
}

void Backpropagation::softmax(double* sums, double* smVals, int size) {
    double summedExp = 0.0;
    for (int i = 0; i < size; i++) {
        summedExp += exp(sums[i]);
    }

    for (int i = 0; i < size; i++) {
        smVals[i] = exp(sums[i]) / summedExp;
    }
}

void Backpropagation::softmax() {
    softmax(outputSums, actual, OUTPUT_NEURONS);
}


/*
 *  feedForward()
 *
 *  Feedforward the inputs of the neural network to the outputs.
 *
 */

void Backpropagation::feedForward( )
{
  int inp, hid, out;
  double sum;

  /* Calculate input to hidden layer */
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    sum = 0.0;
    for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
      sum += inputs[inp] * wih[inp][hid];
    }

    /* Add in Bias */
    sum += wih[INPUT_NEURONS][hid];

    hidden[hid] = sigmoid( sum );

  }

  /* Calculate the hidden to output layer */
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {

    sum = 0.0;
    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      sum += hidden[hid] * who[hid][out];
    }

    /* Add in Bias */
    sum += who[HIDDEN_NEURONS][out];

//    actual[out] = sigmoid( sum );
    outputSums[out] = sum;
  }

  softmax();

}


/*
 *  backPropagate()
 *
 *  Backpropagate the error through the network.
 *
 */

void Backpropagation::backPropagate( void )
{
  int inp, hid, out;

  /* Calculate the output layer error (step 3 for output cell) */
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      // Here I leave sigmoid derivative in as derivative because the derivative is the same as softmax derivative
    erro[out] = (target[out] - actual[out]) * sigmoidDerivative( actual[out] );
  }

  /* Calculate the hidden layer error (step 3 for hidden cell) */
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    errh[hid] = 0.0;
    for (out = 0 ; out < OUTPUT_NEURONS ; out++) {
      errh[hid] += erro[out] * who[hid][out];
    }

    errh[hid] *= sigmoidDerivative( hidden[hid] );

  }

  /* Update the weights for the output layer (step 4 for output cell) */
  for (out = 0 ; out < OUTPUT_NEURONS ; out++) {

    for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {
      who[hid][out] += (LEARNING_RATE * erro[out] * hidden[hid]);
    }

    /* Update the Bias */
    who[HIDDEN_NEURONS][out] += (LEARNING_RATE * erro[out]);

  }

  /* Update the weights for the hidden layer (step 4 for hidden cell) */
  for (hid = 0 ; hid < HIDDEN_NEURONS ; hid++) {

    for (inp = 0 ; inp < INPUT_NEURONS ; inp++) {
      wih[inp][hid] += (LEARNING_RATE * errh[hid] * inputs[inp]);
    }

    /* Update the Bias */
    wih[INPUT_NEURONS][hid] += (LEARNING_RATE * errh[hid]);

  }

}

double Backpropagation::testDataset(LetterStructure* letters, int size, int startIndex, int range) {
    int correct = 0;
//    double mse = 0.0;

    for (int i = startIndex; i < startIndex + range; i++) {
        int index = i % size;
        if (check(letters[index])) {
            correct++;
        }
    }

    double r = (double) range;

    return (double) (correct/r);
}

bool Backpropagation::check(LetterStructure letter) {
    double* res = testNetwork(letter);
    check(letter, res);
}

bool Backpropagation::check(LetterStructure letter, double &error) {
    double* res = testNetwork(letter, error);
    check(letter, res);
}

bool Backpropagation::check(LetterStructure letter, double* results) {
    bool ret = false;

    int index = 0;
    double highest = 0.0;
    for (int i = 0; i < OUTPUT_NEURONS; ++i) {
        if (results[i] > highest) {
            highest = results[i];
            index = i;
        }
    }

//    debug("check() - got index");

    if (letter.outputs[index] == 1) {
        ret = true;
    }

//    debug("Printing letter:");
//    debug(QString::fromUtf8(letter.toString().c_str()));

//    QString word;
//    QTextStream str(&word);
//    str << "largest index: " << index;

//    debug(word);
//    debug("finished");

    return ret;
}

double Backpropagation::testTrainingData() {
    train_mse = 0.0;
    int correct = 0;

    for (int i = 0; i < NUMBER_OF_TRAINING_PATTERNS; i++) {
        if (check(letters[i], train_mse)) {
            correct++;
        }
    }

    train_mse /= NUMBER_OF_TRAINING_PATTERNS;

    double total = (double) NUMBER_OF_TRAINING_PATTERNS;
    return (double) (correct/total);
}

double Backpropagation::testTestData() {
    test_mse = 0.0;
    int correct = 0;

    for (int i = NUMBER_OF_TRAINING_PATTERNS; i < NUMBER_OF_TRAINING_PATTERNS + NUMBER_OF_TEST_PATTERNS; i++) {
        if (check(letters[i], test_mse)) {
            correct++;
        }
    }

    test_mse /= NUMBER_OF_TEST_PATTERNS;

    double total = (double) NUMBER_OF_TEST_PATTERNS;
    return (double) (correct/total);
}

double Backpropagation::getTrainMSE() {
    return train_mse;
}

double Backpropagation::getTestMSE() {
    return test_mse;
}

void Backpropagation::setInputs(LetterStructure letter) {

    double range = 15;

    for (int i = 0; i < INPUT_NEURONS; i++) {
        inputs[i] = letter.f[i]/range;                // Here we normalize it so we have inputs in range 0->1
//        inputs[i] = letter.f[i];
    }
}
