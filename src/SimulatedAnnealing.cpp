#ifndef SA
#define SA
#include "../lib/SimulatedAnnealing.h"
#endif  // !SA

using namespace std;
int SimulatedAnnealing::temperture = 1000000;
void SimulatedAnnealing::pinsLookUp(Inst* a, LibCell& b) {
    if (a->type == b.gettype()) {
        for (int i = 0; i < b.getpinNum(); i++) {
            a->pins[i].relativePosX = (*b.pins)[i].relativePosX;
            a->pins[i].relativePosY = (*b.pins)[i].relativePosY;
        }
    } else {
        cout << "error: copy wrong cell" << '\n';
    }
}
SimulatedAnnealing::SimulatedAnnealing(int nn) {
    nets.resize(nn);
    srand(time(NULL));
}
void SimulatedAnnealing::randomLayer(Die& die, vector<vector<LibCell>>& lib) {
    int r = 0, c = 0;
    if (die.instances.size() > die.colNum * die.rowNum) {
        cout << "error insts too much";
        return;
    } else {
        currentBest.resize(die.colNum);
        for (int i = 0; i < die.instNum; i++) {
            currentBest[i].resize(die.rowNum);
        }
        for (int i = 0; i < die.instances.size(); i++) {
            pinsLookUp(die.instances[i], (lib[die.tech][die.instances[i]->type]));
            if (r >= die.rowNum) {
                r = r % die.rowNum;
                c += 1;
            }
            die.grid[c][r] = die.instances[i];
            currentBest[c][r] = die.instances[i];
            die.instances[i]->posX = die.gridStartX + die.gridWidth * c;
            die.instances[i]->posY = die.gridHeight * r;
            pinPlacer((*die.instances[i]));
            for (int j = 0; j < die.instances[i]->pinNum; j++) {
                nets[die.instances[i]->pins[j].net].pins.push_back(&die.instances[i]->pins[j]);
            }
        }
    }
    previousCost = Cost(die);
}
void SimulatedAnnealing::pinPlacer(Inst& inst) {
    for (int i = 0; i < inst.pinNum; i++) {
        inst.pins[i].posX = inst.posX + inst.pins[i].relativePosX;
        inst.pins[i].posY = inst.posX + inst.pins[i].relativePosY;
    }
}
int SimulatedAnnealing::hpwlCalculator(vector<Net>& nets) {
    int sum = 0;
    for (int i = 0; i < nets.size(); i++) {
        if (nets[i].pins.size() == 0)
            continue;
        int left, right, top, bottom;
        left = nets[i].pins[0]->posX;
        right = left;
        top = nets[i].pins[0]->posY;
        bottom = top;
        for (int j = 1; j < nets[i].pins.size(); j++) {
            left = nets[i].pins[j]->posX < left ? nets[i].pins[j]->posX : left;
            right = nets[i].pins[j]->posX > right ? nets[i].pins[j]->posX : right;
            top = nets[i].pins[j]->posY > top ? nets[i].pins[j]->posY : top;
            bottom = nets[i].pins[j]->posX < bottom ? nets[i].pins[j]->posY : bottom;
        }
        sum += (right - left + top - bottom);
    }
    return sum;
}
int SimulatedAnnealing::Cost(Die& die) {
    int hpwl, cost;
    // int ror;
    hpwl = hpwlCalculator(nets);
    cost = hpwl;
    return cost;
}
void SimulatedAnnealing::instMove(Die& die) {
    int i = rand() % (die.instances.size());
    int x = rand() % (die.colNum);
    int y = rand() % (die.rowNum);
    bool swap = false;
    int originX = die.instances[i]->posX, originY = die.instances[i]->posY;
    if (die.grid[x][y] != nullptr) {
        die.grid[x][y]->posX = originX;
        die.grid[x][y]->posY = originY;
        pinPlacer((*die.grid[x][y]));
        swap = true;
    }
    die.instances[i]->posX = x * die.gridWidth;
    die.instances[i]->posY = y * die.gridHeight;
    pinPlacer((*die.instances[i]));
    currentCost = Cost(die);
    if (accept() == 'a') {
        if (swap) {
            currentBest[originX][originY] = die.grid[x][y];
            die.grid[originX][originY] = currentBest[originX][originY];
        } else {
            currentBest[originX][originY] = nullptr;
            die.grid[originX][originY] = nullptr;
        }
        currentBest[x][y] = die.instances[i];
        die.grid[x][y] = currentBest[x][y];
        previousCost = currentCost;
    } else if (accept() == 'b') {
        if (swap) {
            die.grid[originX][originY] = die.grid[x][y];
        } else {
            die.grid[originX][originY] = nullptr;
        }
        die.grid[x][y] = die.instances[i];
        previousCost = currentCost;
    } else {
        if (swap) {
            die.grid[x][y]->posX = x * die.gridWidth;
            die.grid[x][y]->posY = y * die.gridHeight;
            pinPlacer((*die.grid[x][y]));
        }
        die.instances[i]->posX = originX;
        die.instances[i]->posY = originY;
        pinPlacer((*die.instances[i]));
    }
}
char SimulatedAnnealing::accept() {
    if (currentCost - previousCost <= 0)
        return 'a';
    else
        return 'c';
}