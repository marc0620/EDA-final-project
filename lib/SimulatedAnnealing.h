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
    static int temperture;
    vector<Net> nets;
    int previousCost, currentCost;

    vector<vector<Inst*>> currentBest;
    SimulatedAnnealing() {
        srand(time(NULL));
    };
    SimulatedAnnealing(int nn);

    void pinsLookUp(Inst* a, LibCell& b);
    void randomLayer(Die& die, vector<vector<LibCell>>& lib);

    int hpwlCalculator(vector<Net>& nets);
    float rowOccupationRatio();
    int Cost(Die& die);
    char accept();
    void instMove(Die& die);
    void pinPlacer(Inst& inst);
};
