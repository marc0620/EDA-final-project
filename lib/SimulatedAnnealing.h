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
    int temperature = 50000;
    vector<Net> nets;
    double previousCost, currentCost;
    vector<int> rowOccupied;
    vector<vector<Inst*>> currentBest;
    SimulatedAnnealing() {
        srand(time(NULL));
    };
    SimulatedAnnealing(int nn);

    void pinsLookUp(Inst* a, LibCell& b);
    void randomLayer(Die& die, vector<vector<LibCell>>& lib);

    int hpwlCalculator(vector<Net>& nets);
    int Cost(Die& die);
    char accept();
    void instMove(Die& die);
    void pinPlacer(Inst& inst);
    // void recover(Die& die);                                        // todo
    void entireProcedure(Die& die, vector<vector<LibCell>>& lib);  // todo
};
