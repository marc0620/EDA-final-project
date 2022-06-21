#ifndef OBJ
#define OBJ
#include "../lib/Objects.h"
#endif
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class SimulatedAnnealing {
   public:
    double temperature = 1000000.0;
    char mode;
    int cCount = 0;
    vector<Net> nets;
    double previousCost, currentCost, bestCost;
    vector<int> rowOccupied;
    vector<vector<Inst*>> currentBest;
    vector<Terminal>* terminals;
    vector<bool>* needTerminal;
    static bool instCompare(Inst* i1, Inst* i2);

    SimulatedAnnealing() {
        srand(time(NULL));
    };
    SimulatedAnnealing(int nn, char m);
    SimulatedAnnealing(int nn, char m, vector<Terminal>* t, vector<bool>* need);
    void pinsLookUp(Inst* a, LibCell& b);
    void randomLayer(Die& die, vector<vector<LibCell>>& lib);

    double hpwlCalculator(vector<Net>& nets);
    double Cost(Die& die);
    char accept();
    void instMove(Die& die);
    void pinPlacer(Inst* inst);
    void recover(Die& die);                                        // todo
    void entireProcedure(Die& die, vector<vector<LibCell>>& lib);  // todo
};
