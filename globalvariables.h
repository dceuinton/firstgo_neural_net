#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <string>
#include <sstream>
#include <map>
#include <QString>

//------------------------------------------

#define INPUT_NEURONS		16
#define HIDDEN_NEURONS		12
#define OUTPUT_NEURONS		27

//------------------------------------------

enum Symbol{A=0, B=1, C=2, D=3, E=4, F=5,
            G=6, H=7, I=8, J=9, K=10, L=11,
            M=12, N=13, O=14, P=15, Q=16, R=17,
            S=18, T=19, U=20, V=21, W=22, X=23,
            Y=24, Z=25, UNKNOWN=26};

extern std::map<int, char> dictionary;

struct LetterStructure{
    Symbol symbol;
    int outputs[OUTPUT_NEURONS];
    int f[INPUT_NEURONS];

    std::string toString() {
        std::ostringstream s;
        s << "Symbol: " << symbol << "\n";
        for (int i = 0; i < INPUT_NEURONS; i++) {
            s << f[i] << " ";
        }
        s << "\n";
        for (int i = 0; i < OUTPUT_NEURONS; i++) {
            s << outputs[i] << " ";
        }
        s << "\n";
        return s.str();
    }
};

extern LetterStructure letters[20001];
extern LetterStructure testPattern;

//extern int NUMBER_OF_PATTERNS;
const int NUMBER_OF_PATTERNS = 20000;
const int NUMBER_OF_TRAINING_PATTERNS = 16000;
const int NUMBER_OF_TEST_PATTERNS = 4000;
extern bool patternsLoadedFromFile;
extern int MAX_EPOCHS;
extern double LEARNING_RATE;
extern QString relative_path;

#endif // GLOBALVARIABLES_H
